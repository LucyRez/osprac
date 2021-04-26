 #include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Тип сообщения, сигнализирующий второй программе о завершении работы.

int main(void)
{
  int msqid;      // IPC дескриптор очереди.
  char pathname[]="lab11_1_1.c"; // Имя файла для генерации ключа.
                             
  key_t  key;     // IPC ключ.
  int i,len;      // Переменная для счётчика и длина информативной части сообщения.

  struct mymsgbuf // Переделанная структура для сообщения.
  {
    long mtype;
      short first; // Можно так же добавить вложенную структуру.
      float second;
   
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
    
    // Заполняем структуру сообщения.
    mybuf.mtype = i; // Каждое сообщение будет разного типа.
    mybuf.first = 1;
    mybuf.second = 2;
 
    // Длину информативной части получаем вычитая размер типа для хранения id из общего размера структуры.
    len = sizeof(struct mymsgbuf) - sizeof(long) ;
   
    // Посылаем сообщение.
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

  }


  mybuf.mtype = LAST_MESSAGE; // Указываем, что сообщение последнее.
  len         = 0; // Размер 0, так как нам важен сам факт получения сообщения.

  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  return 0;
}


