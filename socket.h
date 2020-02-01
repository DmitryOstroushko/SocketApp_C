
#ifndef SOCKET_H
# define SOCKET_H

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <signal.h>

extern pthread_mutex_t clients_mutex;

# define BUF_SIZE 1024
# define MAX_CLIENTS 100
# define NAME_LEN 32

typedef struct		s_server_socket
{
	char			*ip;
	int				port;
	int				option;
	int				listen_fd;
	int				connect_fd;
	pthread_t		t_id;
	struct sockaddr_in	server_address;
	struct sockaddr_in	client_address;
}					t_server_socket;

typedef struct		s_client_socket
{
	struct sockaddr_in	address;
	int				socket_fd;
	int				uid;
	char			name[NAME_LEN];
}					t_client_socket;

typedef struct		s_connect_options
{
	int				port;
	struct hostent	*server;
	int				max_client; // to read fron server args
}					t_connect_options;


void		ret_error(char *msg);

void		server_options_fill(t_server_socket *socket, char *port);
void		server_queue_add(t_client_socket *client, t_client_socket	*clients[MAX_CLIENTS]);
void		server_queue_remove(int client_id, t_client_socket	*clients[MAX_CLIENTS]);

void		ip_address_print(struct sockaddr_in address);

void		str_overwrite_stdout(void);
void		str_trim_eol(char *s, int l);

#endif
