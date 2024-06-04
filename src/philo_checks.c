/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldi-fior <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 16:25:12 by ldi-fior          #+#    #+#             */
/*   Updated: 2024/05/29 16:25:12 by ldi-fior         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosopher.h"

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

int	check_table(int argc, char **argv, t_table *table)
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