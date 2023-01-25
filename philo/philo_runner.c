/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_runner.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:12:24 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 10:34:57 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	to_eat(t_philo *philo);
static void	to_sleep(t_philo *philo);
static void	to_think(t_philo *philo);

void	*philo_runner(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *) arg;
	while (!philo_is_dead(philo) && !philo_is_stopped(philo))
	{
		to_eat(philo);
		to_sleep(philo);
		to_think(philo);
	}
	return (NULL);
}

static void	to_eat(t_philo *philo)
{
	philo_take_forks(philo);
	if (philo_is_dead(philo) || philo_is_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->philo_mutex);
	printf(EATING_MSG, philo_running_time(philo), philo->number);
	philo->is_eating = TRUE;
	pthread_mutex_unlock(&philo->philo_mutex);
	philo->ate_at = time_millisec();
	usleep(philo->timer->eat_interv * 1000);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->meals++;
	philo->is_eating = FALSE;
	pthread_mutex_unlock(&philo->philo_mutex);
	philo_drop_forks(philo);
}

static void	to_sleep(t_philo *philo)
{
	if (philo_is_dead(philo) || philo_is_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->philo_mutex);
	printf(SLEEPING_MSG, philo_running_time(philo), philo->number);
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(philo->timer->sleep_interv * 1000);
}

static void	to_think(t_philo *philo)
{
	if (philo_is_dead(philo) || philo_is_stopped(philo))
		return ;
	pthread_mutex_lock(&philo->philo_mutex);
	printf(THINKING_MSG, philo_running_time(philo), philo->number);
	pthread_mutex_unlock(&philo->philo_mutex);
	usleep(20 * 1000);
}
