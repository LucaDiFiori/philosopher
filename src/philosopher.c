/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:22:56 by ldi-fior          #+#    #+#             */
/*   Updated: 2024/05/29 16:22:56 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

static void single_philosopher (t_philo *philo)
{
	if (philo->table->nb_philo == 1)
	{
		print_status(philo, "is thinking");
		usleep(philo->table->death_time * 1000);
	}
}


/*
This is the routine of the philosopher thread.
1. wait for all the threads to be ready
2. if the philosopher id is even, sleep for 500 milliseconds
   (this is to prevent neighboring philosophers from taking the same fork 
   at the same time)
3. infinite loop to simulate the life of the philosopher
   - take the forks
   - check the death flag
   - eat
   - check the death flag
   - sleep
   - check the death flag
   - think
   - check the death flag*/
static void *philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo *)arg;
	philosopher_getting_ready(philo->table);                                    //1
	if (philo->id % 2 == 0)                                                     //2
		usleep(500);
	single_philosopher(philo);
	while (!death_flagh_monitoring(philo->table))								//3
	{
		take_forks(philo);
		if (monitor_helper(philo, 0))
			break;
		eat_routine(philo);
		if (monitor_helper(philo, 1))
			break;
		sleep_routine(philo);
		if (monitor_helper(philo, 1))
			break;
		think_routine(philo);
		if (monitor_helper(philo, 1))
			break;
	}
	return (NULL);
}


/*
start_philosopher: this function is used to start the philosopher threads.*/
static void	start_or_join(t_table *table, int flag)
{
	int i;

	i = -1;
	if (flag == 0)
	{
		while (++i < table->nb_philo)
		{
			if (pthread_create(&table->philos_arr[i].thread, NULL, philo_routine,
			&table->philos_arr[i]))
				free_and_quit(table, PT_CREATE, 2);
		}
	}
	else 
	{
		while (++i < table->nb_philo)
		{
			if (pthread_join(table->philos_arr[i].thread, NULL))
				free_and_quit(table, PT_JOIN, 2);
		}
	}
}


/*
1. initialize the table struct
2. start the philosopher threads
3. infinite loop to check if a philosopher has died or if all the philosophers 
   have eaten the maximum number of meals
4. for each philosopher, check if the philosopher has died or if all the 
   philosophers have eaten the maximum number of meals.

   If a philosopher has died or all the philosophers have eaten the maximum
   number of meals, break the loop.
   
5. check if a philosopher has died or if all the philosophers have eaten the
   maximum number of meals. If so, break the loop.

6. check the death flag of the table. If the death flag is set to 1, break the loop.

7. join the philosopher threads
8. free the memory and quit the program.
*/
int main(int argc, char **argv)
{
	t_table	table;
	int i;

	philo_init(argc, argv, &table);												//1
	start_or_join(&table, 0);													//2
	while (1)											                        //3
	{
		i = -1;
		while(++i < table.nb_philo)                                             //4
		{
			if (table.max_meals == -1)
			{ 	
				if (check_death(&table, i))
					break;		
			}
			else
			{   
				if (check_meals(&table) || check_death(&table, i))              //5
					break;
			}
		}
		if (death_flagh_monitoring(&table))                                     //6
			break;
	}
	start_or_join(&table, 1);													//7
	free_and_quit(&table, NULL, 3);												//8
}