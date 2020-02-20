/*
** CSULB PROJECT, 2020
** client
** File description:
** main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include "client.h"

static	void	connect_to_server(int ip, int port)
{
	printf("Connected!\n");
	printf("IP is : %d\n", ip);
	printf("Port is : %d\n", port);
}

static	int	get_ip(int ac, char **av)
{
	if (ac != 3) {
		printf("USAGE : ./client \"IP Address\" \"port number\"\n");
		exit(84);
	} else {
		int ip = atoi(av[1]);
		if (ip <= 0) {
			printf("USAGE : ./client \"IP Address\" \"port number\"\n");
			exit(84);
		}
		return (ip);
	}
}

static	int	get_port(int ac, char **av)
{
	if (ac != 3) {
		printf("USAGE : ./client \"IP Address\" \"port number\"\n");
		exit(84);
	} else {
		int port = atoi(av[2]);
		if (port <= 0) {
			printf("USAGE : ./client \"IP Address\" \"port number\"\n");
			exit(84);
		}
		return (port);
	}
}

int	main(int ac, char **av)
{
	connect_to_server(get_ip(ac, av), get_port(ac, av));
	return (0);
}