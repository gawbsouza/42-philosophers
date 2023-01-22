/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:12:24 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 17:49:06 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	to_eat(t_philo *philo);
static void	to_sleep(t_philo *philo);
static void	to_think(t_philo *philo);

void	*philosopher(void *data)
{
	t_philo	*philo;

	philo = (t_philo *) data;
	while (!is_dead(philo) && !is_stopped(philo))
	{
		to_eat(philo);
		to_sleep(philo);
		to_think(philo);
	}
	return (NULL);
}

static void	to_eat(t_philo *philo)
{
	take_forks(philo);
	if (is_dead(philo) || is_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->philo_mutex);
	printf(EATING_MSG, time_millisec(), philo->number);
	philo->status = EATING;
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(philo->timer->eat_interv * 1000);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->meals++;
	philo->ate_at = time_millisec();
	pthread_mutex_unlock(&philo->philo_mutex);
	drop_forks(philo);
}

static void	to_sleep(t_philo *philo)
{
	if (is_dead(philo) || is_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->philo_mutex);
	printf(SLEEPING_MSG, time_millisec(), philo->number);
	philo->status = SLEEPING;
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(philo->timer->sleep_interv * 1000);
}

static void	to_think(t_philo *philo)
{
	if (is_dead(philo) || is_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->philo_mutex);
	printf(THINKING_MSG, time_millisec(), philo->number);
	philo->status = THINKING;
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(20 * 1000);
}
