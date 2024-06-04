/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_err_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-01 07:39:04 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-01 07:39:04 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"


static void heap_destroyer(t_table *table)
{
	if (table->forks)
		free(table->forks);
	if (table->philos_arr)
		free(table->philos_arr);
}

static void	free_philo (t_table *table)
{
	pthread_mutex_destroy(&table->print_lock);
	pthread_mutex_destroy(&table->ready_lock);
	pthread_mutex_destroy(&table->fed_philos_lock);
	pthread_mutex_destroy(&table->death_lock);
	for(int i = 0; i < table->nb_philo; i++)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos_arr[i].im_eating_lock);
	}
	heap_destroyer(table);
}

void    free_and_quit(t_table *table, char *err, int err_type)
{
	if (err_type == 1)
	{
		heap_destroyer(table);
		ft_putstr_fd(err, 2);
	}
	else if (err_type == 2)
	{
		ft_putstr_fd(err, 2);
		free_philo(table);
	}
	else if (err_type == 3)
	{
		free_philo(table);
		return ;
	}
	exit(-1);
}