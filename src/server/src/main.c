/*
** CSULB PROJECT, 2020
** server
** File description:
** main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h> 
#include "server.h"

static	int	get_port(int ac, char **av)
{
	if (ac != 2) {
		printf("USAGE : ./server \"port number\"\n");
		exit(84);
	} else {
		int port = atoi(av[1]);
		if (port <= 0) {
			printf("USAGE : ./server \"port number\"\n");
			exit(84);
		}
		return (port);
	}
}
static void create_socket(int port, int *sfd)
{
    struct sockaddr_in servaddr;

	*sfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (*sfd == -1) { 
        printf("ERROR (%d): socket creation failed.\n", errno); 
        exit(0); 
    }
	printf("Socket created.\n"); 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);
	printf("%d\n", port);
	if ((bind(*sfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("ERROR (%d): socket bind failed.\n", errno); 
        exit(0);
    }
	printf("Socket binded on port %d.\n", port); 
}

void *func_cli(void *clifd)
{
	int cfd = *((int *)clifd);
	char buffer[BUFF_MAX];

	while (1) {
		bzero(buffer, BUFF_MAX); 
        read(cfd, buffer, sizeof(buffer)); 
        printf("%s", buffer);
		if (strncmp("exit\n", buffer, 5) == 0) {
			shutdown(cfd, SHUT_RDWR);
			close(cfd);
            printf("Connection with client closed.\n");
            break;
        }
	}
	return (clifd);
}

static void listen_for_cli(int sfd)
{
	int cfd;
	socklen_t len;
	struct sockaddr_in cli;
	pthread_t tid; 

	printf("Server start listening...\n");
	while (1) {
		if ((listen(sfd, 5)) != 0) { 
			printf("ERROR (%d): server listen failed.\n", errno); 
			exit(0); 
		}
		bzero(&cli, sizeof(cli));
		len = sizeof(cli);
		cfd = accept(sfd, (SA*)&cli, &len); 
		if (cfd < 0) { 
			printf("ERROR (%d): server accept failed.\n", errno); 
			exit(0); 
		}
		printf("Connected with %s.\n", inet_ntoa(cli.sin_addr));
		pthread_create(&tid, NULL, func_cli, (void *)&cfd);
	}
}

int	main(int ac, char **av)
{
	int sfd;
	create_socket(get_port(ac, av), &sfd);
	listen_for_cli(sfd);
	return (0);
}