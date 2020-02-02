# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: clianne <clianne@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/04 05:29:44 by clianne           #+#    #+#              #
#    Updated: 2020/02/02 12:59:14 by clianne          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME1 = server
NAME2 = client
MAKE = make
FLAGS = -Wall -Wextra -Werror -pthread
# FLAGS = -Wall -g3 -fsanitize=address -pthread
SRCS = ip_addr.c str_lib.c ft_strsplit.c

SRCS1 = $(SRCS) server_func.c socketServer.c
SRCS2 = $(SRCS) socketClient.c
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)

all: $(NAME1) $(NAME2)

$(LIB):
	cd $(MAKE)

%.o: %.c $(LIB)
	gcc -c $(FLAGS) $<

$(NAME1): $(OBJS1)
	gcc $(FLAGS) $^ -o $@

$(NAME2): $(OBJS2)
	gcc $(FLAGS) $^ -o $@

clean:
	rm -f $(OBJS1) $(OBJS2)

fclean: clean
	rm -f $(NAME1) $(NAME2)

re: fclean all

.PHONY: all clean fclean re
