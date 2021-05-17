#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LAST_MESSAGE 255 // Тип сообщения, сигнализирующий второй программе о завершении работы.

int main(void)
{
  int msqid;  // IPC дескриптор очереди.
  char pathname[]="lab11_3_1.c";  // Имя файла для генерации ключа.
  key_t  key;  // IPC ключ.
  int len, maxlen; // Переменная для счётчика и длина информативной части сообщения.

  struct mymsgbuf // Переделанная структура для сообщения.
  {
      long mtype;
      pid_t pid; 
      float value;
   
  } mybuf;

   // Пробуем сгенерировать ключ.
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  // Пытаемся получить доступ к очереди по ключу.
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  while (1) {
    
    // Макстимальный размер сообщения.
    maxlen = 200;

    // Получаем сообщение типа 1.
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
   
    // Если сообщение сигнального типа, закрываем всё и выходим.
    if (mybuf.value == LAST_MESSAGE) {
      msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
      exit(0);
    }

    printf("server received : message with pid = %ld, value = %f \n", mybuf.pid, mybuf.value);

    /* ОТПРАВЛЯЕМ СООБЩЕНИЕ */
    
    // Заполняем структуру сообщения.
    mybuf.mtype = mybuf.pid; 
    mybuf.pid = mybuf.pid;
    mybuf.value = mybuf.value*mybuf.value;
 
    // Длину информативной части получаем вычитая размер типа для хранения id из общего размера структуры.
    len = sizeof(struct mymsgbuf) - sizeof(long) ;
   
    // Посылаем сообщение типа pid.
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
    printf("server sent : message with pid = %ld, value = %f \n", mybuf.pid, mybuf.value);
  }

  return 0;
}
