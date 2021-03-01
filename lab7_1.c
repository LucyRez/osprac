#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int     *text;    // Указатель на разделяемую память.
  int     shmid;     // IPC дескриптор для разделяемой памяти.
  char    pathname[] = "lab7_1.c"; // Имя файла, используемое для генерации ключа.
                                 
  key_t   key;       // IPC ключ.
  
  // Пытаемся сгенерировать ключ на основании файла из текущей директории.
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n"); // В случае ошибки выводим сообщение и выходим.
    exit(-1);
  }

  //
  // Пытаемся создать разделяемую память для ключа.
  // Выделяем размер памяти равный 7000 интам
  //
  if ((shmid = shmget(key, 7000*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    
    // Обрабатываем 2 варианта ошибки 
    // (Если сегмент разделяемой памяти уже существует или же по другой причине)
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
 
      // Пытаемся получить IPC дескриптор, если разделяемая память уже существует.
      if ((shmid = shmget(key,  7000*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
    }
  }

  // Пытаемся мапировать разделяемую память к адресному пространству текущего процесса.
  if ((text= (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n"); // Выводим сообщение в случае ошибки.
    exit(-1);
  }

  int index = 0; // Индекс элемента.
  int fd = open("lab7_1.c", O_RDONLY); // Открываем файловый дескриптор.
  int val;

  // Читаем пока не дойдём до конца файла
  while((val = read(fd, &text[index],1))>0){
	index++;
  }
  text[index+1] = EOF; // Указываем, что это конец файла.
	
  // Если произошла ошибка при чтении, выводим сообщение.
  if(val<0){
	printf("Can't read file\n");
        exit(-1);
  }


  if (shmdt(text) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
