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

typedef struct listNode {
    struct listNode *next;
    void *value;
} listNode;

typedef struct list {
    listNode *head;
    listNode *tail;
    unsigned int len;
} list;

list *listCreate(void)
{
    struct list *list;

    if ((list = malloc(sizeof(*list))) == NULL)
        return NULL;
    list->head = list->tail = NULL;
    list->len = 0;
    return list;
}

list *listAddNodeTail(list *list, void *value)
{
    listNode *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;
    node->value = value;
    if (list->len == 0) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->len++;
    return list;
}

void showList(list *list)
{
	listNode *node;
	node = list->head;
	int i;
	for(i=0;i<list->len;i++)
	{
		printf("%d th value is %s\n", i, node->value);
		node=node->next;
	}
}

int
getlocalip (list *list)
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


//  for (i = (ifconf.ifc_len / sizeof (struct ifreq)); i > 0; i--)
  for (i = 0; i < if_num; i++)
    {
//      printf("number is %d\n", i);
      ip = inet_ntoa (((struct sockaddr_in *) &(ifreq->ifr_addr))->sin_addr);

/*      if (strcmp (ip, "127.0.0.1") == 0)
	{
	  ifreq++;
	  continue;
	}
*/
//      printf("ifreq is %ld, size is %d\n", ifreq, sizeof(ifreq));
	char *temp_ip;
	temp_ip=(char *)malloc(20*sizeof(char));
	strcpy(temp_ip,ip);
    list = listAddNodeTail(list, temp_ip);
	  ifreq++;
      continue;
//      return 0;
    }

//  return -1;
//return the number of ip addresses
  printf("the number of Interface is %d\n", i);
  return i;
}

int
main ()
{

    struct list *list;
	
    list = listCreate();


  int i = getlocalip (list);
  if (i > 0)
  {
	listNode *node;	
	node = list->head;
      int k;
      for(k=0; k<list->len; k++)
      {
        printf ("本机#%d IP地址是： %s\n", k, node->value );
	node = node->next;
      }
  }
  else
    printf (" 无法获取本机IP地址 ");

    showList(list);
}
