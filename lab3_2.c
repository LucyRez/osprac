#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  pid_t pid, ppid, val;
  int a = 0; // Будем изменять эту переменную в разных процессах.

  val = fork(); // Создаём новый процесс - ребёнка текущего процесса.
  
  // Проверяем вернувшееся значение.
  if(val == -1){
     // Если что-то пошло не так выводим лог в консоль.
     printf("Something went wrong and new process wasn't created.");
  }else if(val == 0){
       a = a + 10; // В этом процессе увеличиваем на 10.
       pid = getpid();
       ppid = getppid();
       printf("Child's pid = %d, child's ppid = %d, result = %d\n", (int)pid, (int)ppid, a);
       exit(1); // Дочерний процесс завершается.
  }else{
       int childStatus;
       pid = getpid();
       ppid = getppid();
       a = a + 50;
       printf("Parent's pid = %d, parent's ppid = %d, result = %d\n", (int)pid, (int)ppid, a);
       waitpid(val,&childStatus,0); // Родительский процесс ждёт завершения дочернего.
       // Без этого родительский процесс удалится и код родителя для дочернего поменяется + вывод в консоль кривой. 
       // (Просто захотелось сделать красивее)    
  }

  return 0;
}

