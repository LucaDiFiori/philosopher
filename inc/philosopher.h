/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:46:11 by ldi-fior          #+#    #+#             */
/*   Updated: 2024/05/29 15:46:11 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#define MALLOC "Error: malloc failed\n"
#define LOCK_MUTEX "Error: pthread_mutex_lock failed\n"
#define UNLOCK_MUTEX "Error: pthread_mutex_unlock failed\n"
#define PT_CREATE "Error: pthread_create failed\n"
#define PT_DETACH "Error: pthread_detach failed\n"
#define PT_JOIN "Error: pthread_join failed\n"



/*PROVARE A NON ALLOCARE E METTERE ARRAY DA 200*/





/*
This struct is used to store shared data between threads.
fields: - nb_philo: number of philosophers
		- death_time: time in milliseconds after which a philosopher dies
		- eat_time: time in milliseconds a philosopher takes to eat
		- sleep_time: time in milliseconds a philosopher takes to sleep
		- start_time: time in milliseconds when the simulation started.
		              (use to calculate the time since the start of the simulation
				       for the print function: (get_current_time() - start_time)
		- max_meals: number of meals a philosopher must eat before stopping
		- print_lock: mutex used to lock the print function
		- forks: array containing all the forks mutexes
		- philos_arr: array containing all the philosophers
		- ready_count: number threads that are ready to start
		- ready_lock: mutex used to lock the ready_count variable
		- fed_philos: number of philosophers that have eaten all the meals
		- fed_philos_lock: mutex used to lock the fed_philos variable
		- death: flag used to stop the simulation when a philosopher dies or
		         all the philosophers have eaten all the meals
				 (stops the while loop in the philosopher routine 
				 and the main thread to join all the threads and exit the program)
	    - death_lock: mutex used to lock the death variable and read it safely.*/
typedef struct s_table
{
	int				nb_philo;
	size_t			death_time;
	size_t			eat_time;
	size_t			sleep_time;
	size_t			start_time;
	int				max_meals;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
	struct s_philo	*philos_arr;
	int				ready_count;
	pthread_mutex_t	ready_lock;
	int				fed_philos;
	pthread_mutex_t	fed_philos_lock;
	int death;
	pthread_mutex_t death_lock;
}	t_table;


/*
This struct is used to store data for each philosopher.
fields: - thread: thread used to run the philosopher
		- id: id of the philosopher
		- meals_eaten: number of meals eaten by the philosopher
		- im_eating_lock: Mutex used to block access to the "last_meal_time" 
		                  variable and prevent the main thread from checking 
						  it before it is modified by the philosopher.
						  This mutex ensures that no philosopher will die while eating.
		- last_meal_time: time in milliseconds when the philosopher last ate
		- left_fork: pointer to the left fork mutex
		- right_fork: pointer to the right fork mutex
		- table: pointer to the shared data*/
typedef struct s_philo
{
	pthread_t 				thread;
	int						id;
	int						meals_eaten;
	pthread_mutex_t			im_eating_lock; 
	size_t					last_meal_time;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	t_table					*table;

	int *death;
} t_philo;



/*minilib*/
void	ft_putstr_fd(char *s, int fd);
int	ft_atoi(const char *nptr);
int	ft_strncmp_philo(const char *s1, const char *s2);

/*philo_checks*/
int	check_table(int argc, char **argv, t_table *table);

/*philo init*/
void		philo_init(int argc, char **argv, t_table *table);

/*philo_utils.c*/
size_t	get_current_time();
void    print_status(t_philo *philo, const char *action);

/*philo_routine_utils*/
void	philosopher_getting_ready(t_table *table);
void	take_forks(t_philo *philo);
void	sleep_routine(t_philo *philo);
void	think_routine(t_philo *philo);
void	eat_routine(t_philo *philo);

/*philo_err_utils*/
void    free_and_quit(t_table *table, char *err, int err_type);

/*philo_moniot_routine*/
int check_meals(t_table *table);
int check_death(t_table *table, int i);
int death_flagh_monitoring(t_table *table);
int monitor_helper(t_philo *philo, int flag);

