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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
		if (strlen(str) >= 1 &&
			strcmp(str + (len - 1), "\n") == 0) {
			str[len - 1] = '\0';
			return (str);
		}
	}
	return (str);
}

static	void	print_tab(char **tab)
{
	int i = 0;

	if (tab != NULL && tab[i] != NULL) {
		while (tab[i] != NULL && tab[i][0] != '\0') {
			printf("%s\r\n", tab[i]);
			i++;
		}
	}
}

static	void	free_tab(char **tab)
{
	int i = 0;

	if (tab != NULL) {
		while (tab[i] != NULL) {
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

static	size_t		tablen(char **tab)
{
	size_t i = 0;

	while (tab != NULL && tab[i] != NULL)
		i++;
	return (i);
}

static	char		**realloc_tab(char **tab)
{
	int i = 0;
	char **new = malloc(sizeof(char *) * (tablen(tab) + 2));

	while (tab[i]) {
		new[i] = strdup(tab[i]);
		i++;
	}
	new[i] = NULL;
	free_tab(tab);
	return (new);
}

static	void	send_file(char *filename, int sock)
{
	char	fcontent[500];
	int	fdfile, sizefile;
	struct	stat st;

	stat(filename, &st);
	sizefile = st.st_size;
	dprintf(sock,"%d\n", sizefile);
	fdfile = open(filename, O_RDONLY);
	while (sizefile > 0) {
		bzero(fcontent, 500);
		read(fdfile, fcontent, sizeof(fcontent));
		dprintf(sock, "%s", fcontent);
		sizefile = sizefile - 500;
	}
	close(fdfile);
}

static	char	**get_full_tab(int sock)
{
	int i = 0;
	char 	**tab = malloc(sizeof(char *) * 2);

	tab[i] = get_input(sock);
	if (strcmp(tab[i], "file does not exist!!") == 0) {
		send_file(tab[i], sock);
		tab[i + 1] = NULL;
	} else if (strcmp(tab[i], "file exists!") == 0 ||
		strcmp(tab[i], "Connection with server closed.") == 0) {
		tab[i + 1] = NULL;
	} else {
		while (tab[i] && strcmp(tab[i], "ENDOFCMDS") != 0) {
			i++;
			tab[i] = NULL;
			tab = realloc_tab(tab);
			tab[i] = get_input(sock);
			if (strcmp(tab[i], "ENDOFCMDS") == 0) {
				tab[i] = NULL;
				break;
			}
		}
	}
	return (tab);
}

static void			connect_client(char *ip, int port)
{
	int			sock = 0;
	struct sockaddr_in	serv_addr;
	char 			**tab;
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
	while (1) {
		cmd = get_input(0);
		dprintf(sock, "%s\n", cmd);
		tab = get_full_tab(sock);
		if (tab) {
			print_tab(tab);
			free_tab(tab);
		}
		if (strcmp(cmd, "exit") == 0) {
			free(cmd);
			break;
		}
		free(cmd);
	}
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