

```
---
title: A quick look of Linux Logs and System Performance tools
author: Sukhjit Singh Sehra
---
```

# Log files

Log files are the records that Linux stores for administrators to keep track and monitor important events about the server, kernel, services, and applications running on it. Ubuntu provides a centralized repository of log files that can be located under the  /var/log directory. Four types of logs are managed in Ubuntu:

*   Application Logs
*   Event Logs
*   Service Logs
*   System Logs

## Ubuntu Log Files

- **/var/log/syslog**: Shows general messages and info regarding the system. Basically a data log of all activity throughout the global system. 

- **/var/log/auth.log**: Keep authentication logs for both successful or failed logins, and authentication processes. Storage depends on system type. 
- **/var/log/kern**: keeps in Kernel logs and warning info. Also useful to fix problems with custom kernels.
- **/var/log/dmesg**: a repository for device driver messages. Use **dmesg** to see messages in this file.
- **/var/log/faillog:** records info on failed logins. Hence, handy for examining potential security breaches like login credential hacks and brute-force attacks.
- **/var/log/cron**: keeps a record of Crond-related messages (cron jobs). Like when the cron daemon started a job.
- **/var/log/btmp**: keeps a note of all failed login attempts.
- **/var/log/utmp**: current login state by user.
- **/var/log/wtmp**: record of each login/logout.
- **/var/log/lastlog**: holds every user’s last login. A binary file you can read via **lastlog** command.
- **/var/log/dpkg.log**:  lists all installs, upgrades, removes and purges on the system.
- **/var/log/dist-upgrade/**: helpful in debugging the issue.



## proc - process information pseudo-filesystem

The proc filesystem is a pseudo-filesystem which provides an interface to kernel data structures.  It is commonly mounted at /proc.  This **/proc** directory is **NOT** a real **File System**, but a **Virtual File System**. 

A quick rundown on **/proc’s** files:

1. **/proc/cmdline** – Kernel command line information.
2. **/proc/console** – Information about current consoles including tty.
3. **/proc/devices** – Device drivers currently configured for the running kernel.
4. **/proc/dma** – Info about current DMA channels.
5. **/proc/fb** – Framebuffer devices.
6. **/proc/filesystem**s – Current filesystems supported by the kernel.
7. **/proc/iomem** – Current system memory map for devices.
8. **/proc/ioports** – Registered port regions for input output communication with device.
9. **/proc/loadavg** – System load average.
10. **/proc/locks** – Files currently locked by kernel.
11. **/proc/meminfo** – Info about system memory.
12. **/proc/cpuinfo** – Info about cpu.
13. **/proc/misc** – Miscellaneous drivers registered for miscellaneous major device.
14. **/proc/modules** – Currently loaded kernel modules.
15. **/proc/mounts** – List of all mounts in use by system.
16. **/proc/partitions** – Detailed info about partitions available to the system.
17. **/proc/pci** – Information about every PCI device.
18. **/proc/stat** – Record or various statistics kept from last reboot.
19. **/proc/swap** – Information about swap space.
20. **/proc/uptime** – Uptime information (in seconds).
21. **/proc/version** – Kernel version, gcc version, and Linux distribution installed.

Within **/proc’s** numbered directories you will find a few **files** and **links**. Remember that these directories’ numbers correlate to the **PID** (process ID) of the process running on the system. Let’s use an example. On my system, there is a folder name **/proc/12**:

```
# cd /proc/12
# ls
```



## Straces

Strace is a debugging tool that will help you troubleshoot issues. Strace monitors the system calls and signals of a specific program. 

Strace monitors the system calls and signals of a specific program. It is helpful when you do not have the source code and would like to debug the execution of a program. strace provides you the execution sequence of a binary from start to end.



### 1\. Trace the Execution of an Executable

You can use strace command to trace the execution of any executable. The following example shows the output of strace for the Linux ls command.

$  strace ./filecopy


### 2\. Trace a Specific System Calls in an Executable Using Option -e

Be default, strace displays all system calls for the given executable. To display only a specific system call, use the strace -e option as shown below.

$ strace -e open ./filecopy

### 3\. Save the Trace Execution to a File Using Option -o

The following examples stores the strace output to output.txt file.

$ strace -o output.txt ./filecopy

### 4. Print Timestamp for Each Trace Output Line Using Option -t

To print the timestamp for each strace output line, use the option -t as shown below.

$ strace -t -e open ./filecopy


### 5. Print Relative Time for System Calls Using Option -r

Strace also has the option to print the execution time for each system calls as shown below.

$ strace -r ./filecopy

### 6\. Generate Statistics Report of System Calls Using Option -c

Using option -c, strace provides useful statistical report for the execution trace. The “calls” column in the following output indicated how many times that particular system call was executed.

$ strace -c ./filecopy



## BCC 



[BCC or BPF Compiler Collection is a set of tools leveraging eBPF for kernel tracing.](https://www.iovisor.org/technology/bcc)

1. [Installation](https://github.com/iovisor/bcc/blob/master/INSTALL.md)
2. [Getting Started](https://github.com/iovisor/bcc/blob/master/docs/tutorial.md)

These tools may be installed on your system under /usr/share/bcc/tools, go to the folder and run the following command to trace open() calls performed by PID 985.

> sudo ./opensnoop -p 985