CFLAGS = -Wall -Werror -Wextra -pthread -g -fsanitize=thread 

NAME = philosophers

all:
	gcc $(CFLAGS) *.c -o $(NAME)
	./$(NAME)