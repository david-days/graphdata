//
// Created by david on 11/24/23.
//

#include <stdlib.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include <graphData.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stringpaths.h>
#include <unistd.h>
#include <errno.h>

short gen_uuid(char *uchars) {
    short retVal = OP_SUCCESS;
    uuid_t binuuid;
    uuid_generate_random(binuuid);
    uuid_unparse(binuuid, uchars);
    return retVal;
}

bool isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode) == 1 ? true : false;
}

bool isFile(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISREG(statbuf.st_mode) == 1 ? true : false;
}

bool isCharDev(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISCHR(statbuf.st_mode) == 1 ? true : false;
}

bool isBlockDev(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISBLK(statbuf.st_mode) == 1 ? true : false;
}

bool isFifo(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISFIFO(statbuf.st_mode) == 1 ? true : false;
}

bool isSymlink(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISLNK(statbuf.st_mode) == 1 ? true : false;
}

bool isSocket(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISSOCK(statbuf.st_mode) == 1 ? true : false;
}

bool pathExists(const char *path) {
    struct stat buffer;
    return stat(path, &buffer) == 0 ? true : false;
}

bool getCwd(char *pathBuff) {
    bool found = true;
    if (getcwd(pathBuff, PATH_BUF) == NULL) {
        if (errno == ERANGE) {
            printf("[ERROR] pathname length exceeds the buffer size\n");
        } else {
            perror("getcwd");
        }
        found = false;
    }
    printf("Current working directory: %s\n", pathBuff);
    return found;
}