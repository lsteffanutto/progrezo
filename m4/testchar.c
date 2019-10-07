#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <ctype.h>

int main(int argc, char const *argv[]) {
  char bufferstock[100]="salut";
  printf("buffer 1 est:\n%s\n",bufferstock);

  int i=0;

  while(bufferstock[i] != "/0"){
    i++;

  }

  bufferstock[i]="ça va";

  printf("buffer 2 est:\n%s\n",bufferstock);




  return 0;
}
