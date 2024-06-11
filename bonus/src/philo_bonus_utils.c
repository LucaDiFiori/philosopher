/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-07 10:06:41 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-07 10:06:41 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher_bonus.h"

size_t	get_current_time()
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
	{
		ft_putstr_fd("Error: gettimeofday failed\n", 2);
		exit (-1);
	}
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void	print_status(t_table *table, const char *action)
{
	size_t	time;
	
	sem_wait(table->sem_print);
	time = get_current_time() - table->start_time;
	printf("%zu %d %s\n", time, table->id, action);
	sem_post(table->sem_print);
}

int check_death_flag(t_table *table)
{
	sem_wait(table->sem_death);
	if (table->death_flag == 1)
	{
		sem_post(table->sem_death);
		return (1);
	}
	sem_post(table->sem_death);
	return (0);
}

void	quit_and_free(t_table *table)
{
	sem_close(table->sem_forks);
	sem_close(table->sem_print);
	sem_close(table->sem_eating);
	sem_close(table->sem_death);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_IM_EATIN);
	sem_unlink(SEM_DEATH);
	free(table->philo_pid);
}

void modify_death_flag(t_table *table)
{
	sem_wait(table->sem_death);
	table->death_flag = 1;
	sem_post(table->sem_death);
}

