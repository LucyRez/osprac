#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  int  fd; // Переменная для значения дескриптора.
  int  size; // Переменная для хранения длины строки.
  char *str = (char *) calloc(100, sizeof(char));

  fd = open("myfile", O_RDONLY); // Открываем файл.
 
  // Если получаем ошибку, выводим сообщение.
  if (fd  < 0) {
    printf("Could not open file\n");
    exit(-1);
  }
  
  size = read(fd, str, 14); // Читаем строку в переменную.
  printf("Text from file: %s \n", str); // Печатаем полученную строку.

  // если не смогли прочитать все данные, выводим сообщение.
  if (size != 14) {
      printf("Could not read all string from file.\n");
      exit(-1);
    }

  // Если не смогли закрыть файл, выводим сообщение.
  if (close(fd) < 0) {
    printf("Could not close file\n");
  }

  return 0;
}
