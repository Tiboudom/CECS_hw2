/*
** CSULB PROJECT, 2020
** sever
** File description:
** main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include "server.h"

static	void	connect_port(int port)
{
	printf("Connected!\n");
	printf("Port is : %d\n", port);
}

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

int	main(int ac, char **av)
{
	connect_port(get_port(ac, av));
	return (0);
}