
#ifndef SOCKET_H
# define SOCKET_H

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <netdb.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define BUF_SIZE 1024
# define MAX_CLIENT 100

typedef struct		s_connect_options
{
	int				port;
	struct hostent	*server;
	int				max_client; // to read fron server args
}					t_connect_options;


void		ret_error(char *msg);


#endif
