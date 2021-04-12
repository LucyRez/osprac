#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int    semid;        // IPC дескриптор для массива семафоров.
  char   pathname[]="lab9_1_1.c"; // Имя файла из текущей директории для генерации ключа.
                              
  key_t  key;          // IPC ключ.
  struct sembuf mybuf; // Структура, описывающая операции семафора.

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  
  // Пытаемся получить доступ к массиву семафоров по ключу, если его не существует, он будет создан.
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  
  mybuf.sem_num = 0;
  mybuf.sem_op  = -5; // Семафор будет вычитать 5 от текущего значения.
  mybuf.sem_flg = 0;

  // Ожидание будет происходить до тех пор, пока программа не сможет вычесть 5
  // т.е. до тех пор, пока значение в семафоре не будет >= 5.
  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }

  printf("The condition is present\n");
  return 0;
}
