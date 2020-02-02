
#include "socket.h"

volatile sig_atomic_t is_exit = 0;

t_client_connect_options	client_socket;

void		ret_error(char *msg)
{
	printf("%s", msg);
	exit(1);
}

void		catch_ctrl_c_exit()
{
	is_exit = 1;
}

void		recv_msg_handler()
{
	char	msg[BUF_SIZE] = {};

	bzero(msg, BUF_SIZE);
	while (1)
	{
		int recieve = recv(client_socket.socket_fd, msg, BUF_SIZE, 0);
		if (recieve > 0)
		{
			printf("%s", msg);
			ft_str_overwrite_stdout();
		}
		else if (recieve == 0)
			break ;
		bzero(msg, BUF_SIZE);
	}
}

void		send_msg_handler()
{
	char	buffer[BUF_SIZE] = {};
	char	**words;
	char	*instructions[4] = {"seq1", "seq2", "seq3", "export"};
	int		is_send;
	int		param_count;

	bzero(buffer, BUF_SIZE);
	while (1)
	{
		ft_str_overwrite_stdout();
		fgets(buffer, BUF_SIZE, stdin);
		ft_str_trim_eol(buffer, BUF_SIZE);
		is_send = 0;
		words = ft_strsplit(buffer, ' ');
		param_count = ft_count_items_strlist(words);
		is_send = ((param_count == 2) && !strcmp(buffer, "export seq")) ||
					((param_count == 3) &&
					ft_str_in_list(words[0], instructions) &&
					(atoi(words[1]) != 0) && (atoi(words[2]) != 0));
		if (is_send)
		{
			printf("<%s> has sent to a server\n", buffer);
			send(client_socket.socket_fd, buffer, strlen(buffer), 0);
		}
		else
			printf("Didn't send to a server anything\n");
		ft_strlist_del(words);
		bzero(buffer, BUF_SIZE);
	}
/* a block abount client shutdown */
	catch_ctrl_c_exit(2);
}

int			main(int gc, char **gv)
{
	if (gc < 3)
	{
		printf("usage of %s: <hostname> <port>\n", gv[0]);
		exit(0);
	}

	client_socket.ip = gv[1];
	client_socket.port = atoi(gv[2]);
	signal(SIGINT, catch_ctrl_c_exit);
	client_socket.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket.socket_fd < 0)
	{
		printf("ERROR: it is impossible to open a socket\n");
		return EXIT_FAILURE;
	}
	client_socket.server_address.sin_family = AF_INET;
	client_socket.server_address.sin_addr.s_addr = inet_addr(client_socket.ip);
	client_socket.server_address.sin_port = htons(client_socket.port);
	client_socket.connection = connect(client_socket.socket_fd,
								(struct sockaddr*)&(client_socket.server_address),
								sizeof(client_socket.server_address));
	if (client_socket.connection < 0)
	{
		printf("ERROR: in connection\n");
		return EXIT_FAILURE;
	}
	printf("Connected to server %s is established\n", gv[1]);

	if (pthread_create(&(client_socket.send_msg_thread), NULL, (void *)send_msg_handler, NULL) != 0)
	{
		printf("ERROR: in thread creating\n");
		return EXIT_FAILURE;
	}
	if (pthread_create(&(client_socket.recv_msg_thread), NULL, (void *)recv_msg_handler, NULL) != 0)
	{
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}

	while (1)
	{
/* block about client shutdown */
		if (is_exit)
		{
			printf("\nBye\n");
			break ;
		}
	}
	close(client_socket.socket_fd);

	return (0);
}
