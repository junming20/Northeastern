# assignment-4
team members:
Junming Zhao(id:junming20), 
Hongming Zhang(id:runpirate),
Aravind Somasundaram (aravindsomu)
Mian Huang (mxh810)
Xintong Liu (liuxinto01)

A Virtual File System
Background
In lecture 8, we studied disk storage devices whose storage is divided into sectors of a track on a platter. The low-level disk controller translates this structure into a linear sequence of fixed-size blocks that can be addressed by their block numbers. This is known as a block device. We saw how file systems can be implemented on these block devices, and studied the architecture of a file and directory system.
In this assignment, you will have an opportunity to make several significant extensions to a simple version of this type of file system as a virtual file system that is supported by a file system driver, but is implemented as a user-space application. These virtual file systems use regular files on the host operating system as block-oriented devices.

Our fsx600 file system uses Fuse (File system in USEr space) to implement a file system that can be mounted and used just like a regular file system. The design of this file system has a superblock for the volume, inode and block bitmaps to track free inodes and blocks, inodes that represent files and directories, and a pool of blocks that can be allocated and used.

The design of this file system is described in the lecture 8 slides and in this PDF document that summarizes the design. The design is based on Ext2 -- Second Extended File System, a popular file and directory system design that has been in wide use, and serves as the base for Ext3 and other more advanced versions.

Starting Code
The code for this assignment resides in two repositories. The "2020SPCS5600SV/assignment-4" repository is the code you will use to implement the assignment. It contains the following files:

fsx600.h - structure definitions
blkdev.h - structure definitions for virtual block devices
image.h - external declaration of a virtual block device
image.c - implementaiton of a virtual device function
misc.c - main program for virtual file system
It also contains a group of files of the form "fs_op_*.c". These implement functions called by the Fuse virtual file system that correspond to the standard POSIX C I/O functions. Another group of files of the form "fs_util*.c" provide supporting functions for file and directory system access, path decoding, and other operations.

This group of files provide a simple but fully-working Ext2 virtual file system, that will be the starting point for this assignment. You will modify these files and create several new ones for the tasks described below.

The "2020SPCS5600SV/assignment-4-util" repository has utility programs that you will use for development and testing. It contains the following files:

fsx600.h - structure definitions (duplicate)
mkfs-x6.c - program to generate empty file system image
mktest.c - program to generate test file system image
read-image.c - program to read and check file system images
The 'mkfs-x6' program is used to generate an empty disk image of a specified size (e.g. ./mkfs-x -size 50M big.img).

The 'mktest' program creates a 1MB disk image that contains several files and directories (e.g. ./mktest test.img).

When testing, be sure to create a clean disk image each time to ensure that your image has not become corrupted corrupted.

Assignment
Review and understand the existing implementation
You first task is to read and understand the existing virtual file system implementation.

