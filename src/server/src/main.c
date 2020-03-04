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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <dirent.h>
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
        exit(84); 
    }
	printf("Socket created.\n"); 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);
	if ((bind(*sfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("ERROR (%d): socket bind failed.\n", errno); 
        exit(84);
    }
	printf("Socket binded on port %d.\n", port); 
}

static void command_dir(int cfd, char path[])
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (path)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			dprintf(cfd, "%s\n", ent->d_name);
		}
	}
	closedir (dir);
}

static void command_file(int cfd, char filename[])
{
	char fcontent[FILE_MAX];
	int fdfile, sizefile;

	if (access(filename, F_OK) != -1) {
		dprintf(cfd, "file exists!\n");
	} else {
		dprintf(cfd, "file does not exist!!\n");
		sizefile = 0;
		char fread = 0;
		while (fread != '\n') {
			read(cfd, &fread, 1);
			if (fread >= '0' && fread <= '9')
				sizefile = (sizefile * 10) + (fread - '0');
		}
		fdfile = open(filename, O_WRONLY | O_APPEND | O_CREAT, 0644);
		int i = 0;
		int r = 0;
		while (i < sizefile) {
			bzero(fcontent, sizeof(fcontent));
			r = read(cfd, fcontent, sizeof(fcontent));
			write(fdfile, fcontent, r);
			i = i + r;
		}
		close(fdfile);
	}
}

static void *func_cli(void *clifd)
{
	int cfd = *((int *)clifd);
	char buffer[BUFF_MAX];
	char path[PATH_MAX];

	getcwd(path, sizeof(path));
	while (1) {
		bzero(buffer, BUFF_MAX);
        read(cfd, buffer, sizeof(buffer));
		buffer[strlen(buffer)-1] = 0;
		if (strncmp(COM_EXIT, buffer, 4) == 0) {
			dprintf(cfd, "Connection with server closed.\n");
			shutdown(cfd, SHUT_RDWR);
			close(cfd);
            printf("Connection with client closed.\n");
            break;
        } else if (strncmp(COM_DIR, buffer, 3) == 0) {
			command_dir(cfd, path);
		} else {
			command_file(cfd, buffer);
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
			exit(84); 
		}
		bzero(&cli, sizeof(cli));
		len = sizeof(cli);
		cfd = accept(sfd, (SA*)&cli, &len); 
		if (cfd < 0) { 
			printf("ERROR (%d): server accept failed.\n", errno); 
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