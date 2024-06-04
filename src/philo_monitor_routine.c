/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor_routine.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-03 19:32:14 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-03 19:32:14 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

/*
death_flagh_monitoring: this function is used to check in a thread safe way if
						the death variable was set to 1.
						In which case, the simulation must stop.
*/
int death_flagh_monitoring(t_table *table)
{
	pthread_mutex_lock(&table->death_lock);
	if (table->death)
	{
		pthread_mutex_unlock(&table->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->death_lock);
	return (0);
}

/*
monitor_helper: this function is used to check if a philosopher has died during
				each step of the routine. 
				If the death flag is set to 1, the function will unlock the
				philosopher's forks and return 1 (this will break the loop in
				the philosopher routine).*/
int monitor_helper(t_philo *philo, int flag)
{
	if ( flag == 0)
	{
		if (death_flagh_monitoring(philo->table))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (1);
		}
	}
	else if (flag == 1)
	{
		if (death_flagh_monitoring(philo->table))
			return (1);
	}
	return (0);
}


/*
check_meals: this function is used to check if a philosopher has eaten the
			 maximum number of meals.
1. lock the fed_philos_lock mutex: This is to prevent the main thread from 
   checking the number of fed philosophers while it is being modified by a 
   philosopher.
2. check if the philosopher has eaten the maximum number of meals and, if so:
   - print a message
     NOTE: The print_status function will set the death flag to 1
   - unlock the fed_philos_lock mutex
   - return 1
3. otherwise, increment the fed_philos variable.
*/
int check_meals(t_table *table)
{
	pthread_mutex_lock(&table->fed_philos_lock);								//1
	if (table->fed_philos == table->nb_philo)                                   //2
	{
		print_status(&table->philos_arr[1], "All philosophers are full");
		pthread_mutex_unlock(&table->fed_philos_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->fed_philos_lock);                              //3
	return (0);
}

/*
check_death: this function is used to check if a philosopher has died.
1. lock the im_eating_lock mutex: This is to prevent the main thread from 
   checking the last_meal_time variable while it is being modified by a 
   philosopher.
2. check if the philosopher has not eaten for a time greater than the death
   If the philosopher has died:
   - print a message
   - lock the death_lock mutex
   - set the death variable to 1
   - unlock the death_lock mutex
   - unlock the im_eating_lock mutex
   - return 1
3. otherwise, unlock the im_eating_lock mutex.
*/
int check_death(t_table *table, int i)
{
	pthread_mutex_lock(&table->philos_arr[i].im_eating_lock);                   //1
	if (get_current_time()
		- table->philos_arr[i].last_meal_time >= table->death_time)             //2
	{
		print_status(&table->philos_arr[i], "died");
		pthread_mutex_lock(&table->death_lock);
		table->death = 1;
		pthread_mutex_unlock(&table->death_lock);
		pthread_mutex_unlock(&table->philos_arr[i].im_eating_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->philos_arr[i].im_eating_lock);                 //3
	return (0);
}