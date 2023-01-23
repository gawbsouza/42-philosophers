# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/23 15:50:58 by gasouza           #+#    #+#              #
#    Updated: 2023/01/23 21:51:16 by gasouza          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		=	philo
SRCS		=	debug/summary.c				\
				runners/monitor_runner.c	\
				runners/philo_runner.c		\
				utils/init_utils.c			\
				utils/number_utils.c		\
				utils/philo_utils.c			\
				utils/status_utils.c		\
				utils/time_utils.c			\
				main.c
OBJS		=	$(SRCS:.c=.o)
COMPILER	= 	gcc -c
LINKER		=	gcc -pthread
CFLAGS 		= 	-Wall -Werror -Wextra
CLEANUP		=	rm -rf
DEBUG_MODE	=	0


all:		$(NAME)

$(NAME):	$(OBJS)
			$(LINKER) $(OBJS) -o $(NAME)

%.o: %.c
			$(COMPILER) $(CFLAGS) -D DEBUG_MODE=$(DEBUG_MODE) $< -o $@

clean:
			$(CLEANUP) $(OBJS)

fclean:		clean
			$(CLEANUP) $(NAME)

re:			fclean all

debug:		DEBUG_MODE := 1
debug:		re

sanitize:	$(OBJS)
			$(LINKER) $(CFLAGS) -fsanitize=thread $(OBJS) -o $(NAME)

.PHONY:		all clean fclean re debug sanitize
