/*
 * fs_op_symlink.c
 *
 *  Created on: Apr 10, 2020
 *      Author: aravindsomasundaram
 */
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <sys/stat.h>

#include "fs_util_dir.h"
#include "fs_util_file.h"
#include "max.h"

/**
 * symlink - create a symlink
 *
 * Errors
 *   -ENOENT   - file does not exist
 *   -ENOTDIR  - component of path not a directory
 *   -EISDIR   - cannot unlink a directory
 *
 * @param link_name absolute or relative path to existing file or directory
 * @param link_path path to symlink file
 * @return 0 if successful, or -error number
 */
int fs_symlink(const char* link_name, const char* link_path)
{
	// make directory entry
	int inum = do_mkentry(link_path, 0777, S_IFLNK);

	if (inum < 0) {
        return inum;
	}

	int path_len = strlen(link_name);

	int nwritten = do_write(inum, link_name, path_len, 0);

	return 0;
}




