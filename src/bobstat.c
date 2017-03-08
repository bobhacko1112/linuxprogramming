// basically source I got from the man pages.
// http://man7.org/linux/man-pages/man2/stat.2.html
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "../include/boberr.h"
#define buffer_size 4096 //buffer size for read/write functions.

int main(int argc, char *argv[]) {
    struct stat sb;

    if (argc != 2) {
        printf((char*)"stupid input");
    }

    if (stat(argv[1], &sb) == -1) {
        handleError();
    }

    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
        printf("block device\n");
        break;

    case S_IFCHR:
        printf("character device\n");
        break;

    case S_IFDIR:
        printf("directory\n");
        break;

    case S_IFIFO:
        printf("FIFO/pipe\n");
        break;

    case S_IFLNK:
        printf("symlink\n");
        break;

    case S_IFREG:
        printf("regular file\n");
        break;

    case S_IFSOCK:
        printf("socket\n");
        break;

    default:
        printf("unknown?\n");
        break;
    }

    printf("I-node number:            %ld\n", (long) sb.st_ino);
    printf("Mode:                     %lo (octal)\n",
           (unsigned long) sb.st_mode);
    printf("Link count:               %ld\n", (long) sb.st_nlink);
    printf("Ownership:                UID=%ld   GID=%ld\n",
           (long) sb.st_uid, (long) sb.st_gid);
    printf("Preferred I/O block size: %ld bytes\n",
           (long) sb.st_blksize);
    printf("File size:                %lld bytes\n",
           (long long) sb.st_size);
    printf("Blocks allocated:         %lld\n",
           (long long) sb.st_blocks);
    printf("Last status change:       %s", ctime(&sb.st_ctime));
    printf("Last file access:         %s", ctime(&sb.st_atime));
    printf("Last file modification:   %s", ctime(&sb.st_mtime));
    _exit(EXIT_SUCCESS);
}
