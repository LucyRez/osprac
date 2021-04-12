#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  int     *array;
  int     shmid;
  int     new = 1;
  char    pathname[] = "lab9_2_1.c"; // Имя файла из текущей директории для генерации ключа.
  key_t   key; // IPC ключ.
  long    i;

  int    semid;        // IPC дескриптор для массива семафоров.
  struct sembuf mybuf; // Структура, описывающая операции семафора.

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  // Пытаемся найти разделяемую память по ключу.
  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  // Получаем массив из памяти.
  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  // Пытаемся получить доступ к массиву семафоров по ключу, если его не существует, он будет создан.
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  
  mybuf.sem_num = 0;
  mybuf.sem_op  = -1; // Операция для вычитания 1 из текущего значения.
  mybuf.sem_flg = 0;

  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;
  } else {
    // Ожидание будет происходить, пока значение в семафоре не станет >= 1.
    if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
    }

    array[1] += 1;
    array[2] += 1;
  }

  printf
    ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
    array[0], array[1], array[2]);

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
