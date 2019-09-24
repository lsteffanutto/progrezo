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


//****** CREER SERVEUR DEDIE A UN CLIENT ****//
int main(int argc, char const *argv[]) {

  // défini les var

  int sock;
  struct sockaddr_in servaddr, cliaddr;

  //char* hello = "Hey ! What's up serveur ?";
  char* hello1 = "J'ai bien reçu ton msg, voila ta réponse \n";

  // initialise socket et vérifie pas d'erreur

  if ( (sock = socket(AF_INET, SOCK_STREAM,0)) < 0 ) {
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

  //***1st arg descripteur de socket et 2nd SOMAXCONNµµ//
  if ( listen(sock, SOMAXCONN) == -1) {
    perror("listen part en couille");
    exit(EXIT_FAILURE);
  }
  //*** getaddrinfo(), on veut @IP et #port, protocole, type socket et famille
  //*** getaddrinfo = DNS avec tous les renseignement
  //**** a utiliser pour contacter n'importe quelle machine
  //*** si plusieurs processus c seul fonction qui marche

  int n, len=100;
  int dedisock = accept( sock,(struct sockaddr*)&servaddr, (socklen_t*)&len);
  //***** ACCETPT + SERVEUR DEDIE


  // On crée un buffer pour stocker le message
  char bufferstock[100];
  int messagerecu = recv(dedisock,bufferstock, sizeof(bufferstock),0);
  //***** RECVFROM = UDP FDP

  printf("%s", bufferstock);
  int i=0;

  // for(i=0; i<strlen(bufferstock); i++){
  //   bufferstock[i] = toupper(bufferstock[i]);
  // }

  send(dedisock, (const char*) hello1, strlen(hello1), 0);
  send(dedisock, (const char*) bufferstock, strlen(bufferstock), 0);





  //strupr = prend char* et la converti en majuscule

  printf("Hello message receive.\n)");


  close(sock);

  return 0;
}
