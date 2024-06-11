/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_checks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-08 17:18:22 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-08 17:18:22 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher_bonus.h" 

static int	check_value(int argc, t_table *table)
{
	if (table->nb_philo <= 0 || table->nb_philo > 200)
	{
		ft_putstr_fd("Error: wrong number of philosophers\n", 2);
		return (1);
	}
	if (table->death_time <= 0 || table->eat_time <= 0
		|| table->sleep_time <= 0)
	{
		ft_putstr_fd("Error: wrong time value\n", 2);
		return (1);
	}
	if (argc == 6 && table->max_meals <= 0)
	{
		ft_putstr_fd("Error: wrong number of meals\n", 2);
		return (1);
	}
	return (0);
}

static int	check_table(int argc, char **argv, t_table *table)
{
	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	table->nb_philo = ft_atoi(argv[1]);
	table->death_time = ft_atoi(argv[2]);
	table->eat_time = ft_atoi(argv[3]);
	table->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;

	if (check_value(argc, table))
		return (1);

	return (0);
}


/*
NOTE 1: first, I unlink the semaphores to avoid conflicts with previous executions.
		This is because the semaphores are named and they are not removed when 
		the program ends.
NOTE 2: sem_open creates a new named semaphore or opens an existing named semaphore.
NOTE 3: A named semaphore is sharable between processes.
*/
static void init_semaphores(t_table *table)
{
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_IM_EATIN);
	sem_unlink(SEM_DEATH);

	table->sem_print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	table->sem_forks = sem_open(SEM_FORKS, O_CREAT, 0644, table->nb_philo);
	table->sem_eating = sem_open(SEM_IM_EATIN, O_CREAT, 0644, 1);
	table->sem_death = sem_open(SEM_DEATH, O_CREAT, 0644, 1);
}

/*NOTE: The philo_pid array is used to store the pid of each philosopher.*/
static void init_table(t_table *table)
{
	init_semaphores(table);
	table->philo_pid = malloc(sizeof(pid_t) * table->nb_philo);
	if (!table->philo_pid)
	{
		ft_putstr_fd("Error: malloc failed\n", 2);
		exit(EXIT_FAILURE);
	}
	table->start_time = get_current_time();
	table->meals_eaten = 0;
	table->fed_philos = 0;
	table->death_flag = 0;
}

void	philo_init(int argc, char **argv, t_table *table)
{
	if (check_table(argc, argv, table))
		exit(1);
	init_table(table);
}