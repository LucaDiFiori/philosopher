NAME = philo
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread

SRCS = src/philosopher.c \
	   src/minilib.c \
	   src/philo_checks.c \
	   src/philo_init.c \
	   src/philo_utils.c \
	   src/philo_routine_utils.c \
	   src/philo_err_utils.c \
	   src/philo_monitor_routine.c \
	   

OBJS = $(SRCS:%.c=obj/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

obj/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf obj

fclean: clean
	@rm -f $(NAME)

re: fclean all