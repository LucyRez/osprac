#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid, ppid, val;

  val = fork(); // Создаём новый процесс - ребёнка текущего процесса.
  
  // Проверяем вернувшееся значение.
  if(val == -1){
    // Если что-то пошло не так выводим лог в консоль.
    printf("Something went wrong and new process wasn't created.");
    exit(1);
  }else if(val == 0){
    pid = getpid();
    ppid = getppid();
    printf("Child's pid = %d, child's ppid = %d \n", (int)pid, (int)ppid);
    printf("Child is making a new directory in current directory called 'newDirectory'\n");

    // В массиве хранятся все данные необходимы е для выполнения подпрограммы.
    char *args[] = {"/bin/mkdir", "mkdir", "newDirectory", NULL};

    // Проверяем существование директории, которую хотим создать.
    if(chdir(args[2]) == -1){
      // Запускаем на исполнение создание новой директории в текущей.
      if(execv(args[0], args+1) == -1){
        printf("Child couldn't make new directory");
      }
    }
    else{
	    printf("Directory 'newDirectory' already exists");
    }
    exit(0); // Дочерний процесс завершается.
  }else{
       int childStatus;
       pid = getpid();
       ppid = getppid();
       printf("Parent's pid = %d, parent's ppid = %d \n", (int)pid, (int)ppid);
       exit(0);
  }
     return 0;
  }
