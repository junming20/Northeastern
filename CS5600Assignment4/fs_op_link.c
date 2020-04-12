/*
 * fs_op_open.c
 *
 * description: fs_open function for CS 5600 / 7600 file system
 *
 * CS 5600, Computer Systems, Northeastern CCIS
 * Peter Desnoyers, November 2016
 * Philip Gust, March 2019, March 2020
 */

#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <fuse.h>

#include "fs_util_path.h"
#include "fs_util_vol.h"
#include "fs_util_dir.h"
#include "fs_util_file.h"
#include "blkdev.h"
#include "max.h"

/**
 * link - make a hard link
 *
 * Errors
 *   -ENOSPC   - no free entry in directory
 *   -ENOTDIR  - component of path not a directory
 *   -EISDIR   - cannot link a directory
 *
 * @param src_path the source path
 * @param dst_path the destination path
 * @return 0 if successful, or -error number
 */
int fs_link(const char* src_path, const char* dst_path)
{
    // very similar to do_mkentry in fs_util_file
    // find the inode of the src_path file (fs_util_path), make sure this is a file not a directory itself f_type
	// find the inode of the directory to the dst_path
	// get a free block buffer, block entry no, block no from the directory to dst_path (fs_util_dir)
	// cast this buff into dirent struct
	// set the dir entry in this structure at the entry no, this automatically increments the hard link (fs_util_dir)
	// write this block to disk
	// increment size of directory by sizeof struct dirent
	// mark the directory inode as dirty
	// flush metadata

	int file_inum = get_inode_of_file_path(src_path);

    // report error if not found
    if (file_inum < 0) {
        return file_inum;
    }

    /* cannot link if it is directory */
    if (S_ISDIR(fs.inodes[file_inum].mode)) {
        return -EISDIR;
    }

	/* get directory inode */
    char leaf[FS_FILENAME_SIZE];
	int dir_inum = get_inode_of_file_path_dir(dst_path, leaf);


    // error getting directory inode
    if (dir_inum < 0) {
        return dir_inum;
    }

    /* get pointer to directory inode */
    struct fs_inode *din = &fs.inodes[dir_inum];
    if (!S_ISDIR(din->mode)) {
        return -ENOTDIR;	// path component not directory
    }

    // make sure entry does not exist in directory
    int blkno;
    char buf[FS_BLOCK_SIZE];
	if (get_dir_entry_block(dir_inum, buf, &blkno, leaf) >= 0) {
		return -EEXIST;	// leaf already exists
	}

	// find free directory entry
	int entno = get_dir_free_entry_block(dir_inum, buf, &blkno);
	if (entno < 0) {
		return -ENOSPC;	// no free directory entry
    }

	// set directory entry information
	struct fs_dirent *de = (void*)buf;
	set_dir_entry(&de[entno], file_inum, leaf);

    // write updated directory block to disk
    disk->ops->write(disk, blkno, 1, buf);

    // increment size of directory by one fs_dirent
    din->size += sizeof(struct fs_dirent);
    mark_inode(dir_inum);

    // flush metadata updates
    flush_metadata();

    return 0;
}


