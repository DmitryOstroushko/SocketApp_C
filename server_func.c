
#include "socket.h"

void		server_options_fill(t_server_socket *server_socket, char *port)
{
	server_socket->ip = "127.0.0.1";
	server_socket->port = atoi(port);
	server_socket->option = 1;
	server_socket->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	server_socket->server_address.sin_family = AF_INET;
	server_socket->server_address.sin_port = htons(server_socket->port);
}

void		server_queue_add(t_client_socket *client)
{
	pthread_mutex_lock(&clients_mutex);
	for (int idx = 0; idx < MAX_CLIENTS; idx++)
	{
		if (!clients[idx])
		{
			clients[idx] = client;
			for (int jdx = 0; jdx < 3; jdx++)
				bzero(clients[idx]->seq[jdx].seq_name, 4);
			break ;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void		server_queue_remove(int client_id)
{
	pthread_mutex_lock(&clients_mutex);
	for (int idx = 0; idx < MAX_CLIENTS; idx++)
	{
		if (clients[idx] && clients[idx]->id == client_id)
		{
			clients[idx] = NULL;
			break ;
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void		server_send_msg(t_client_socket *client)
{
	char	msg[BUF_SIZE];
	int		is_break;

	pthread_mutex_lock(&clients_mutex);
	for (int idx = 0; idx < MAX_CLIENTS; idx++)
	{
		if (clients[idx] && clients[idx]->id == client->id)
		{
			is_break = 0;
			while (!is_break)
			{
				bzero(msg, BUF_SIZE);
				for (int jdx = 0; jdx < 3; jdx++)
				{
					if (strlen(client->seq[jdx].seq_name))
					{
						snprintf(msg, BUF_SIZE, "%s %ld ", msg, client->seq[jdx].n_current);
						if (write(clients[idx]->socket_fd, msg, strlen(msg)) < 0)
						{
							printf("ERROR: failed writing to descriptor\n");
							is_break = 1;
							break ;
						}
						client->seq[jdx].n_current = (client->seq[jdx].n_current + client->seq[jdx].step > LONG_MAX) ?
							client->seq[jdx].n_start : client->seq[jdx].n_current + client->seq[jdx].step;
					}
				}
			}
		}
	}
	pthread_mutex_unlock(&clients_mutex);
}

void		replace_seq_info(t_seq *seq, char **words)
{
	size_t jdx;

	seq->n_start = atoi(words[1]);
	seq->n_current = atoi(words[1]);
	seq->step = atoi(words[2]);
	for (jdx = 0; jdx < strlen(words[0]); jdx++)
		seq->seq_name[jdx] = words[0][jdx];
	seq->seq_name[jdx] = '\0';
}

void		server_add_seq_to_cleint(t_client_socket *client, char **words)
{
	int		idx;
	int		is_added;

	is_added = 0;
	for (idx = 0; idx < 3; idx++)
		if (!strcmp(client->seq[idx].seq_name, words[0]))
		{
			replace_seq_info(&(client->seq[idx]), words);
			is_added = 1;
			break ;
		}
	if (!is_added)
	{
		for (idx = 0; idx < 3; idx++)
			if (strlen(client->seq[idx].seq_name) == 0)
			{
				replace_seq_info(&(client->seq[idx]), words);
				break ;
			}
	}
}

void		server_client_print(t_client_socket *client)
{
	for (int idx = 0; idx < 3; idx++)
		if (strlen(client->seq[idx].seq_name) > 0)
			printf("%s, start = %ld, step = %ld, current = %ld.\n",
				client->seq[idx].seq_name,
				client->seq[idx].n_start,
				client->seq[idx].step,
				client->seq[idx].n_current);
}
