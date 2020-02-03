#include "socket.h"

static _Atomic unsigned int client_count = 0;
static int uid = 1;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void		*server_handle_client(void *arg)
{
	char			buffer[BUF_SIZE];
	int				to_finish;
	t_client_socket *client;
	char			**words;

	to_finish = 0;
	client_count++;
	client = (t_client_socket *)arg;
	bzero(buffer, BUF_SIZE);
	while (1)
	{
		if (to_finish)
			break ;
		int recieve = recv(client->socket_fd, buffer, BUF_SIZE, 0);
		if (recieve > 0)
		{
			words = ft_strsplit(buffer, ' ');
			if (ft_count_items_strlist(words) == 3)
			{
				printf("Has recieved <%s %s %s> from ", words[0], words[1], words[2]);
				ip_address_print(client->address);
				printf(".\n");
				ft_str_overwrite_stdout();
				server_add_seq_to_cleint(client, words);
				ft_strlist_del(words);
			}
			else
				client->is_send = 1;
			server_send_msg();
		}
		else if (recieve == 0)
		{
			ip_address_print(client->address);
			printf(" #%d has disconnected from the server.\n", client->id);
			to_finish = 1;
		}
		else
		{
			printf("ERROR\n");
			to_finish = 1;
		}
		bzero(buffer, BUF_SIZE);
	}
	close(client->socket_fd);
	server_queue_remove(client->id);
	free(client);
	client_count--;
	pthread_detach(pthread_self());
	return NULL;
}

int			main(int gc, char **gv)
{
	t_server_socket		*server;
	struct sockaddr_in	client_address;
	int					connect_fd;
	pthread_t 			thread_id;

	connect_fd = 0;

	if (gc < 2)
	{
		printf("usage of %s: <port>\n", gv[0]);
		return EXIT_FAILURE;
	}
	server = (t_server_socket *)malloc(sizeof(t_server_socket));
	server_options_fill(server, gv[1]);
	signal(SIGPIPE, SIG_IGN);

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
		socklen_t clilen = sizeof(client_address);
		connect_fd = accept(server->listen_fd,
					(struct sockaddr *)&client_address,
					&clilen);
		if (client_count + 1 == MAX_CLIENTS)
		{
			printf("Maximum clients are connected. The connection is rejected.\n");
			ip_address_print(client_address);
			close(connect_fd);
			continue;
		}

		t_client_socket *client = (t_client_socket *)malloc(sizeof (t_client_socket));
		client->address = client_address;
		client->socket_fd = connect_fd;
		client->id = uid++;
		server_queue_add(client);
		printf("Client ");
		ip_address_print(client->address);
		printf(" #%d is connected.\n", client->id);
		pthread_create(&thread_id, NULL, &server_handle_client, (void *)client);

		sleep(1);
	}

	close(server->listen_fd);

	return EXIT_SUCCESS;
}
