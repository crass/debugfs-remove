// Copyright 2018, Glenn Washburn <development@efficientek.com>
// 
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.

// =====================================================================
// debugfs_remove - remove a debugfs path specified with the debugfs_path
//     modules parameter.
//
// author: Glenn Washburn <development@efficientek.com>
// date: 2018-09-26
// =====================================================================

// TODO: Allow removing debugfs paths root at the debugfs fs root when the
//       filesystem is not mounted.  Perhaps use debugfs_lookup in v4.18.
// TODO: Silently unload module in init.

#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/namei.h>


// User supplied debugfs_path module parameter
static char *debugfs_path = NULL;
module_param(debugfs_path, charp, 0400);
MODULE_PARM_DESC(debugfs_path, "debugfs path to remove");

// The GPL license is required for the module to load.
// Without it, insmod reports unknown symbols in the module.
MODULE_LICENSE("GPL");

// This is called when the module loads.
int init_module(void)
{
    int ret = 0;
    struct path path;

    // If debugfs is not enabled exit early
    if (!IS_ENABLED(CONFIG_DEBUG_FS))
        return 0;

    // Get path struct for debugfs_path
    if ((ret=kern_path(debugfs_path, LOOKUP_FOLLOW, &path))) {
        printk(KERN_ALERT "debugfs_remove: failed to get path %s (%d)\n",
               debugfs_path, ret);
        goto err;
    }

    // We must manually remove the debugfs entries we created. They are not
    // automatically removed upon module removal.
    debugfs_remove_recursive(path.dentry);

    // Decrement the reference count for the path created by kern_path
    path_put(&path);

    printk(KERN_ALERT "debugfs_remove: removed debugfs path %s, module not "
                      "needed, so failing to initialize.\n", debugfs_path);

    // kern_path should never return EPERM, so returning it here should not
    // be confused for a real error.  Always error as a convenience, so the
    // module does not need to be unloaded.
    return -EPERM;
err:
    return ret;
}

// This is called when the module is removed.
void cleanup_module(void)
{
}
