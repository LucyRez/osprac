#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd; // Файловый дескриптор.

  size_t  size; // Размер строки.
  char    name[] = "newFile.fifo";

  // Если не удалось создать именнованный пайп, выводим сообщение об ошибке.
  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("Can\'t create FIFO, file allready exists\n");
    exit(-1);
  }

   // Если не можем открыть файл, выводим сообщение об ошибке.
    if ((fd = open(name, O_WRONLY)) < 0) {
      printf("Can\'t open FIFO for writting\n");
      exit(-1);
    }

    size = write(fd, "Hello, world!", 14); // Получаем размер записанной строки.

    // Если размер меньше ожидаемого, что-то пошло не так.
    if (size != 14) {
      printf("Can\'t write all string to FIFO\n");
      exit(-1);
    }

    // Пробуем закрыть
    if (close(fd) < 0) {
      printf("parent: Can\'t close FIFO\n");
      exit(-1);
    }

    printf("First program exit\n");


}
