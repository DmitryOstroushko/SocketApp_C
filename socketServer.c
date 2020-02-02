
#include "socket.h"

static _Atomic unsigned int client_count = 0;
static int uid = 1;

pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void		*server_handle_client(void *arg)
{
	char			buffer[BUF_SIZE];
//	char			name[NAME_LEN];
	int				to_finish;
	t_client_socket *client;
	char			**words;

	to_finish = 0;
	client_count++;
	client = (t_client_socket *)arg;
/* a block about client name
	if (recv(client->socket_fd, name, NAME_LEN, 0) <= 0 ||
		strlen(name) < 2 || strlen(name) >= NAME_LEN - 1)
	{
		printf("ERROR: type the name correctly\n");
		to_finish = 1;
	}
	else
	{
		strcpy(client->name, name);
		sprintf(buffer, "%s has joined to the server\n", client->name);
		printf("%s", buffer);
		server_send_msg(buffer, client->id);
	}
*/
	bzero(buffer, BUF_SIZE);
	while (1)
	{
		if (to_finish)
			break ;
		int recieve = recv(client->socket_fd, buffer, BUF_SIZE, 0);
		if (recieve > 0)
		{
			words = ft_strsplit(buffer, ' ');
			printf("Has recieved <%s %s %s> from ", words[0], words[1], words[2]);
			ip_address_print(client->address);
			printf(" #%d.", client->id);
			ft_str_overwrite_stdout();
			server_add_seq_to_cleint(client, words);
			server_send_msg(buffer, client->id);
			ft_str_trim_eol(buffer, strlen(buffer));
			ft_strlist_del(words);
//			printf("%s -> %s", buffer, client->name);
		}
		else if (recieve == 0)
		{
			ip_address_print(client->address);
			printf(" #%d has disconnected from the server.\n", client->id);
//			sprintf(buffer, "%s has disconnected from the server\n", client->address.sin_addr);
//			printf("%s\n", buffer);
			server_send_msg(buffer, client->id);
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
		socklen_t clilen = sizeof(client_address);
		connect_fd = accept(server->listen_fd,
					(struct sockaddr *)&client_address,
					&clilen);
		if (client_count + 1 == MAX_CLIENTS)
		{
			printf("Maximum clians are connected. The connection is rejected.\n");
			ip_address_print(client_address);
			close(connect_fd);
			continue;
		}

		t_client_socket *client = (t_client_socket *)malloc(sizeof (t_client_socket));
		client->address = client_address;
		client->socket_fd = connect_fd;
		client->id = uid++;
		server_queue_add(client);
		pthread_create(&thread_id, NULL, &server_handle_client, (void *)client);

		sleep(1);
	}

	close(server->listen_fd);

	return EXIT_SUCCESS;
}
