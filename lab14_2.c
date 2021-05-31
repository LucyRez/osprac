#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {

  if(nsig == SIGINT){
     printf("Receive signal %d, CTRL-C pressed\n", nsig);
  }else if(nsig == SIGQUIT){
     printf("Receive signal %d, CTRL-4 pressed\n", nsig);
  }
  
}

int main(void) {
 
  // Обрабатываем сигнал SIGINT 
  (void)signal(SIGINT, my_handler);
 
  // Дополнительно обрабатываем сигнал SIGQUIT при комбинации клавиш CTRL+4
  (void)signal(SIGQUIT, my_handler);

  while(1);
  return 0;
}
