#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char **argv)
{
    int fd; // Файловый дескриптор.

    // Если файл уже существует, его нужно будет удалить перед запуском.
    if (mknod("fifo", S_IFIFO | 0666, 0) < 0) {
    printf("Can\'t create FIFO, file allready exists\n");
    exit(-1);
    }   

    fd = open("fifo", O_RDWR); // Открываем файл и получаем дескриптор.
    printf("Size of pipe is: %d\n", fcntl(fd, F_GETPIPE_SZ)); // Выполняем операцию F_GETPIPE_SZ над дескриптором.

    return 0;
}
