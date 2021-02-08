#include <stdio.h>

int main( int argc, char *argv[], char **envp )
  {
    printf("Command line parameters:\n");

    // Проходимся по массиву параметров командной строки.
    for(int i = 0; i < argc; i++){
	    printf( "Parameter argv[ %d ] = %s \n", i, argv[i] );
        // Выводим элемент с индексом i.
    }    
    
    int counter = 0; // Здесь храним индекс элемента массива переменных среды.
    printf("Environment variables:\n");
    
    // В цикле увеличиваем значение указателя на элемент массива.
    for(char **envir = envp; *envir != 0; envir++){
        char *iEnv = *envir;
    	printf( "Environment variable envp[ %d ] = %s \n", counter, iEnv );
        counter++;
    }
    
    return 0;
  }
