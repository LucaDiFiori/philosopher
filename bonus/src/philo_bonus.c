/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-06-08 17:16:43 by ldi-fior          #+#    #+#             */
/*   Updated: 2024-06-08 17:16:43 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher_bonus.h"


/*
This function checks if the philosopher has died. If so, it prints the message DIED
and returns 1. Otherwise, it returns 0.

1. lock the eating semaphore. This semaphore is used to avoid the death of a 
   philosopher while he is eating.
2. check if the time of the last meal is greater than the death time.
   If so:
   2.1 print the message DIED
   2.2 lock permanently the print semaphore until the process exits.
       This is done to avoid the print of the message DIED multiple times.
	   NOTE: The System hendles the release of the semaphore when the process exits.
   2.3 unlock the eating semaphore
   2.4 set the death_flag to 1
   2.5 return 1

else
3. unlock the eating semaphore and return 0
*/
int	death_routine(t_table *table)
{

		sem_wait(table->sem_eating);											//1
		if (get_current_time() - table->last_meal > table->death_time)			//2
		{
			print_status(table, DIED);											//2.1

			sem_wait(table->sem_print);											//2.2
			
			sem_post(table->sem_eating);										//2.3
			modify_death_flag(table);											//2.4

			return (1);															//2.5
		}	
		else																	//3											
		{
			sem_post(table->sem_eating);
			return(0);
		}
}


/*
This is the main function executed by the child processes.
It will execute the following steps until the death_flag is set to 1:
1. take the forks
2. if the death_flag is set to 1, unlock the forks and break the loop
3. eat
4. if the death_flag is set to 1, break the loop
5. sleep
6. if the death_flag is set to 1, break the loop
7. think
8. if the death_flag is set to 1, break the loop
9 call the quit_and_free function and exit the process with the value 1

NOTE: Every process will clean up the memory and close the semaphores before exiting.
      This is becouse every process has its own copy of the semaphores and the memory.*/
static void philo_p(t_table *table)
{
	while (!check_death_flag(table))											
	{
		take_forks(table);														//1
		if (death_routine(table))												//2
		{
			sem_post(table->sem_forks);
			sem_post(table->sem_forks);
			break;
		}
		eat(table);																//3	
		if (death_routine(table))												//4
			break;
		sleep_routine(table);													//5
		if (death_routine(table))												//6			
			break;
		think(table);															//7								
		if (death_routine(table))												//8
			break;
	}
	quit_and_free(table);														//9
	exit(1);
}


/*This sends a SIGKILL signal to all the child processes.*/
static void kill_the_childrens(t_table *table)
{
	int j;

	modify_death_flag(table);
	j = -1;
	while (++j < table->nb_philo)
		kill(table->philo_pid[j], SIGKILL);
}


/*
This is the monitor function run by the parent process. 
It waits for a signal from the child processes (send with the kill function)
and checks the status of the child processes.

1. for each philosopher:
   1.1 wait for the child process to terminate
   1.2 if the child process has exited with the value 1, kill all the child processes 
	   and return
   1.3 if the child process has exited with the value 2, increment the number of fed 
	   philosophers. If all the philosophers have eaten, print the message ALL_FED and 
	   return*/
static void monitor_p(t_table *table)
{
	int status = 0;
	int i;

	i = -1;
	while (++i < table->nb_philo)												//1
	{
		waitpid(-1, &status, 0);											    //1.1							
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)                      //1.2
		{
			kill_the_childrens(table);
			return;
		}
		if (WIFEXITED(status) && WEXITSTATUS(status) == 2)                      //1.3
		{
			table->fed_philos++;
			if (table->fed_philos == table->nb_philo)
			{
				sem_wait(table->sem_print);
				printf("%s\n", ALL_FED);
				sem_post(table->sem_print);
				return;
			}
		}
	}
}


/*
The main function creates a new process for each philosopher. Each child process will
execute the philo_p function. The parent process will execute the monitor_p function.

1. initialize the table structure
2. for each philosopher:
   2.1 create a new process
   2.2 if the process is a child process
	   2.2.1 set the id of the philosopher
	   2.2.2 set the time of the last meal to the current time
	   2.2.3 if there is only one philosopher, execute the single_philo function
	   2.2.4 execute the philo_p function
3. if the process is the parent process, execute the monitor_p function
4. free the memory and close the semaphores*/
int main(int argc, char **argv)
{
	t_table table;
	int i;

	philo_init(argc, argv, &table);												//1
	i = -1;
	while(++i < table.nb_philo)													//2
	{
		table.philo_pid[i] = fork();											//2.1
		if(table.philo_pid[i] == -1)
			exit(1);
		if (table.philo_pid[i] == 0)											//2.2
		{
			table.id = i + 1;													//2.2.1
			table.last_meal = get_current_time();								//2.2.2
			if (table.nb_philo == 1)											//2.2.3
			{
				single_philo(&table);
				return (0);
			}
			philo_p(&table);													//2.2.4			
		}
	}
	monitor_p(&table);															//3				
	quit_and_free(&table);														//4
}