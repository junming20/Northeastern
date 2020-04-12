/*
 * fs_op_mkdir.c
 *
 * description: fs_mkdir function for CS 5600 / 7600 file system
 *
 * CS 5600, Computer Systems, Northeastern CCIS
 * Peter Desnoyers, November 2016
 * Philip Gust, March 2019, March 2020
 */

#include <errno.h>
#include <sys/stat.h>

#include "fs_util_file.h"
#include "fs_util_path.h"
#include "fsx600.h"

/**
 *  mkdir - create a directory with the given mode. Behavior
 *  undefined when mode bits other than the low 9 bits are used.
 *
 * Errors
 *   -ENOTDIR  - component of path not a directory
 *   -EEXIST   - directory already exists
 *   -ENOSPC   - free inode not available
 *   -ENOSPC   - directory full
 *
 * @param path path to file
 * @param mode the mode for the new directory
 * @return 0 if successful, or -error number
 */
int fs_mkdir(const char* path, mode_t mode)
{
	// make regular entry
    int inum = do_mkentry(path, mode, S_IFDIR);

    if(inum < 0){
    	return inum;
    }

    // TEST: get the parent dirctory inum
    char leaf[FS_FILENAME_SIZE];
    int parent_inum = get_inode_of_file_path_dir(path, leaf);

    // TEST: make entry . and .. in the newly created directory
    int result = do_mklink(inum, inum, ".");
    if(result < 0){
    	return inum;
    }

    int result2 = do_mklink(parent_inum, inum, "..");
    if(result2 < 0){
    	return inum;
    }

    //TEST
    //return (inum < 0) ? inum : 0;
    return 0;
}


