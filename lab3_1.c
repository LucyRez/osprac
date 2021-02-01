#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;

  //fork(); // Закомменчено, так как нам нужен только 1 процесс.

  // Получаем id  текущего процесса и его родителя.
  pid  = getpid(); 
  ppid = getppid();

  printf("My pid = %d, my ppid = %d \n", (int)pid, (int)ppid);

  return 0;
}
