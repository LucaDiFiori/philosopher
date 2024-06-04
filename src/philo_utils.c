/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:00:02 by ldi-fior          #+#    #+#             */
/*   Updated: 2024/05/29 17:00:02 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

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


void    print_status(t_philo *philo, const char *action)
{
    size_t time;
    
    pthread_mutex_lock(&philo->table->print_lock);
    if (!death_flagh_monitoring(philo->table))
    {
        if (ft_strncmp_philo(action, "All philosophers are full") == 0)
        {
            pthread_mutex_lock(&philo->table->death_lock);
		    philo->table->death = 1;
		    pthread_mutex_unlock(&philo->table->death_lock);
            printf("All philosophers are full\n");
            pthread_mutex_unlock(&philo->table->print_lock);
            return ;
        }
        else
        {
            time = get_current_time() - philo->table->start_time;
            printf("%ld %d %s\n", time, philo->id, action);
            pthread_mutex_unlock(&philo->table->print_lock);
            return ;
        }
    }
    pthread_mutex_unlock(&philo->table->print_lock);
}