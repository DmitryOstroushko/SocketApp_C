#include "socket.h"

void		ip_address_print(struct sockaddr_in address)
{
	printf("%d.%d.%d.%d",
			address.sin_addr.s_addr & 0xff,
			(address.sin_addr.s_addr & 0xff00) >> 8,
			(address.sin_addr.s_addr & 0xff0000) >> 16,
			(address.sin_addr.s_addr & 0xff000000) >> 24);

}
