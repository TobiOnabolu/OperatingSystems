

---
```
title: Not a complete list of Linux Commands
author: Sukhjit Singh Sehra
---
```



## Not a complete list of Unix/Linux Commands 



The following section discusses the Unix/Linux Commands along with its description. 

The command format is:

`commands [<*options*>] [<*argument(s)*>] `

*Note*: command names (like filenames) are *case sensitive*.

## System Commands

```
- date – show the current date and time
- cal – show this month's calendar
- uptime – show current uptime
- w – display who is online
- whoami – who you are logged in as
- finger user – display information about user
- uname -a – show kernel information
- cat /proc/cpuinfo – cpu information
- cat /proc/meminfo – memory information
- man command – show the manual for command
- df – show disk usage
- du – show directory space usage
- free – show memory and swap usage
- whereis app – show possible locations of app
- which app – show which app will be run by default
```

## File Commands

```
- ls – directory listing 
  - Options: ls -'options', e.g. ls -a
    -a: Show all (including hidden)
    -r: Reverse order
    -t: Sort by last modified
    -S: Sort by file size
    -l: Long listing format
    -1: One file per line
    -m: Comma-separated output
    -Q: Quoted output 
- cd dir - change directory to dir
- cd – change to home
- pwd – show current directory
- mkdir dir – create a directory dir
- rm file – delete file
- rm -r dir – delete directory dir
- rm -f file – force remove file
- rm -rf dir – force remove directory dir *
- cp file1 file2 – copy file1 to file
- cp -r dir1 dir2 – copy dir1 to dir2 ; create dir2 if it doesn't exist
- mv file1 file2 – rename or move file1 to file if file2 is an existing directory, moves file1 into directory file
- ln -s file link – create symbolic link link to file
- touch file – create or update file
- cat > file – places standard input into file
- more file – output the contents of file
- head file – output the first 10 lines of file
- tail file – output the last 10 lines of file
- tail -f file – output the contents of file as it grows, starting with the last 10 lines
```

### Explanation of `ls -l` output

- Example:

 ```
  total 6
  drwxr-xr-x  6 lucy cps444  480 Dec  6 17:42 .
  drwxr-xr-x 26 root system  816 Dec  6 14:45 .. 
  drwxr-xr-x  2 lucy cps444   80 Oct 25 08:33 bin
  drwx------  2 lucy cps444  144 Nov 30 14:25 C
  -rw-rw-r--  1 lucy cps444  104 Sep 12 19:54 notes
  -rwxrwxr--  1 lucy cps444   85 Jan  4 20:06 run*
  drw-rw-rw-  2 lucy cps444   32 Mar 23 13:45 memos
  drwx------ 16 lucy cps444 1296 Dec  6 17:33 text
  ```

- total line gives the number of blocks in the directory; a block in most UNIX systems = 1/2k (512 bytes), the above directory contains 3k
- the first character of each following row indicates the type of file: `d` = directory; `-` = plain file; `l` = link
- characters 2 thru 10 give the file access permissions (`r` = read; `w` = write; `x` = execute)
- characters 2, 3, 4 give the owner's permissions
- characters 5, 6, 7 given the group's permissions
- characters 8, 9, 10 given the other user's permissions
- `-` denotes absence of the permission
- number of links to the file
- file owner's user id
- group name for the file
- size of the file in bytes (characters)
- file creation or modification date and time, and the name of the file

## Process  Management and Process Monitoring

```
- [Per-process]
	- ps – display your currently active processes
	- top – display all running processes
	- kill pid – kill process id pid
	- strace - traces the system calls invoked by the user
	- gdb - a source level-debugger
- [System-wide]
	- vmstat - reports the memory usage
	- netstat - reports the statistics of network interfae
	- iostat - reports the I/O usage  for disks
	- perf - a collection of Linux performace tools
	- tcpdump - collects the network packets
```
## File Permissions

```
 - chmod octal_numbers file – change the permissions of file to octal , which can be found separately for user, group, and world by adding:
    ● 4 – read (r)
    ● 2 – write (w)
    ● 1 – execute (x)
Examples:
- chmod 777 – read, write, execute for all
- chmod 755 – rwx for owner, rx for group and world
```
## SSH

```
- ssh user@host – connect to host as user
- ssh -p port user@host – connect to host on port as user
- ssh-copy-id user@host – add your key to host for user to enable a keyed or passwordless login
```
## Compression

```
- tar -czf file.tar.gz files – create a tar with Gzip compression
- tar -xzf file.tar.gz – extract a tar using Gzip 
```
## Installation

```
Install from source:
./configure
make
make install
dpkg -i pkg.deb – install a package (Debian)
sudo apt install <package name>
```
## Shortcuts

```
Ctrl+C – halts the current command
Ctrl+Z – stops the current command, resume with fg in the foreground or bg in the background
Ctrl+D – log out of current session, similar to exit
!! - repeats the last command
exit – log out of current session
```



## Standard I/O redirection and Pipes

- Unix shells allow I/O to be redirected to other devices/files; thus UNIX commands are unaware of the device from which their input may originate or to what device their output may be sent

- The output redirection symbol, >, sends a command's output to the specified file instead of the console/terminal screen.

  - command name [args] > filename: For example
    - `ls -l > ls.out`  would create a  file `ls.out` if it does not exist (or emptied and overwritten if it exists prior to command execution) and sent the directory listing. 
  - programs which send their output to the console/terminal may also be redirected. For example
    - `./a.out >> out.txt`to save the output of program into a single file `out.txt`.

- The output redirection symbol >> appends the output to a file, rather than overwriting an existing file.

  ### pipes `|` are the logical extension of I/O redirection.

  - pipes allow the output of one program to become the input of another program.

  - specifically, a pipe redirects the standard output of the command to the immediate left from the screen to the standard input of the command to the immediate right.

    `ls -l | more`

    this command allows the viewing of the long listing of a large directory one screen at a time.

    `ls -l > ls.out`
    `more < ls.out`
    `rm ls.out`

    this I/O redirection is an equivalent set of commands as the prior, but requires the `ls.out` temporary file.

  - pipes support inter-process communication and introduce concurrency



## vi editor 

Editors such as vi and emacs are editors for programmers and power-users; they were designed for people who want to be extremely efficient and productive in their work. You may need to work on Vi editor (Vim editor - improved VI editor) for working with files through Ubuntu terminal. You can Google or [Visit Here](https://www.guru99.com/the-vi-editor.html) to get started with VI editor. 



## MakeFile and GCC For C programs

When you are working on C, C++ and others language and want to compile them from terminal Command. Makefile would run and compile your programs more efficiently. Makefile helps in reducing the typing again and again, a numbers of source files as well as linker flags, those are required during compilation of the program. 

A Makefile is a set of commands (similar to terminal commands) with variable names and targets to create object file and to remove them. You can compile your project (program) any number of times by using Makefile. Please [read here][https://www.gnu.org/software/make/manual/html_node/Introduction.html] to get started about makefile, and make.

We would use make file to run C programs by GNU Compiler Collection (GCC). Some introductory information can be [read from here][https://opensource.com/article/18/8/what-how-makefile]. If you want more detailed information on GCC, use the [document][https://tfetimes.com/wp-content/uploads/2015/09/An_Introduction_to_GCC-Brian_Gough.pdf] (Chapter 2). 



