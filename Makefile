# CFLAGS = -Wall -Werror -Wextra -pthread -g -fsanitize=thread 
CFLAGS = -Wall -Werror -Wextra -pthread -g

NAME = philosophers

all:
	gcc $(CFLAGS) *.c -o $(NAME)
	./$(NAME)