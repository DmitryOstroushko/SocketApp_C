#include "socket.h"

void		ft_str_overwrite_stdout(void)
{
	printf("\r%s", "> ");
	fflush(stdout);
}

void		ft_str_trim_eol(char *s, int l)
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

char		*ft_strsub(char const *s, unsigned int start, size_t len)
{
	char	*new_str;
	size_t	index;

	if (!s)
		return (NULL);
	new_str = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (NULL);
	index = 0;
	while (index < len)
	{
		new_str[index] = s[start + index];
		index++;
	}
	new_str[index] = '\0';
	return (new_str);
}

int			ft_str_in_list(char *word, char **instructions)
{
	int		idx;

	for (idx = 0; idx < 4; idx++)
		if (!strcmp(word, instructions[idx]))
			return (1);
	return (0);
}

int			ft_count_items_strlist(char **words)
{
	int	count;

	if (!words)
		return (0);
	count = 0;
	while (words[count])
		count++;
	return count;
}
