/*
 * fs_op_symlink.c
 *
 *  Created on: Apr 10, 2020
 *      Author: aravindsomasundaram
 */
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

#include "fs_util_vol.h"
#include "fs_util_file.h"
#include "fs_util_path.h"
#include "fs_util_dir.h"
#include "blkdev.h"
#include "max.h"

/**
 * readlink - read a symlink
 *
 * Errors
 *   -EISDIR   - not a symlink file
 *
 * @param path to the symlink file
 * @param sympath path referenced in symlink file
 * @oaram max_path_len maximum length of path referenced by symlink
 * @return 0 if successful, or -error number
 */
int fs_readlink(const char* path, char* sympath, size_t max_path_len)
{
	/* get directory inode */
	char leaf[FS_FILENAME_SIZE];
	int dir_inum = get_inode_of_file_path_dir(path, leaf);
	if (dir_inum < 0) {
	    return dir_inum;
	}

	int inum = get_dir_entry_inode(dir_inum, leaf);
	if (inum < 0) {
		return inum;
	}

	// make sure that the file is of type symlink
    if (!S_ISLNK(fs.inodes[inum].mode)) {
        return -EISDIR;
    }

	int path_len = do_read(inum, sympath, max_path_len, 0);
	sympath[path_len] = '\0';

	return 0;
}




