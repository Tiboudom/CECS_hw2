/*
** CSULB PROJECT, 2020
** client
** File description:
** main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

static char 			*get_input(int sock)
{
	ssize_t			fd;
	size_t			len;
	char			buf[2];
	char 			*str;

	str = malloc(sizeof(char) * 1);
	str[0] = '\0';
	while ((fd = read(sock, buf, 1)) > 0) {
		buf[fd] = '\0';
		str = strcat(realloc(str, strlen(str) + 2), buf);
		len = strlen(str);
		if (strlen(str) >= 2 &&
			strcmp(str + (len - 2), "\r\n") == 0) {
			str[len - 2] = '\0';
			return (str);
		} else if (strlen(str) >= 1 &&
			strcmp(str + (len - 1), "\n") == 0) {
			str[len - 1] = '\0';
			return (str);
		}
	}
	return (str);
}

static void			connect_client(char *ip, int port)
{
	int			sock = 0;
	struct sockaddr_in	serv_addr;
	char 			*str;
	char 			*cmd;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket creation error\n");
		exit(-1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
		printf("Invalid address\n");
		exit(-1);
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Connection Failed\n");
		exit(-1);
	}
	printf("Connection established\n");
	printf("IP: %s Port: %d\n", ip, port);
	cmd = get_input(0);
	dprintf(sock, "%s\n", cmd);
	str = get_input(sock);
	printf("%s\n", str);
	free(str);
	free(cmd);
}

int	main(int ac, char **av)
{
	if (ac != 3) {
		printf("USAGE : ./client \"IP Address\" \"port number\"\n");
		exit(-1);
	} else {
		connect_client(av[1], atoi(av[2]));
	}
	return 0;
}