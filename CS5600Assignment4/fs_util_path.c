/*
 * fs_util_path.c
 *
 * description: path utility functions for CS 5600 / 7600 file system
 *
 * CS 5600, Computer Systems, Northeastern CCIS
 * Peter Desnoyers, November 2016
 * Philip Gust, March 2019, March 2020
 */

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "fs_util_dir.h"
#include "fs_util_path.h"
#include "fs_util_file.h"
#include "fs_util_vol.h"
#include "split.h"

/** File path delimiter character */
const char* PATH_DELIM = "/";

/* Return inode number for specified file or directory path.
 * Given "/a/b/c", returns the inode number for "c".
 *
 * Errors
 *   -ENOENT  - a component of the path is not present.
 *   -ENOTDIR - an intermediate component of path not a directory
 *
 * @param path the file path
 * @return inode of path node or error
 */
int get_inode_of_file_path(const char* path)
{
    int pathlen = split(path, NULL, 0, "/");  // count path names

    // array of path names
    char* names[pathlen];
    split(path, names, pathlen, "/");

    // get inum for path
    int inum = fs.root_inode;


    int prev_inum = inum;
    int path_len;

    for (int i = 0; i < pathlen && inum > 0; i++) {
    	prev_inum = inum;
        inum = get_dir_entry_inode(inum, names[i]);
        if (inum < 0) {  // error getting inum
        	break;
        }

        // check if inode file is that of a symlink
		if (S_ISLNK(fs.inodes[inum].mode)) {
			char sympath[1024];
			path_len = do_read(inum, sympath, 1024, 0);
			sympath[path_len] = '\0';

			inum = get_inode_of_symlink_path(prev_inum, sympath);

			if (inum < 0) {  // error getting inum
				break;
			}
		}
    }

    return inum;
}

/**
 *  Return inode number of directory for specified file
 *  or directory path, and a leaf name that may not yet
 *  exist. Given "/a/b/c", returns inode number for "b"
 *  and the leaf "c".
 *
 * Errors
 *   -ENOENT  - a component of the path is not present.
 *   -ENOTDIR - an intermediate component of path not a directory
 *
 * @param path the file path
 * @param leaf pointer to space for FS_FILENAME_SIZE leaf name
 * @return inode of path node or -error
 */
int get_inode_of_file_path_dir(const char* path, char* leaf)
{
    int pathlen = split(path, NULL, 0, "/");  // count path names

    // array of path names
    char* names[pathlen];
    split(path, names, pathlen, "/");

    int inum = fs.root_inode;
    int prev_inum = inum;
    int path_len;

    int i;
    for (i = 0; i < pathlen-1 && inum > 0; i++) {
    	prev_inum = inum;
        inum = get_dir_entry_inode(inum, names[i]);
        if (inum < 0) {  // error getting inum
        	break;
        }

        // check if inode file is that of a symlink
		if (S_ISLNK(fs.inodes[inum].mode)) {
			char sympath[1024];
			path_len = do_read(inum, sympath, 1024, 0);
			sympath[path_len] = '\0';

			inum = get_inode_of_symlink_path(prev_inum, sympath);

			if (inum < 0) {  // error getting inum
				break;
			}
		}
    }

    if (inum >= 0) {  // if directory num valid
    	strcpy(leaf, names[i]);
    }

    return inum;
}

/* Return inode number referenced by a symlink path
 *
 * Errors
 *   -ENOENT  - a component of the path is not present.
 *   -ENOTDIR - an intermediate component of path not a directory
 *
 * @param entry_inum the inum of the directory containing the symlink file
 * @param path the relative or absolute path referenced by the symlink file
 * @return inode of referenced path node or error
 */
int get_inode_of_symlink_path(int entry_inum, const char* path)
{
    int pathlen = split(path, NULL, 0, "/");  // count path names

    // array of path names
    char* names[pathlen];
    split(path, names, pathlen, "/");

    // get inum for path
    int inum = fs.root_inode;

    // check if path is relative
    if(path[0] != '/'){
    	inum = entry_inum;
    }

    int prev_inum = inum;
    int path_len;

    for (int i = 0; i < pathlen && inum > 0; i++) {
    	prev_inum = inum;
        inum = get_dir_entry_inode(inum, names[i]);
        if (inum < 0) {  // error getting inum
        	break;
        }

        // check if inode file is that of a symlink
		if (S_ISLNK(fs.inodes[inum].mode)) {
			char sympath[1024];
			path_len = do_read(inum, sympath, 1024, 0);
			sympath[path_len] = '\0';

			inum = get_inode_of_symlink_path(prev_inum, sympath);

			if (inum < 0) {  // error getting inum
				break;
			}
		}
    }

    return inum;
}

