/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:48:21 by ldi-fior          #+#    #+#             */
/*   Updated: 2024/05/29 16:48:21 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

static void	init_table (t_table *table)
{
	int i;

	table->ready_count = 0;
	table->fed_philos = 0;
	table->start_time = get_current_time();
	table->death = 0;
	pthread_mutex_init(&table->ready_lock, NULL);
	pthread_mutex_init(&table->print_lock, NULL);

	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	table->philos_arr = malloc(sizeof(t_philo) * table->nb_philo); 
	if (!table->forks || !table->philos_arr)
		free_and_quit(table, MALLOC, 1);
	i = -1;
	while (++i < table->nb_philo)
	{
		pthread_mutex_init(&table->forks[i], NULL);
	}
	pthread_mutex_init(&table->fed_philos_lock, NULL);
	pthread_mutex_init(&table->death_lock, NULL);
}

static void	init_philosopher(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->nb_philo)
	{
		table->philos_arr[i].id = i + 1;
		table->philos_arr[i].meals_eaten = 0;
		pthread_mutex_init(&table->philos_arr[i].im_eating_lock, NULL);
		table->philos_arr[i].last_meal_time = get_current_time();
		table->philos_arr[i].left_fork = &table->forks[i];
		table->philos_arr[i].right_fork = &table->forks[(i + 1) % table->nb_philo];
		table->philos_arr[i].table = table;

		table->philos_arr[i].death = &table->death;
	}
}


void		philo_init(int argc, char **argv, t_table *table)
{
	if (check_table(argc, argv, table))
		exit(1);
	init_table(table);
	init_philosopher(table);
}