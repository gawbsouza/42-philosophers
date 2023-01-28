# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/23 15:50:58 by gasouza           #+#    #+#              #
#    Updated: 2023/01/28 14:15:28 by gasouza          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philo
SRCS		=	summary.c			\
				ft_atoi.c			\
				philo_utils.c		\
				forks_utils.c		\
				monitor_runner.c	\
				philo_runner.c		\
				simulation_run.c	\
				time_millisec.c		\
				main.c
OBJS		=	$(SRCS:.c=.o)
COMPILER	= 	gcc -c
LINKER		=	gcc -pthread
CFLAGS 		= 	-Wall -Werror -Wextra -g
CLEANUP		=	rm -rf
SUMMARY		=	1


all:		$(NAME)

$(NAME):	$(OBJS)
			$(LINKER) $(OBJS) -o $(NAME)

%.o: %.c
			$(COMPILER) $(CFLAGS) -D SHOW_SUMMARY=$(SUMMARY) $< -o $@

clean:
			$(CLEANUP) $(OBJS)

fclean:		clean
			$(CLEANUP) $(NAME)

re:			fclean all

no-summary:	SUMMARY := 0
no-summary:	re

sanitize:	$(OBJS)
			$(LINKER) $(CFLAGS) -fsanitize=thread $(OBJS) -o $(NAME)

.PHONY:		all clean fclean re summary sanitize
