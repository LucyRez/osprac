#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;  // Глобальная переменная, разделяемая всеми нитями исполнения.

/**
 Функция, которая будет ассоциироваться со 2 и 3 нитью исполнения.
**/
void *mythread(void *dummy)
{
  pthread_t mythid;

  mythid = pthread_self(); // Получаем идентификатор текущей нити исполнения.

  a = a+1;
  printf("Thread %u, Calculation result = %d\n", mythid, a);
  return NULL;
}

/**
  Функция main тоже ассоциирована с отдельной нитью исполнения.
**/
int main()
{
  pthread_t thid, mythid, secondthid; // Переменные для хранения всех идентификаторов.
  int       result;
  
  // Пробуем создать новый thread.
  result = pthread_create (&thid, (pthread_attr_t *)NULL, mythread, NULL);

  // В случае, если результат не равен 0, произошла ошибка.
  if (result != 0) {
    printf ("Error on thread create, return value = %d\n", result);
    exit(-1);
  }

  // Создаём 3-ий thread.
  result = pthread_create (&secondthid, (pthread_attr_t *)NULL, mythread, NULL);
 
  // Выводим соообщение в случае ошибки.
  if (result != 0) {
    printf ("Error on thread create, return value = %d\n", result);
    exit(-1);
  }

  // Выводим идентификаторы.
  printf("Thread created, thid = %u\n", thid);

  printf("Thread created, thid = %u\n", secondthid);

  mythid = pthread_self(); // Получаем идентификатор текущего thread-а.

  a = a+1;

  printf("Thread %u, Calculation result = %d\n", mythid, a);
 
  // Текущий (родительский) thread ожидает завершения всех дочерних thread-ов.
  pthread_join(thid, (void **)NULL);
  pthread_join(secondthid, (void **)NULL);

  return 0;
}
