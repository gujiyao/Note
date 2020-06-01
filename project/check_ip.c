#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
int check_ip_net(uint8_t *ip_net, int version)
//int check_ip_net(const unsigned char *ip_net, int version)
{
	int i = 0;
	if(version == 4)
	{
		for(i = 0; i < 4; i ++)
		{
			if((ip_net + i) != NULL &&(*(ip_net + i) >= 0 && *(ip_net + i) <= 255))
				continue;
			else
				return -1;
		}
	}
	else
	{
		for(i = 0; i < 16; i ++)
		{
			if((ip_net + i) != NULL &&(*(ip_net + i) >= 0 && *(ip_net + i) <= 255))
                                continue;
                        else
                                return -1;
		}
	}
	return -1;
}

int check_ip(char *ip, int version)
{
        if(version == 4)
        {
                int ipnum[4];
                char c='\0';
                int err;
                err = sscanf(ip, "%d.%d.%d.%d%c", &ipnum[0], &ipnum[1], &ipnum[2], &ipnum[3], &c);
                if(err == 4)
                {
                        if(ipnum[0] >= 0 && ipnum[0] <= 255 && ipnum[1] >= 0&&ipnum[1] <= 255 && ipnum[2] >= 0&& ipnum[2] <= 255 && ipnum[3] >= 0 && ipnum[3] <= 255 && c=='\0')
                        {
                                return 0;
                        }
                }
        }
        else
        {
                struct in6_addr ip6;
                int ret;
                ret = inet_pton(AF_INET6, ip,(void *)&ip6);
                if(ret <= 0)
                        return -1;
                else
                        return 0;
        }
        return -1;
}

int main()
{
	char ip[16] = "192.168.23.256";
	//char ip[16] = "192.168.23.3";
	struct sockaddr_in server_addr;
	inet_pton(AF_INET, ip, &server_addr.sin_addr);
	
	if (check_ip_net((uint8_t*)&(server_addr.sin_addr.s_addr), 4) == 0)
	//if (check_ip_net((const unsigned char *)&(server_addr.sin_addr.s_addr), 4) == 0)
		printf("check_ip_net:%s OK\n", ip);
	if (check_ip(ip, 4) == 0)
		printf("check_ip:%s OK\n", ip);
	return 0;
}
