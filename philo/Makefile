CFLAGS = -Wall -Werror -Wextra -pthread -g

NAME = philo

all:
	gcc $(CFLAGS) *.c */*.c -o $(NAME)

debug: 
	gcc $(CFLAGS) -D DEBUG_MODE=1 *.c */*.c -o $(NAME)

sanitize:
	gcc $(CFLAGS) -fsanitize=thread  *.c */*.c -o $(NAME)

.PHONY: debug