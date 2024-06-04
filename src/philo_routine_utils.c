/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-05-30 07:28:02 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-05-30 07:28:02 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

/*
philosopher_waiting: this function is used to make sure that all the philosophers 
                     are ready to start at the same time.

1. Lock the ready_lock: this mutex is used to make sure that just one philosopher 
   at a time can increment the ready_count. 
2. Increment the ready_count: this variable is used to keep track of how many 
                              philosophers are ready to start.

3. waiting cicle: this while loop is used to make sure that all the philosophers 
				  are ready to start.
				  If the ready_count is less than the number of philosophers, 
				  we temporarily unlock the ready_lock and wait for 100 microseconds
				  to let the other philosophers increment the ready_count.
				  Than we lock the ready_lock again and check if all the philosophers
				  are ready to start.
4. Unlock the ready_lock: once all the philosophers are ready to start, we unlock the 
						   ready_lock and let the philosophers start.

NOTE: the 100 microseconds wait is used to avoid a busy waiting loop and reduce the 
	  CPU usage.*/
void	philosopher_getting_ready(t_table *table)
{
	pthread_mutex_lock(&table->ready_lock);										//1
	table->ready_count++;                                                       //2
	while (table->ready_count < table->nb_philo)                                //3
	{
		pthread_mutex_unlock(&table->ready_lock);
		usleep(100);
		pthread_mutex_lock(&table->ready_lock);
	}
	pthread_mutex_unlock(&table->ready_lock);									//4
}



/*
take_forks: this function is used to make sure that the philosophers take the 
			forks in the right order. If the philosopher has an even id, he takes
			the right fork first, otherwise he takes the left fork first.

2. Check if the philosopher has an even id: if the id is even, the philosopher
   Lock the right fork mutex in the array of forks
   Print the status of the philosopher
   Lock the left fork mutex in the array of forks
   Print the status of the philosopher

3. If the philosopher has an odd id, do the opposite of the previous step.*/
void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)                                                     //1
    {
        pthread_mutex_lock(philo->right_fork);                            		//2
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);
        print_status(philo, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);                             		//3
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);
        print_status(philo, "has taken a fork");
    }
}



/*
eat_routine: this function is used to simulate the eating routine of the philosopher.

1. print the status of the philosopher
2. lock the im_eating_lock: this mutex is used to prevent the main thread from 
   checking the last_meal_time variable before it is modified by the philosopher.
   This mutex ensures that no philosopher will die while eating.
3. update the last_meal_time variable: this variable is used to keep track of the
                                       last time the philosopher ate.
4. increment the meals_eaten variable: this variable is used to keep track of how
									   many meals the philosopher has eaten.
5. check if the philosopher has eaten the maximum number of meals: if the
   philosopher has eaten the maximum number of meals, we lock the fed_philos_lock
   and increment the fed_philos variable (is a table variable, common to all the
   philosophers). This variable is used to keep track of how many philosophers have
   eaten the maximum number of meals.
6. unlock the im_eating_lock: once the philosopher has eaten, we unlock the
	                          im_eating_lock.
7. sleep for the eat_time: this is the time the philosopher takes to eat.
8. unlock the right fork mutex in the array of forks
   unlock the left fork mutex in the array of forks*/
void	eat_routine(t_philo *philo)
{
	print_status(philo, "is eating");                                           //1

	pthread_mutex_lock(&philo->im_eating_lock);                          		//2                        
	philo->last_meal_time = get_current_time();                                 //3
	philo->meals_eaten++;													    //4		
	if (philo->meals_eaten == philo->table->max_meals)                          //5
	{
		pthread_mutex_lock(&philo->table->fed_philos_lock);
		philo->table->fed_philos++;
		pthread_mutex_unlock(&philo->table->fed_philos_lock);
	}
	pthread_mutex_unlock(&philo->im_eating_lock);                        		//6
	usleep(philo->table->eat_time * 1000);                                      //7
	pthread_mutex_unlock(philo->right_fork);                           			//8
	pthread_mutex_unlock(philo->left_fork);
}




/*
sleep_routine: this function is used to simulate the sleeping routine of the 
               philosopher. Simply print the status of the philosopher and sleep
			   for the sleep_time.*/
void	sleep_routine(t_philo *philo)
{
	print_status(philo, "is sleeping");
	usleep(philo->table->sleep_time * 1000);
}



/*
think_routine: this function is used to simulate the thinking routine of the 
			   philosopher.

			   NOTE: In this routine, if we have an odd number of philosophers, 
			         we introduce a delay for philosophers with odd IDs to help 
					 ensure that philosophers with even IDs eat first. 
					 This helps prevent deadlock, where philosophers with even 
					 IDs could be stuck waiting for the right fork if odd-ID 
					 philosophers eat first.*/
void	think_routine(t_philo *philo)
{
	print_status(philo, "is thinking");
	if (philo->table->nb_philo % 2 != 0)
		usleep(philo->table->eat_time * 1000);
}
