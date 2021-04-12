#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int   semid; // IPC дескриптор для массива семафоров.
  char pathname[]="lab9_1_1.c"; // Имя файла из текущей директории для генерации ключа.
  key_t key; // IPC ключ.
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
  mybuf.sem_op  = 1; // Добавляем 1 к текущему значению в семафоре.
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t add 1 to semaphore\n");
    exit(-1);
  }

  printf("The condition is present\n");
  return 0;
}
