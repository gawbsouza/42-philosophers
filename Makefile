# CFLAGS = -Wall -Werror -Wextra -pthread -g -fsanitize=thread 
CFLAGS = -Wall -Werror -Wextra -pthread -g

NAME = philosophers

all:
	gcc $(CFLAGS) *.c */*.c -o $(NAME)
	./$(NAME)

debug: 
	gcc $(CFLAGS) -D DEBUG_MODE=1 *.c */*.c -o $(NAME)
	./$(NAME)

.PHONY: debug