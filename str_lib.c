#include "socket.h"

void		str_overwrite_stdout(void)
{
	printf("\r%s", "> ");
	fflush(stdout);
}

void		str_trim_eol(char *s, int l)
{
	for (int idx = 0; idx < l; idx++)
	{
		if (s[idx] == '\n')
		{
			s[idx] = '\0';
			break ;
		}
	}
}
