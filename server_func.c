
#include "socket.h"

void		server_options_fill(t_server_socket *server_socket, char *port)
{
	server_socket->ip = "127.0.0.1";
	server_socket->port = atoi(port);
	server_socket->option = 1;
	server_socket->listen_fd = 0;
	server_socket->connect_fd = 0;
	server_socket->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	server_socket->server_address.sin_family = AF_INET;
	server_socket->server_address.sin_port = htons(server_socket->port);
}

void		server_queue_add(t_client_socket *client, t_client_socket	*clients[MAX_CLIENTS])
{
	pthread_mutex_lock(&clients_mutex);
	for (int idx = 0; idx < MAX_CLIENTS; idx++)
	{
		if (!clients[idx])
		{
			clients[idx] = client;
			break ;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void		server_queue_remove(int client_id, t_client_socket	*clients[MAX_CLIENTS])
{
	pthread_mutex_lock(&clients_mutex);
	for (int idx = 0; idx < MAX_CLIENTS; idx++)
	{
		if (clients[idx] && clients[idx]->uid)
		{
			clients[idx] = NULL;
			break ;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}
