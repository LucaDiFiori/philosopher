/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-08 17:13:24 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-08 17:13:24 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_BONUS_H
# define PHILOSOPHER_BONUS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

/*the following strings are used to create the named semaphores*/
#define SEM_PRINT "/sem_print"
#define SEM_FORKS "/sem_forks"
#define SEM_IM_EATIN "/sem_eating"
#define SEM_DEATH "/sem_death"

/*the following strings are used to print the status of the philosophers*/
#define FORK "has taken a fork"
#define EAT "is eating"
#define SLEEP "is sleeping"
#define THINK "is thinking"
#define DIED "died"
#define ALL_FED "All philosophers have eaten enough"


/*
In this program, the main process will create table.nb_philo child processes.
Each child rappresent a philosopher.
*/
typedef struct s_table
{
	int				id;
	pid_t			*philo_pid;
	int				nb_philo;   
	int				max_meals;
	size_t			death_time;
	size_t			eat_time;
	size_t			sleep_time;
	size_t			start_time;
	int				meals_eaten;
	int				fed_philos;
	sem_t			*sem_print;
	sem_t			*sem_forks;
	sem_t			*sem_eating;
	sem_t			*sem_death;
	size_t			last_meal;
	int				death_flag;
}	t_table;




void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(const char *nptr);
int		ft_strncmp_philo(const char *s1, const char *s2);
void	philo_init(int argc, char **argv, t_table *table);
size_t	get_current_time();
void	print_status(t_table *table, const char *action);
void	quit_and_free(t_table *table);
void	take_forks(t_table *table);
void	eat(t_table *table);
void	sleep_routine(t_table *table);
void	think(t_table *table);
int		check_death_flag(t_table *table);
void	modify_death_flag(t_table *table);
void	single_philo(t_table *table);

#endif