# CFLAGS = -Wall -Werror -Wextra -pthread -g -fsanitize=thread 
CFLAGS = -Wall -Werror -Wextra -pthread -g

NAME = philosophers

# -fsanitize=thread  detect race condition

all:
	gcc $(CFLAGS) philo/*.c philo/*/*.c -o $(NAME)
	./$(NAME)

debug: 
	gcc $(CFLAGS) -D DEBUG_MODE=1 philo/*.c philo/*/*.c -o $(NAME)
	./$(NAME)

.PHONY: debug