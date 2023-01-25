/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_runner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:12:24 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/26 00:12:28 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	to_eat(t_philo *philo);
static t_bool	to_sleep(t_philo *philo);
static t_bool	to_think(t_philo *philo);

void	*philo_runner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (TRUE)
	{
		if (!to_eat(philo))
			break ;
		if (!to_sleep(philo))
			break ;
		if (!to_think(philo))
			break ;
	}
	return (NULL);
}

static t_bool	to_eat(t_philo *philo)
{
	philo_take_forks(philo);
	pthread_mutex_lock(&philo->philo_mutex);
	if (!philo_can_run(philo))
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		return (FALSE);
	}
	printf(EATING_MSG, philo_running_time(philo), philo->number);
	philo->is_eating = TRUE;
	philo->ate_at = time_millisec();
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(philo->timer->eat_interv * 1000);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->meals++;
	philo->is_eating = FALSE;
	philo_drop_forks(philo);
	pthread_mutex_unlock(&philo->philo_mutex);
	return (TRUE);
}

static t_bool	to_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	if (!philo_can_run(philo))
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		return (FALSE);
	}
	printf(SLEEPING_MSG, philo_running_time(philo), philo->number);
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(philo->timer->sleep_interv * 1000);
	return (TRUE);
}

static t_bool	to_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	if (!philo_can_run(philo))
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		return (FALSE);
	}
	printf(THINKING_MSG, philo_running_time(philo), philo->number);
	pthread_mutex_unlock(&philo->philo_mutex);
	return (TRUE);
}
