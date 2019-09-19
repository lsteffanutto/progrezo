#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>

int main(int argc, char const *argv[]) {
  struct if_nameindex *interf_first, *intf; //2 addresses

  interf_first = if_nameindex(); // te te fais un malloc et stock de dans le premier element
  if( interf_first != NULL){

    for( intf = interf_first; intf->if_index != 0 || intf->if_name != NULL; intf++){
      //++ d'une adresse d'un struct == je passe à la struct suivante /la valeur/objet suivante dans la mémoire
      printf("%s\n", intf->if_name);

    }

    if_freenameindex(interf_first); // free après le malloc après toutes les opérations

  }

  return 0;
}
