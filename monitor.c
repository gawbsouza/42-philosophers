/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:14:12 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 16:46:03 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		update_philos_health(t_table *table);
static t_bool	meals_goal_reached(t_table *table);
static t_bool	has_philo_dead(t_table *table);
static void		stop_philo(t_philo *philo);

void	*monitor(void *data)
{
	t_monitor	*monitor;
	t_table		*table;
	size_t		i;

	monitor = (t_monitor *) data;
	table = monitor->table;
	while (!has_philo_dead(table) && !meals_goal_reached(table))
		update_philos_health(table);
	i = 0;
	while (i < table->philos_num)
	{
		stop_philo(&table->philos[i]);
		i++;
	}
	i = 0;
	while (i < table->philos_num)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
	return (NULL);
}

static void	update_philos_health(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->philos_num)
	{
		update_philo_health(&table->philos[i]);
		i++;
	}
}

static t_bool	meals_goal_reached(t_table *table)
{
	size_t	i;
	size_t	reached_count;

	reached_count = 0;
	i = 0;
	while (i < table->philos_num)
	{
		pthread_mutex_lock(&table->philos[i].philo_mutex);
		if (table->philos[i].meals >= table->meals_goal)
				reached_count++;
		pthread_mutex_unlock(&table->philos[i].philo_mutex);
		i++;
	}
	return ((table->meals_goal && reached_count >= table->philos_num));
}

static t_bool	has_philo_dead(t_table *table)
{
	size_t	i;

	i = 0;
	while (i < table->philos_num)
	{
		if (is_dead(&table->philos[i]))
		{
			printf(DIED_MSG, time_millisec(), table->philos[i].number);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

static void	stop_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->stopped = TRUE;
	philo->stopped_at = time_millisec();
	pthread_mutex_unlock(&philo->philo_mutex);
}
