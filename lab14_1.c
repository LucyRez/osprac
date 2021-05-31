#include <signal.h>

int main(void) {
  //
  // Игнорируем сигнал SIGINT
  //
  (void)signal(SIGINT, SIG_IGN);

  // Дополнительно игнорируем сигнал SIGQUIT при комбинации клавиш CTRL+4
  (void)signal(SIGQUIT, SIG_IGN);

  while(1);
  return 0;
}
