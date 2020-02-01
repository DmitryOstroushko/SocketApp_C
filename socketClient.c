
#include "socket.h"

void		ret_error(char *msg)
{
	printf("%s", msg);
	exit(1);
}

int			main(int gc, char **gv)
{
	int					client_socket;
	int					connection;
	struct sockaddr_in	server_addr;
	char				buffer[BUF_SIZE];
	t_connect_options	copts;

	if (gc < 3)
	{
		printf("usage of %s: hostname port\n", gv[0]);
		exit(0);
	}

	// port number
	copts.port = atoi(gv[2]);
	// create a socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0)
		ret_error("ERROR: it is impossible to open a socket\n");
	// get IP of a server
	copts.server = gethostbyname(gv[1]);
	if (!copts.server)
		ret_error("ERROR: no such host\n");

	memset(&server_addr, '\0', sizeof(server_addr));
	// a socket type
	server_addr.sin_family = AF_INET;
	// a socket port
	server_addr.sin_port = htons(copts.port);
	// IP address
	bcopy((char *)(copts.server)->h_addr_list, (char *)&server_addr.sin_addr.s_addr, copts.server->h_length);
	// make a connection
	printf("Connecting ...");
	connection = connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (connection < 0)
		ret_error("ERROR: in connection\n");
	printf("Connected to server %s is established\n", gv[1]);

	// talking with the server
	while (1)
	{
		printf("Client: ");
		scanf("%s", &buffer[0]);
		send(client_socket, buffer, strlen(buffer), 0);

		// to stop client
		if (!strcmp(buffer, "exit"))
		{
			close(client_socket);
			printf("Client is disconnected\n");
			exit(1);
		}

		if (recv(client_socket, buffer, 1024, 0) < 0)
			printf("ERROR in data receiving from server\n");
		else
			printf("Msg from server: %s\n", buffer);
	}
	return (0);
}
