#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
  int     *text;    // Указатель на разделяему память.
  int     shmid;     // IPC дескриптор для разделяемой памяти.
  char    pathname[] = "lab7_1.c"; // имя файла, использованное для генерации ключа в первой программе.

  key_t   key;       // IPC ключ.

  // Генерируем ключ для заданного пути в текущей директории.
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  // Если разделяемая память уже существует.
  if ((shmid = shmget(key,  7000*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
  }
 
  // Пытаемся мапировать разделяемую память к адресному пространству текущего процесса.
  if ((text= (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  int index = 0;
  int fd = open("lab7_1.c", O_RDONLY);
  int val;
  // Читаем, пока не дойдём до конца файла.
  while(text[index]!=EOF){
	putchar(text[index]);
	index++;
  }
 
  // Освобождаем разделяемую память.
  if (shmctl(shmid,IPC_RMID, NULL) < 0) {
    printf("Can't free shared memory\n");
    exit(-1);
  }

  return 0;
}
