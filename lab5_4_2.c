#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd; // Файловый дескриптор.

  size_t  size; // Длина строки.
  char    resstring[14]; // Строка, получаемая в результате.
  char    name[] = "newFile.fifo"; // Названия файла fifo.

// Если не можем открыть файл, выводим сообщение об ошибке.
if ((fd = open(name, O_RDONLY)) < 0) {
      printf("Can\'t open FIFO for reading\n");
      exit(-1);
    }

    size = read(fd, resstring, 14); // Читаем строку из адресного пространства.

    // Если длина строки меньше ожидаемой, выводим сообщение об ошибке.
    if (size < 0) {
      printf("Can\'t read string from FIFO\n");
      exit(-1);
    }

    printf("Second program exit, string from fifo: %s\n", resstring);

    
    if (close(fd) < 0) {
      printf("child: Can\'t close FIFO\n"); 
      exit(-1);
    }
    // Работа первой программы одновременно завершается.

}
