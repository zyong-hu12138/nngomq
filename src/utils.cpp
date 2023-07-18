
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "utils.h"
#include "parameter.h"
using namespace std;

int  get_local_ip(char *ip){
    int i=0;
	int sockfd;
	struct ifconf ifc;
	char buf[1024]={0};
	char ipbuf[20]={0};
	struct ifreq *ifr;
 
	ifc.ifc_len = 1024;
	ifc.ifc_buf = buf;
 
	if((sockfd = socket(AF_INET, SOCK_DGRAM,0))<0)
	{
		error="socket can't create";
		return -1;
	}
	ioctl(sockfd,SIOCGIFCONF, &ifc);
	ifr = (struct ifreq*)buf;
 
	for(i=(ifc.ifc_len/sizeof(struct ifreq)); i > 0; i--)
	{
		//printf("net name: %s\n",ifr->ifr_name);
		inet_ntop(AF_INET,&((struct sockaddr_in *)&ifr->ifr_addr)->sin_addr,ipbuf,20);
		//printf("ip: %s \n",ipbuf);
		
        if(strcmp(ipbuf,"127.0.0.1") != 0)
		{
			strcpy(ip,ipbuf);
			return 0;
		}
		
    		
		ifr = ifr +1;
	}
    return -1;
}