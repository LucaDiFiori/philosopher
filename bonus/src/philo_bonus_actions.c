/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-08 07:32:25 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-08 07:32:25 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher_bonus.h"

/*
Function used if there is only one philosopher. It will sleep for the death time
and then print the message DIED. After that, it will free the memory and exit.
*/
void single_philo(t_table *table)
{
	usleep(table->death_time * 1000);
	print_status(table, DIED);
	quit_and_free(table);
	return;
}

void	take_forks(t_table *table)
{
	sem_wait(table->sem_forks);
	print_status(table, FORK);
	sem_wait(table->sem_forks);
	print_status(table, FORK);
}

/*
1. print the status
2. lock the eating semaphore. This semaphore is used to avoid the death of a 
   philosopher while he is eating. If the philosopher is eating, the death_routine 
   will not be able to check the time of the last meal. 
   This semaphore is unlocked at the end of the function.
3. set the time of the last meal to the current time
4. if the philosopher has a maximum number of meals to eat, check if he has eaten 
   all the meals. If so, unlock the forks and the eating semaphore, and exit the 
   process with the value 2.
   NOTE: The usleep(100) is NECESSARY. I don't know why, couse i'm bad at coding.
5. unlock the eating semaphore
6. sleep for the eating time
7. unlock the forks*/
void	eat(t_table *table)
{
	print_status(table, EAT);													//1
	sem_wait(table->sem_eating);												//2
	table->last_meal = get_current_time();										//3
	if (table->max_meals != -1)													//4
	{
		table->meals_eaten++;
		if (table->meals_eaten == table->max_meals)
		{
			sem_post(table->sem_forks);
			sem_post(table->sem_forks);
			sem_post(table->sem_eating);
			usleep(100);
			exit(2);
		}
	}
	sem_post(table->sem_eating);												//5
	usleep(table->eat_time * 1000);												//6
	sem_post(table->sem_forks); 												//7
	sem_post(table->sem_forks);

}

void	sleep_routine(t_table *table)
{
	print_status(table, SLEEP);
	usleep(table->sleep_time * 1000);
}

void	think(t_table *table)
{
	print_status(table, THINK);
	if(table->nb_philo % 2 != 0)
		usleep(table->eat_time * 1000);
}