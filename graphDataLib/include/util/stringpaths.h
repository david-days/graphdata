//
// Created by david on 11/24/23.
//

#ifndef PATHFUNCS_H
#define PATHFUNCS_H

#ifndef PATH_BUF
#define PATH_BUF 1024
#endif

/**
 * Generates a UUID string and places it in the allocated uchars array
 *
 * @param uchars Pre-allocated array of UUID_STR_LEN length
 * @return OP_SUCCESS if successful, otherwise OP_FAIL
 */
short gen_uuid(char *uchars);

/**
 * Utility function to check if the given character string is a path to directory
 * @param path Path to check
 * @return true if it's a directory; otherwise, false
 */
bool isDirectory(const char *path);

/**
 * Utility function to check if the given character string is a path to file
 * @param path Path to check
 * @return true if it's a file; otherwise, false
 */
bool isFile(const char *path);

/**
 * Utility function to check if the given character string is a path to a character device
 * @param path Path to check
 * @return true if it's a character device; otherwise, false
 */
bool isCharDev(const char *path);

/**
 * Utility function to check if the given character string is a path to block device
 * @param path Path to check
 * @return true if it's a block device; otherwise, false
 */
bool isBlockDev(const char *path);


/**
 * Utility function to check if the given character string is a path to a FIFO
 * @param path Path to check
 * @return true if it's a FIFO; otherwise, false
 */
bool isFifo(const char *path);

/**
 * Utility function to check if the given character string is a path to a symlink
 * @param path Path to check
 * @return true if it's a symlink; otherwise, false
 */
bool isSymlink(const char *path);

/**
 * Utility function to check if the given character string is a path to a socket
 * @param path Path to check
 * @return true if it's a socket; otherwise, false
 */
bool isSocket(const char *path);

/**
 * Utility function to determine if a path exists (regardless of type)
 * @param path Path to check
 * @return true if it's a socket; otherwise, false
 */
bool pathExists(const char *path);

/**
 * Utility function to get the current working directory
 * @param path buffer of size PATH_BUFF
 * @returns true if the path was found and presentable as a string; otherwise, false
 */
bool getCwd(char *pathBuff);
#endif //PATHFUNCS_H
