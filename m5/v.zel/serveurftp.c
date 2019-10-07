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
#include <poll.h>

#define MAX_CLIENTS 10

void upper_string(char s[]);

/***********************************************************/
int main(int argc, char const *argv[])
{

  int sock, sock_client, len, nfds = 1, rc; //nfds = number of file descriptor that are ready (event based)
  struct sockaddr_in servaddr, cliaddr;
  struct pollfd pollfds[MAX_CLIENTS];

  char buffer[100];

  /***********************************************************/
  // create sock
  /***********************************************************/

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket created");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(stdout, "socket created\n");
  }

  /***********************************************************/
  // intializing polls, server and client adresses
  /***********************************************************/

  memset(pollfds, 0, MAX_CLIENTS * sizeof(struct pollfd)); //for Event Based server
  memset(&servaddr, 0, sizeof(servaddr));                  //addresse de la mémoire rempli de 0, quelle adresse quelle port serveur ecoute
  memset(&cliaddr, 0, sizeof(cliaddr));                    // pour recvfrom quel client il reçoit msg

  //Filling server information
  servaddr.sin_family = AF_INET;   // IPv4
  servaddr.sin_port = htons(8080); // port aléatoire > 1024
  //servaddr.sin_addr.s_addr = "127.0.0.1";
  //inet_aton("127.0.0.1", & servaddr.sin_addr);
  servaddr.sin_addr.s_addr = INADDR_ANY; // cas serveur

  //Initialise pollfd for event base server
  pollfds[0].fd = sock;
  pollfds[0].events = POLLIN;

  /***********************************************************/
  // Listen and Bind
  /***********************************************************/

  //sockaddr caster pour faciliter la vérifie
  if (bind(sock, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  else
    fprintf(stdout, "binded successfull\n");

  if (listen(sock, SOMAXCONN) < 0)
  {
    perror("error listening");
    close(sock);
  }
  else
    fprintf(stdout, "listening\n");

  /***********************************************************/
  // call poll()
  /***********************************************************/

  //Event based
  while (1)
  {
    rc = poll(pollfds, nfds, -1);
    if (rc < -1)
    {
      perror("No descriptors ready for I/O");
      exit(EXIT_FAILURE);
    }
    int i;
    for (i = 0; i < nfds; i++)
    {
      if (pollfds[i].revents == POLLIN) //Il s'est passé quelque chose
      {
        if (pollfds[i].fd == sock) //On est dans la sock d'écoute
        {
          len = sizeof(cliaddr);
          sock_client = accept(sock, (struct sockaddr *)&cliaddr, (socklen_t *)&len); // crée une nouvelle socket du client
          if (sock_client < 0)
          {
            perror("failed to accept connection");
            close(sock);
            exit(EXIT_FAILURE);
          }
          else {
            fprintf(stdout, "accepted\n");
            pollfds[nfds].fd = sock_client;
            pollfds[nfds].events = POLLIN;
            nfds++;
          }
        }
        else //On est dans un message de reception
        {
          //receive buffer and put it into uppercase
          if (recv(pollfds[i].fd, buffer, sizeof(buffer) * 8, 0) < 0)
          {
            perror("failed to recv message");
          }
          else
          {
            fprintf(stdout, "received: %s\n", buffer);
            upper_string(buffer); //On le met en capital
            if (send(pollfds[i].fd, (const char *)buffer, sizeof(buffer) * 8, 0) < 0)
            {
              perror("failed to send upper case string");
            }
            else
            {
              fprintf(stdout, "replied\n");
            }
          }
        }
      }
    }
  }

  close(sock);

  return 0;
}
/***********************************************************/

/***********************************************************/
// Functions
/***********************************************************/

void upper_string(char s[])
{
  int c = 0;

  while (s[c] != '\0')
  {
    if (s[c] >= 'a' && s[c] <= 'z')
    {
      s[c] = s[c] - 32;
    }
    c++;
  }
}
