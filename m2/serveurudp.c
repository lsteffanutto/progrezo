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

// Client questionne serveur lancé avec netcast, serveur lancé sur un terminal à part puis
// éxécute ce programme pour lui envoyer un message

int main(int argc, char const *argv[]) {

  // défini les var

  int sock;
  struct sockaddr_in servaddr, cliaddr;

  //char* hello = "Hey ! What's up serveur ?";
  char* hello1 = "Tqt bae I received your msg";

  // initialise socket et vérifie pas d'erreur

  if ( (sock = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
    perror("socket()"); //printf de ce qui c'est pas bien passé
    exit(EXIT_FAILURE);
  }


  memset(&servaddr, 0, sizeof(servaddr)); //addresse de la mémoire rempli de 0, quelle adresse quelle port serveur ecoute
  memset(&cliaddr, 0, sizeof(cliaddr)); // pour recvfrom quel client il reçoit msg

  //Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_port = htons(8080); // port aléatoire > 1024
  //servaddr.sin_addr.s_addr = "127.0.0.1";
  //inet_aton("127.0.0.1", & servaddr.sin_addr);
  servaddr.sin_addr.s_addr = INADDR_ANY; // cas serveur

  //sockaddr caster pour faciliter la vérifie
  if ( bind(sock,(const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  int n, len=100;

  // if ( listen(sock, SOMAXCONN) == -1) {
  //   perror("listen");
  //   exit(EXIT_FAILURE);
  // }

  // On crée un buffer pour stocker le message
  char bufferstock[100];
  int messagerecu = recvfrom(sock,bufferstock, sizeof(bufferstock),0, (struct sockaddr*)&cliaddr, (socklen_t*)&len);

  sendto(sock, (const char*) hello1, strlen(hello1), 0, (const struct sockaddr*) &cliaddr, sizeof(servaddr));





  //strupr = prend char* et la converti en majuscule

  printf("Hello message receive.\n)");


  close(sock);

  return 0;
}
