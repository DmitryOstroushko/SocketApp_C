
#include "socket.h"

static _Atomic unsigned int client_count = 0;
static int uid = 10;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int			main(int gc, char **gv)
{
	t_server_socket	*server;
	t_client_socket	*clients[MAX_CLIENTS];

	if (gc < 2)
	{
		printf("usage of %s: <port>\n", gv[0]);
		return EXIT_FAILURE;
	}
	server = (t_server_socket *)malloc(sizeof(t_server_socket));
	server_options_fill(server, gv[1]);
	signal(SIGPIPE, SIG_IGN);
//	sigaction(SIGPIPE, SIG_IGN);

	if (setsockopt(server->listen_fd,
					SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR),
					(char *)&(server->option),
					sizeof(server->option)) < 0)
	{
		printf("ERROR: setsockopt\n");
		return EXIT_FAILURE;
	}

	if (bind(server->listen_fd,
			(struct sockaddr *)&(server->server_address),
			sizeof(server->server_address)) < 0)
	{
		printf("ERROR: bind\n");
		return EXIT_FAILURE;
	}

	if (listen(server->listen_fd, 10) < 0)
	{
		printf("ERROR: listen\n");
		return EXIT_FAILURE;
	}

	printf("SERVER is listening ...\n");

	while (1)
	{
		socklen_t clilen = sizeof(server->client_address);
		server->connect_fd = accept(server->listen_fd,
								(struct sockaddr *)&(server->client_address),
								&clilen);
		if (client_count + 1 == MAX_CLIENTS)
		{
			printf("Maximum clians are connected. The connection is rejected.\n");
			ip_address_print(server->client_address);
			close(server->connect_fd);
			continue;
		}
	}


	return EXIT_SUCCESS;
}
