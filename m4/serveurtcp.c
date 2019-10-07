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
#include <poll.h>
#include <errno.h>

#define MAX_CLIENTS 1024

// Client questionne serveur lancé avec netcast, serveur lancé sur un terminal à part puis
// éxécute ce programme pour lui envoyer un message

//***** SERVEUR EVENEMENTIEL avec poll **************
// https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_71/rzab6/poll.htm

int main(int argc, char const *argv[]) {

  // défini les var

  int sock;
  struct sockaddr_in servaddr, cliaddr;
  int end_server, dedisock, close_conn, lenrc, k, l, m;

  //Oninitialise poll()

  //char* hello = "Hey ! What's up serveur ?";
  char* hello1 = "Salut c'est le serveur, j'ai bien reçu ton msg, voila ta réponse ;) \n";
  char* buffer = "Wesh le iencli";

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

  struct pollfd pollfds[MAX_CLIENTS];
  memset(pollfds, 0, MAX_CLIENTS * sizeof(struct pollfd) );

  pollfds[0].fd = sock;
  pollfds[0].events = POLLIN;

  int timeout = -1;
  int nfds = 1;
  int compress_array = 0;

  // boucle infini pour voir si on a eu des évenements
  // si se passe un truc faut ajouter le descripteur pour l'écouter avec accepter
  // sinon faut rcv le msg

  while(1){

    printf("Waiting on poll()...\n");
    int rc = poll(pollfds, nfds, timeout);

    //contrôle d'erreur sur le poll
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }

    //On regarde les demandes de connexions pour voir laquelle est active

    int size = nfds;
    int j = 0;

    for (j = 0; j < size; j++)
    {
      if(pollfds[j].revents == 0)
      continue;

      //if revents is not POLLIN
      if(pollfds[j].revents != POLLIN)
      {
        printf("  Error! revents = %d\n", pollfds[j].revents);
        int end_server = 1;
        break;

      }

      //Si par contre le descripteur est en activité:

      if(pollfds[j].fd == sock)
      {
        printf(" Listening socket is readable\n");

        // et là on va accepter toutes les connexions qui attendent
        // avant de refaire un poll sa mère

        do {
          dedisock = accept( sock, NULL, NULL);
          if ( dedisock < 0 )
          {
            if(errno != EWOULDBLOCK)
            {
              perror( " accept() failed");
              end_server = 1;
            }
          break;
          }

          // on ajoute la nouvelle connection à la structure pollfds
          printf("  New incoming connection - %d\n", dedisock);
          pollfds[nfds].fd = dedisock;
          pollfds[nfds].events = POLLIN;
          nfds++;

          // Puis on peut accepter de nouvelles connexions

        } while( dedisock != -1);

      }

      else
      {
        printf("  Descriptor %d is readable\n", pollfds[j].fd);
        close_conn = 0;

        //On veut recevoir les datas de cette socket avant de relancer poll
        do {

          rc = recv(pollfds[j].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = 1;
            }
            break;
          }

          if (rc == 0)
          {
            printf("Connection closed\n");
            close_conn = 1;
            break;
          }

          lenrc = rc;
          printf("%d bytes received\n", lenrc );

          rc = send(pollfds[j].fd, buffer, lenrc, 0);

          if (rc < 0 )
          {
            perror(" send() failed");
            close_conn = 1;
            break;
          }

        } while(1);

        if (close_conn)
        {
          close(pollfds[j].fd);
          pollfds[j].fd = -1;
          compress_array = 1;
        }

      //fin else
      }




    // fin for
    }

    if (compress_array)
    {
      compress_array = 0;
      for (k = 0; k < nfds; k++)
      {
        if (pollfds[k].fd == -1)
        {
          for(l = k; l < nfds; l++)
          {
            pollfds[l].fd = pollfds[l+1].fd;
          }
          k--;
          nfds--;
        }
      }
    }

    for (m = 0; m < nfds; m++)
    {
      if(pollfds[m].fd >= 0)
        close(pollfds[m].fd);
    }



  }


  // int n, len=100;
  // //***** ACCETPT + SERVEUR DEDIE
  //
  //
  // // On crée un buffer pour stocker le message
  // char bufferstock[100];
  // int messagerecu = recv(dedisock,bufferstock, sizeof(bufferstock),0);
  // //***** RECVFROM = UDP FDP
  //
  // printf("Le message reçu du client est:\n)");
  // printf("%s\n", bufferstock);
  // int i=0;
  //
  // // for(i=0; i<strlen(bufferstock); i++){
  // //   bufferstock[i] = toupper(bufferstock[i]);
  // // }
  // send(dedisock, (const char*) hello1, strlen(hello1), 0);
  // send(dedisock, (const char*) bufferstock, strlen(bufferstock), 0);





  //strupr = prend char* et la converti en majuscule



  close(sock);

  return 0;
}
