
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int main()
{
  int fd1[2]; // Первый pipe, чтобы отправить данные от родителя к ребёнку.
  int fd2[2]; // Второй pipe, чтобы отправить данные к родителю от ребёнка.
  int size; // Длина первой строки отправленной от родителя.
  int result;
  
  // Выводим сообщения об ошибках если не смогли создать какой-либо pipe.
  if (pipe(fd1)<0)
  {
    printf("Couldn't create first pipe.\n" );
    exit(-1);
  }
  if (pipe(fd2)<0)
  {
    printf("Couldn't create second pipe.\n");
    exit(-1);
  }

  result = fork(); // Создаём процесс ребёнка.
  
  // Выводим сообщение об ошибке если не удалось создать.
  if (result < 0)
  {
    printf("Child could not be created.\n");
    exit(-1);
  }
  else if (result > 0)
  {
    // Родительский процесс

    char stringFromChild[14];
    
    // Родитель не смог закрыть свой pipe на чтение.
    if (close(fd1[0]) < 0) {
      printf("Parent could not close reading side of pipe.\n"); 
      exit(-1);
    }
    
    // Записываем строку в первый pipe.
    size = write(fd1[1], "Hello, child!", 14);
    
    // Если не удалось записать всю строку.
    if (size != 14) {
      printf("Parent : Can\'t write all string to pipe\n");
      exit(-1);
    }

    // Родитель не смог закрыть свой pipe на запись.
    if (close(fd1[1]) < 0) {
      printf("Parent could not close reading side of pipe.\n");
      exit(-1);
    }

    // Ждём ребёнка.
    wait(NULL);
    
    // Читаем данные из pipe от ребёнка.
    size = read(fd2[0], stringFromChild, 100);
 
    // Выводим сообщения об ошибках если родитель не смог прочитать всю строку,
    // или если pipe ребёнка не смогли закрыться.
    if (size != 14) {
      printf("Parent: Can\'t read all string from pipe\n");
      exit(-1);
    }

    printf("Parent has read string: %s\n", stringFromChild);
    
    if (close(fd2[0]) < 0) {
      printf("Parent could not close reading side of child pipe.\n");
      exit(-1);
    }

    if (close(fd2[1]) < 0) {
      printf("Could not close writing side of child pipe.\n");
      exit(-1);
    }

    printf("Exit parent process\n");
    exit(0);
  }
  else
  {
    // Закрываем pipe родителя на запись.
    if (close(fd1[1]) < 0) {
      printf("Child could not close writing side of pipe.\n"); 
      exit(-1);
    } 

    // Читаем строку полученную от родителя.
    char stringFromParent[14];
    size = read(fd1[0], stringFromParent, 14);
    if (size != 14) {
      printf("Child: Can\'t read all string from pipe\n");
      exit(-1);
    }
    printf("String from parent: %s\n", stringFromParent);

    char childString[] = "Hello, parent!";
   
    // закрываем оба конца на чтение или выводим сообщения об ошибках.
     if (close(fd2[0]) < 0) {
      printf("Сhild could not close reading side of child pipe.\n");
      exit(-1);
    }

    if (close(fd1[0]) < 0) {
      printf("Child could not close reading side of parent pipe.\n");
      exit(-1);
    }

    // Записываем строку во второй pipe.
    size = write(fd2[1], childString, 14);
    
    // Если не удалось записать всю строку.
    if (size != 14) {
      printf("Child : Couldn't write all string to second pipe\n");
      exit(-1);
    }

    // Закрываем pipe ребёнка на запись.
    if (close(fd2[1]) < 0) {
      printf("Child could not close writing side of pipe.\n"); 
      exit(-1);
    } 
    exit(0);
  }
}