Read the implementation of the interactive command interpreter in "misc.c". Start by creating a test virtual file system using "maketest.c" and using it to run misc.c interactively. Read the code in "misc.c" that implements each command to see how it calls the underlying "fs_op_*.c" functions. Also read the 'man' pages for the corresponding basic system I/O functions (e.g. "read" for "fs_op_read.c" (function name: fs_read).
Study the basic file and directory system data structures in "fsx600.h". These match the data structures that we looked at in lecture 8.
Study the simulated block device that is implemented by "image.c", "image.h" and "blkdev.h". This provide block-oriented read and write functions using the file you crated using "mktest".
Read each of the FUSE file operations for our virutal file system to understand how each function works. You saw these functions in action when studying "misc.c". Now you will look at how they are implemented. You can either stop and read the utility functions now, or just understand what they do from their documentation and study them separately.
Always draw pictures of data structures and make notes as you read through the code. At the end of your studies, you should have a good understanding of the current code. Do not start making changes until you are sure that you completely understand the existing code. The code is highly modularized so that it is easy to work on individual pieces of functionaity, and to start over with a fresh version if changes go too badly wrong.

Add directory entries for "." and ".."
Every directory in a POSIX file system always includes two entries. One entry "." points to the inode of the directory itself. The other entry ".." points to the inode of the parent directory. This scheme relies on directories being organized as a tree with the root directory as the root of the tree. The ".." entry in the root directory points to its own inode.

Modify the operation and utility functions that create and manage directories to always create entries for "." and ".." in every new directory. These entries cannot be renamed, moved, or deleted.
A directory is considered to be empty if it contains only these two entries. Ensure that the implementation of the 'fs_rmdir" and its supporting functions recognize that a directory containing only "." and ".." is treated as empty and allows the directory to be deleted.
The reference count of an empty directory or file in the current implemention is 1 because it is only pointed to by its parent directory. Now, the reference count of a parent directory must also be increased when a new child directory is created because the child directory's '..' entry points to its parent. When a child directory is removed, the reference count of the parent directory must also be decremented because the child directory no longer points to it.
Multi-block directories
The current implementation simplifies directory handling by using only one block per directory. Extend directory handling by allowing any number of entries in a directory.

The simplest implementation is to use the same mechanism to add directory blocks as is done for adding file blocks. This is not efficient in either space or time, but it is sufficient for this exercise. (An Ext3 extension to Ext2 uses an HTree instead of a linear list of blocks.) You need not try to reclaim directory blocks once allocated.

File truncation
The current implementation of fs_truncate() only supports truncation of a regular file to 0 length. Extend this to allow truncating a file to a specified length betwen 0 and its current length. This means freeing blocks beyond the one that contains the specified length, and setting the inode size to the specified length.

The Posix truncate() function also allows expanding the file by specifying a value that is longer than the current length. This requires adding enough blocks so that the capacity is large enough to hold that many bytes, and setting the specified file size. The function sets the additional bytes to 0s.

Create a "hard" link
A "hard link" or just "link" is an entry in a directory for an inode. We have already seen how entries can be created for new content. Now we will look at another ability to create multiple links for the same file. For this exercise, we will exclude creating a hard link for a directory so that a directory hierarchy is a directory tree. More advanced file systems lift this restriction.

Add a new file "fs_op_link.c" that implements the "fs_link" function, add the function decaration to "fs_ops.h" and add the function pointer to the struct in "fs_ops.c" using the selector ".link".
Given a source path and a target path, this function creates a directory entry corresponding to the target path that uses the same inode as the one for the source path. The result is that a file can appear in the same directory twice with different names, or in differnt directories with the same or a different name.
The source file must exist, and the target must not already exist.
A hard link increments the reference count of the underlying inode. When a file is unlinked, its reference count is decremented. If the reference count s 0, the inode and file blocks are freed. Otherwise other references to the file continue to have access to it.
When a file is opened, its reference count is incremented. When the file is closed, its reference count is decremented.
Modify "misc.c" to implement a new two-parameter "link" command with a new "do_link" function. The first parameter is an existing file, and the second parameter is the name of a file that will share the same inode.
Allow rename to move a file or directory between directories
The current implementation of the "fs_rename" function in "fs_op_rename.c" can only rename a file or directory within the same parent direcctory. Modify the implementaiton of this function to also allow moving the file to another directory, either with the same or a different name. Your implementation could benefit from using the new hard link functionality rather than simply changing the name in the directory entry.

Modify this function to allow a file or directory (except the "/", ".", or "..") to be moved to a different directory and replace an existing entry. Per the rename(2) man page:

If target exists, the source and target must be the same type, and if it is a directory, it must be empty.
If the target exists, it is first unlinked.
A link is then created in the target directory, then the link is removed in the source directory if that was successful.
Create a "symbolic" link (symlink)
A "symbolic link" is an special kind of link that links to a file using its name, rather than sharing its inode. A symbolic link is a special file type (S_IFLINK) that stores the name of the linked-to file as the content of the inode.

When a file operation like read() occurs for a symlink, the file system reads the content of the symlink inode, and uses the inode assoociated with the stored name instead. If that inode is also a symlink, the file system continues chaining until a non-symlink inode is reached. This is the inode used for the command.

Unlike hard links, it is possible to form loops, so a robust resolution process puts a limit on the number of indirections. It is also possible to create a symlink to a non-existent file or directory. The resolution process must also handle that situation. Determining whether a symlink is to a regular file or a directory or to no file at all can only be done after the resolution process.

The link-to path string for a symlink can be stored in one or more data blocks associated with the inode. Since may symlinks are short, a common optimization is to use the bytes of the direct pointers array in the inode to store the string if it is short enough, rather than allocating data blocks. This reqires special-case handling in several places based on the file type.

Add a new file "fs_op_symlink.c" that implements the "fs_symlink" function, add the function decaration to "fs_ops.h" and add the function pointer to the struct in "fs_ops.c" using the selector ".symlink".
This function makes a new node with the type 'S_IFLINK'. At this point, you can just use the link-to string as its content. Consider the short link=to string as an optimization once you have the basic functionality working. Ensure that the size field in the inode is set to the length of the link-to string.
Add a new file "fs_op_readlink.c" that implements the "fs_readink" function, add the function decaration to "fs_ops.h" and add the function pointer to the struct in "fs_ops.c" using the selector ".symlink".
This function reads the link information from the symlink inode. It does not resolve the symlink. The function returns 0 if the operation is successful and -error in case of error. The buffer passed in should be filled with a null terminated string. The link name stored in the symlink is not null terminated. The buffer size argument includes the space for the terminating null character. If the link name is too long to fit in the buffer, it should be truncated.
for each FUSE function except fs_readlink, add code to resolve the symlink before performing any checks on the resulting inode, such as whether it is a directory or a regular file, A suggestion is to create new "fs_util_symlink.c" and "fs_util_symlink.h" files with utility functions for resolving symlinks that can be used by all FUSE functions.
Modify "misc.c" to implement a new two-parameter "symlink" command with a new "do_symlink" function. The first parameter is an existing file, and the second parameter is the name of the symlink file that will hold the first parameter string.
Modify "misc.c" to implement a new one-parameter "readlink" command with a new "do_readlink" function. The parameter is an existing symlink. The function prints the link-to string from they symlink.
Modify the "_strmode" function in "misc.c" to display 'l' as the file type character, just as it currently does for directory ('d') and regular file ('-').
Modify the "dashl_filler" function to display " -> link-to" at the end of each symlink file following the file name to show the link-to string returned by the readlink function. This is the behavior of the "ls -l" long-list option.
Testing
The main program for the assignment can be run in two modes:

as an interactive program with a bash-like command interpreter (e.g. ./assignment-4 -cmdline -image foo.img).
as a non-interactive program that mounts your file system at a location on disk (e.g. ./assignment-4 -image foo.img ~/fusefs ).
See the design documentation and the code for details on running the program in both modes.
You should test your code interactively using the command mode, and then view the results using the "read-image" program. Note that it is possible to script the execution of the interactive program and to capture the output for comparison purposes. This is not required but may facilitate your testing process.

When you run the misc.c program from the command line or the IDE, simply redirect a script file to standard input, and capture its standard output to a file. As you make changes, you can re-run your existing scripts and compare the results to expected output files. You can also create additional scripts for new features. Your team can share these scripts and output files to check for any regressions in behavior during development. You can even create a bash script that automatically generates a new disk image, image, runs one or more scripts, and then compares the result files.

Once you have completed your implementation, you can test it by mounting your file system as described earlier, and performing operations on files in the subdirectory using standard shell utilities and/or scripts. Be aware of some limitations of this implementation when performing operations on the mounted file system.

The program mounts your file system at the specified mount point and then exits. To unmount your file system on Microsoft Windows, run

fusermount -u <mountpoint>

On MacOS and Linux, use the system umount command:

umount <mountpoint>

We can have futher discussions about implementation and testing strategies in class and on Piazza.

Project Staffing
You can work in teams of 2-4 people on this assignment. The README.md file must show the name and the CCIS ID of each team member. As with past assignments, please turn in all the files from the original assignment, even though you have not modified some of them. This will make it easier for the graders to run your code.

Note: This is a very challenging assignment, and you are strongly advised to work in teams of 4 and only in teams of 2 or 3 if you must. Your team will need every bit of time available to complete this project on time. Please do not procrastinate. Plan your work and stay on schedule. Starting one week before the assignment is due is a recipe for failure.

Turning in the assignment
Turn your assignment in by uploading all the .c and .h files to your 2020SPCS5600SV/assignment-4-ccis_id repository. Do not turn in other files that may have been created by your IDE. The graders will download and run your code, so be sure that all the files are avaiable and no extraneous files have been checked in. Once you have checked in your code, it is a good idea to verify that what you checked in is actually in the repository to avoid delays in grading your assignment.

You are welcome to check in intermediate versions during development as a backup to what is on your computer. Only the final version will be graded.

Important: If you are working as part of a team, all of you should check in your work in your respective repositories. You both must also edit your "README.md" files to list the names and the CCIS-IDs of both of you.
