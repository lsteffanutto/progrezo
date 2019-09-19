#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>

//print les interfaces réseaux machine courante, donc ifa_name

int main(int argc, char const *argv[]) {

  struct if_nameindex *if_nidxs, *intf; // Variables d'indexs

  struct ifaaddrs *ifaddr, *ifa; // Variables d'addresses
  int family, s;
  char host[NI_MAXHOST];



  if_nidxs = if_nameindex(); // fonction retourne pointeur vers liste chaîné des interface (obtenir nom et indice interface réseau) alloue memoire
  if ( if_nidxs !=NULL)
  {
    for(intf = if_nidxs; intf->if_index != 0 || intf->if_name !=NULL; intf++) //pointeur vers structure et on passe à l'adresse d'après, saut taille structure et on passe à la structure d'après)
    {
      printf("%s\n", intf->if_name);
      //printf("%s\n", intf->if_addr);







      if (ifa->ifaddr == NULL)    // on veut aussi afficher les noms
        continue;

        family = ifa->ifaddr->sa_family;

        /* Display interface name and family (including symbolic
          form of the latter for the common families) */

        printf("%s  address family: %d%s\n",
                ifa->ifa_name, family,
                (family == AF_PACKET) ? " (AF_PACKET)" :
                (family == AF_INET) ?   " (AF_INET)" :
                (family == AF_INET6) ?  " (AF_INET6)" : "");

         /* For an AF_INET* interface address, display the address */

         if (family == AF_INET || family == AF_INET6) {
             s = getnameinfo(ifa->ifaddr,
                     (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                           sizeof(struct sockaddr_in6),
                     host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
          }
          printf("\taddress: <%s>\n", host);




    }

    if_freenameindex(if_nidxs); // relibère mémoire de if_nameindex
  }

  return;
}
