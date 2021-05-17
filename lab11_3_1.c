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
  int msqid;      // IPC дескриптор очереди.
  char pathname[]="lab11_3_1.c"; // Имя файла для генерации ключа.
                             
  key_t  key;     // IPC ключ.
  int i,len;      // Переменная для счётчика и длина информативной части сообщения.

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
 
  // Пытаемся получить доступ к очереди по ключу, если её нет, то она будет создана.
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

 
  // Посылаем в очередь 5 сообщений.
  for (i = 1; i <= 5; i++) {

    /* ОТПРАВЛЯЕМ СООБЩЕНИЕ */
    
    // Заполняем структуру сообщения.

    printf("Write a float number: \n");
    float val;
    scanf("%f", &val);
    mybuf.mtype = 1; 
    mybuf.pid = getpid();
    mybuf.value = val;
 
    // Длину информативной части получаем вычитая размер типа для хранения id из общего размера структуры.
    len = sizeof(struct mymsgbuf) - sizeof(long) ;
   
    // Посылаем сообщение типа 1.
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
    
    /* ЧИТАЕМ СООБЩЕНИЕ */

    // Максимальный размер прочитанного сообщения.
    int maxlen = 200;

    // Получаем сообщение типа pid.
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, getpid(), 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

     printf("Client received : message type = %ld, square = %f \n", mybuf.mtype, mybuf.value);

  }


  mybuf.mtype = 1; 
  mybuf.pid = getpid();
  mybuf.value = LAST_MESSAGE; // Указываем, что сообщение последнее.
  len         = sizeof(struct mymsgbuf) - sizeof(long);

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  return 0;
}


