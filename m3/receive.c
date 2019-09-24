/** Receiver/client multicast Datagram example. **/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 1024

int main(int argc, char *argv[])
{

	/* Create useful var and structures */
	struct sockaddr_in localSock;
	struct ip_mreq group;
	int sd;
	int datalen;
	char databuf[BUFLEN];

	/* Create a datagram socket on which to receive. */
	/* TEST return value !!! */
	// socket()

	if ( (sock = socket(AF_INET, SOCK_DGRAM,0)) < 0 ) {
	    perror("Opening datagram socket error"); //printf de ce qui c'est pas bien passé
			close(sd);
			exit(EXIT_FAILURE);
	  }

		memset(&servaddr, 0, sizeof(servaddr));
		memset(&cliaddr, 0, sizeof(cliaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(4321);
		servaddr.sin_addr.s_addr = INADDR_ANY;

		group.imr_multiaddr.s_addr = inet_addr("226.1.111.255");
		group.imr_interface.s_addr = inet_addr("127.0.0.1");
	/* Enable SO_REUSEADDR to allow multiple instances of this */
	/* application to receive copies of the multicast datagrams. */
	{
		int reuse = 1;

		// ajoute une socket MEMBERSHIP
		if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
		{
			perror("Adding multicast group error");
			close(sd);
			exit(EXIT_FAILURE);
		}else{
		}
			printf("Adding multicast group...OK.\n");
		}


	/* Init and fill localSock */
	/* Bind to the proper port number with the IP address */
	/* specified as INADDR_ANY. */
	/* TEST return value ! */



	/* Join the multicast group 226.1.1.1 on the local 127.0.0.1 */
	/* interface. Note that this IP_ADD_MEMBERSHIP option must be */
	/* called for each local interface over which the multicast */
	/* datagrams are to be received. */
	/* struct ip_mreq */

	/* Read from the socket. */
	datalen = sizeof(databuf);
	if( read(sd,databuf, datalen)<0) {
		perror("Reading datagram message erro");
		close(sd);
		exit(EXIT_FAILURE);
	}else {
		printf("Reading datagram message... OK \n");
		printf("The msg from multicast server is:\%s\ \n",databuf);
	}
	// read()

	return 0;
}
