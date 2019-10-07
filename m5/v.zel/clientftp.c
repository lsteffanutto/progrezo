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

int main(int argc, char const *argv[])
{

  // défini les var
  int sock;
  struct sockaddr_in servaddr;
  char buffer[100];
  int len_buffer;

  // initialise socket et vérifie pas d'erreur
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket()"); //printf de ce qui c'est pas bien passé
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr)); //addresse de la mémoire rempli de 0

  //Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(8080); // port aléatoire > 1024
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    perror("failed to connect to server");
    close(sock);
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    //memset(buffer, '\0', 100*sizeof(char));
    fprintf(stdout, "Quel est votre message?\n");
    scanf("%s", buffer);
    len_buffer = strlen(buffer);
    fprintf(stdout, "len_buffer = %i\n", len_buffer);
    if (send(sock, (const char *)buffer, len_buffer, 0) < 0)
    {
      perror("failed to send to server");
      close(sock);
      exit(EXIT_FAILURE);
    }
    else
      fprintf(stdout, "sent\n");
    

    //Ne pas oublier receive pour recevoir le msg retour
    char buffer_upper[100];
    memset(buffer_upper, '\0', sizeof(buffer_upper));
    if (recv(sock, buffer_upper, 100, 0) < 0)
    {
      perror("failed to receive upper text");
      close(sock);
      exit(EXIT_FAILURE);
    }
    else
    {
      fprintf(stdout, "receive new text: %s\n", buffer_upper);
    }
  
  }
  close(sock);

  return 0;
}
