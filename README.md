
Overview
========
DebugFS is a virtual filesystem in the kernel for kernel debugging.  DebugFS
paths created by kernel modules are not automatically cleaned up, so it is
possible that a buggy module creates some debugfs paths that it does not clean
up on module unload.  This module is intended to allow the user to perform this
clean up manually.

Specifically on 4.10 kernels the `nandsim` kernel module will create the debugfs
path `/sys/kernel/debug/nandsim`.  Under certain circumstances the module will
fail to clean up this path.  When this happens, the module will fail to load
again because it fails to create the already existing debugfs path.


Build
=====
Make sure that you have your kernel headers installed.

`make -C debugfs-remove`


Usage
=====

`insmod debugfs_remove.ko debugfs_path=/sys/kernel/debug/nandsim`


Example
=======
A real world exmple can be done with the 4.10 kernel.

```
root@ubuntu-nandsim:~/debugfs-remove.git# modprobe nandsim id_bytes=0x01,0xf1
modprobe: ERROR: could not insert 'nandsim': Input/output error

root@ubuntu-nandsim:~/debugfs-remove.git# lsmod | grep nandsim

root@ubuntu-nandsim:~/debugfs-remove.git# ls /sys/kernel/debug/nandsim -ld
drwxr-xr-x 2 root root 0 Sep 26 20:28 /sys/kernel/debug/nandsim

root@ubuntu-nandsim:~/debugfs-remove.git# insmod debugfs-remove/debugfs_remove.ko debugfs_path=/sys/kernel/debug/nandsim

root@ubuntu-nandsim:~/debugfs-remove.git# dmesg |tail -n 5
[66570.572170] nand: device found, Manufacturer ID: 0x01, Chip ID: 0xf1
[66570.572171] nand: AMD/Spansion NAND 128MiB 3,3V 8-bit
[66570.572172] nand: 128 MiB, SLC, erase size: 64 KiB, page size: 1024, OOB size: 16
[66570.572250] [nandsim] error: init_nandsim: unknown page size 1024
[66779.749925] debugfs_remove: removed debugfs path /sys/kernel/debug/nandsim, module not needed, so failing to initialize.
```
