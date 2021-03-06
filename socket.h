
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
# include <limits.h>

extern pthread_mutex_t	clients_mutex;

# define BUF_SIZE 1024
# define MAX_CLIENTS 100
# define NAME_LEN 32

typedef struct 		s_seq
{
	char			seq_name[4];
	long			n_start;
	long			n_current;
	long			step;
}					t_seq;

typedef struct		s_server_socket
{
	char			*ip;
	int				port;
	int				option;
	int				listen_fd;
	struct sockaddr_in	server_address;
}					t_server_socket;

typedef struct		s_client_socket
{
	int				id;
	int				socket_fd;
	char			name[NAME_LEN];
	int				is_send;
	t_seq			seq[3];
	struct sockaddr_in	address;
}					t_client_socket;

typedef struct		s_client_connect_options
{
	int				port;
	char			*ip;
	int				socket_fd;
	int				connection;
	char			name[NAME_LEN];
	struct sockaddr_in	server_address;
	pthread_t		send_msg_thread;
	pthread_t		recv_msg_thread;
	struct hostent	*server;
}					t_client_connect_options;

t_client_socket			*clients[MAX_CLIENTS];

void		ret_error(char *msg);

void		server_options_fill(t_server_socket *socket, char *port);
void		server_queue_add(t_client_socket *client);
void		server_queue_remove(int client_id);
void		server_send_msg(void);
void		*server_handle_client(void *arg);
void		server_add_seq_to_cleint(t_client_socket *client, char **words);
void		server_client_print(t_client_socket *client);

void		ip_address_print(struct sockaddr_in address);

void		ft_str_overwrite_stdout(void);
void		ft_str_trim_eol(char *s, int l);
void		ft_strdel(char **as);
void		ft_memdel(void **ap);
char		**ft_strsplit(char const *s, char c);
void		ft_strlist_del(char **words);
char		*ft_strsub(char const *s, unsigned int start, size_t len);
int			ft_str_in_list(char *word, char **instructions);
int			ft_count_items_strlist(char **words);

#endif
