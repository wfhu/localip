#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

int
getlocalip (char **outip)
{
  int i = 0;
  int sockfd;
  struct ifconf ifconf;
  char buf[512];
  struct ifreq *ifreq;
  char *ip;
  ifconf.ifc_len = 512;
  ifconf.ifc_buf = buf;

  if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      return -1;
    }
  ioctl (sockfd, SIOCGIFCONF, &ifconf);
  close (sockfd);
  ifreq = (struct ifreq *) buf;


  int if_num = (ifconf.ifc_len / sizeof (struct ifreq));
  int j;
  char *temp_ip;
  for (j=0;j<if_num;j++)
  {
    temp_ip=(char *)malloc(20 * sizeof(char));
    outip[j]=temp_ip;
    printf("%dth outip is %ld\n", j, outip[j]);
  }
  printf("the number of Interface is %d\n", j);


//  for (i = (ifconf.ifc_len / sizeof (struct ifreq)); i > 0; i--)
  for (i = 0; i < if_num; i++)
    {
      printf("number is %d\n", i);
      ip = inet_ntoa (((struct sockaddr_in *) &(ifreq->ifr_addr))->sin_addr);

/*      if (strcmp (ip, "127.0.0.1") == 0)
	{
	  ifreq++;
	  continue;
	}
*/
//      printf("ifreq is %ld, size is %d\n", ifreq, sizeof(ifreq));
      strcpy (outip[i], ip);
	  ifreq++;
      continue;
//      return 0;
    }

//  return -1;
  return i;
}

int
main ()
{
  int max_if_num = 20;
  char *ip[max_if_num];

  int i = getlocalip (ip);
  if (i > 0)
  {
      int k;
      for(k=0; k<i && k < max_if_num; k++)
      {
        printf ("本机#%d IP地址是： %s\n", k,ip[k]);
      }
  }
  else
    printf (" 无法获取本机IP地址 ");
}
