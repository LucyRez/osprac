
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int     fd; // Файловый дескриптор.
  FILE *fp; // Указатель на файл.


  int counter = 0;
  char name[] = "b"; // Имя первого файла.

  //mkdir("temporaryFiles",0777);

   
   // Если не можем открыть файл, выводим сообщение об ошибке.
    if ((fp = fopen(name, "w")) == NULL) {
      printf("Can\'t open file\n");
      exit(-1);
    }else{
      fclose(fp);
    }

    char linkName[]= "b";

    // Цикл будет выполняться, пока не достигнем максимальной глубины рекурсии.
    while(!((fd = open(linkName, O_RDONLY | O_CREAT )) < 0)){
      counter++; // Увеличиваем счётчик.
      char result[50];
      sprintf(result, "%d", counter);
      char temp[] = "b";
      strcat(temp,result); // Добавляем значение счётчика к названию файла.
      
      // Создаём связь.
      if (symlink(linkName, temp) !=0){
         printf("Can\'t create link\n");
         unlink(linkName);
         close(fd);
         exit(-1);
      }
     
      linkName[0] = 0;
      strcat(linkName,temp);
      printf("%s \n", linkName);
      close(fd); // Закрываем файл.
    }

    char res[50];
    sprintf(res, "%d", counter);
    printf("Depth of recursion: %s \n", res);

}
