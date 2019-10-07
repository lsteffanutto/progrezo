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

// Client questionne serveur lancé avec netcast, serveur lancé sur un terminal à part puis
// éxécute ce programme pour lui envoyer un message

int main(int argc, char const *argv[]) {

  // défini les var
  int sock;
  struct sockaddr_in servaddr;

  char* hello = "Hey ! What's up serveur ?\n";

  // initialise socket et vérifie pas d'erreur
  if ( (sock = socket(AF_INET, SOCK_STREAM,0)) < 0 ) {
    perror("socket()"); //printf de ce qui c'est pas bien passé
    exit(EXIT_FAILURE);
  }


  memset(&servaddr, 0, sizeof(servaddr)); //addresse de la mémoire rempli de 0

  //Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080); // port aléatoire > 1024
  //servaddr.sin_addr.s_addr = "127.0.0.1";
  inet_aton("127.0.0.1", & servaddr.sin_addr);

  connect(sock, (const struct sockaddr*) &servaddr, sizeof(servaddr) );

  send(sock, (const char*) hello, strlen(hello), 0);

  //Ne pas oublier receive pour recevoir le msg retour
  char bufferstock[100];
  int n, len=100;
  memset(&bufferstock, '\0', len);
  int messagerecu = recv(sock,bufferstock, sizeof(bufferstock),0);

  //printf("Hello message sent to serveur.\n)");

  // PUIS PRINT LE MSG DE RETOUR KON A STOCKE FDP
  printf("%s\n",bufferstock);

  close(sock);

  return 0;
}
