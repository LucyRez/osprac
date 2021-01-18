#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main( void )
  {
    printf( "My user id is: %d\n", getuid() );
    printf( "My group id is: %d\n", getgid() );
    return 0;
  }
