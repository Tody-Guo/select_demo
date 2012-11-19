/*
*  (C) 2012, T-ware Inc.
*
*  Test select function of Linux for non-block socket.
*  
*  - Tody Guo
*
*  Open source, GPL2.0
* 
*/



#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	int fd = 0, new_fd = 0;
	struct sockaddr_in addr;
	struct sockaddr_in cli_addr;
	fd_set fds;
	struct timeval timeout;
	int len;
	char buf[1024] = {'\0'};
	long count = 0;

    fd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&addr, '\0' , sizeof(addr)); 		    
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8086);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	
	timeout.tv_sec = 1;
	timeout.tv_usec = 5000;


	if (-1 == bind(fd, (struct sockaddr *)&addr, sizeof(addr)))
	{
		printf("bind err\n");
		return (1);
	}
								    
	if(-1 == listen(fd, 5))
	{
		printf("listen err\n");
		return (1);
	}
	

	printf("Initialing... ok\n");

	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		switch(select(fd+1, &fds, NULL, NULL, &timeout))
		{
			case -1:
				printf("select error!\r");
				break;

			case 0:
				printf("timeout %u\r", count++);
				break;

			default:
				if (FD_ISSET(fd, &fds))
				{
					len = sizeof(cli_addr);

					new_fd = accept(fd, (struct sockaddr *)&cli_addr, &len);
					if(recv(new_fd, buf, sizeof(buf), 0)<0)
					{
						printf("recv error occured!\n");
						close(new_fd);
						continue;
					}
					buf[strlen(buf)]='\0';
					printf("\nRECV: %s len: %d \n", buf, strlen(buf));
					
					if(send(new_fd, buf, sizeof(buf), 0)<0)
					{
						printf("send error occured!\n");
						close(new_fd);
						continue;
					}

					close(new_fd);
				}
		
		}
	
		fflush(stdout);
	}


}
