/* Send Multicast Datagram code example. */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 1024

// SEND RECEIVE car pas de serveur
int main (int argc, char *argv[ ])
{

	/* Create useful var struc */
	struct in_addr localInterface; //***** INTERFACE DE TA MACHINE ****// Défini interface machine en multicast
	struct sockaddr_in groupSock; // **** struct avec adresse d'envoie ****//
	int sd;
	char databuf[BUFLEN] = "A multicast message example";
	int datalen = sizeof(databuf);

	/* Create a datagram socket */
	if ( (sock = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
	    perror("socket()"); //printf de ce qui c'est pas bien passé
	    exit(EXIT_FAILURE);
	  }

	/* TEST return value */
	//*** DEFINI ADRESSE D'ENVOIE ***//
  memset(&groupSock, 0, sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = intet_addr("226.1.11.69");
	groupSock.sin_port = htons(4321);
	/* Initialize the group sockaddr structure with a */

	//*** ON PARAMETRE L'INTERFACE EN MULTICAST ***//
	localInterface.s_addr = inet_addr("127.0.0.1");
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, (char*)&localInterface, sizeof(localInterface) <0){
		perro("setting local interface error");
		closed(sd);
		EXIT_FAILURE;
	} else {
		printf("Setting local interface... OK\n")
	}

	if (sendto(sd, databuf, datalen, 0, (struct sockaddr*) &groupSock, sizeof(groupSock)) < 0){
		perror("Sending datagram message error");
		close(sd);
		exit(EXIT_FAILURE);
	} else {
		printf("Sending datagral message ... OK\n", );
	}

	return EXIT_SUCCASS;
	/* group address of 226.1.1.X and port Y . */
	if ( bind(sock,(const struct sockaddr*)&localInterface, sizeof(localInterface)) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

	/* Set localInterface interface for outbound multicast datagrams with setsockopt and option IP_MULTICAST_IF */
	/* The IP address specified must be associated with a local address with inet_addr */
	/* multicast capable interface. */


	/* Send a message to the multicast group specified by the*/
	/* groupSock sockaddr structure. */
	// sendto()

	char bufferstock[100];
  int messagerecu = recvfrom(sock,bufferstock, sizeof(bufferstock),0, (struct sockaddr*)&groupSock, (socklen_t*)&datalen);

  sendto(sock, (const char*) databuf, strlen(databuf), 0, (const struct sockaddr*) &groupSock, sizeof(localInterface));





  //strupr = prend char* et la converti en majuscule

  printf("Hello message receive.\n)");


  close(sock);

	return 0;
}
