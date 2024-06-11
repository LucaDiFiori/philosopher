NAME = philobonus
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread

SRCS = src/philo_bonus.c \
	   src/philo_bonus_init.c \
	   src/minilib.c \
	   src/philo_bonus_utils.c \
	   src/philo_bonus_actions.c \

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