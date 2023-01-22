/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_status_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:08:29 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 17:42:55 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_bool	is_dead(t_philo *philo)
{
	t_bool	died;

	pthread_mutex_lock(&philo->philo_mutex);
	died = philo->died;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (died);
}

t_bool	is_stopped(t_philo *philo)
{
	t_bool	stopped;

	pthread_mutex_lock(&philo->philo_mutex);
	stopped = philo->stopped;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (stopped);
}

t_bool	is_waiting_fork(t_philo *philo)
{
	t_bool	waiting;

	pthread_mutex_lock(philo->fork_mutex);
	pthread_mutex_lock(&philo->philo_mutex);
	if (!philo->right_fork)
		waiting = TRUE;
	else if (philo->left_fork->holded_by != philo->number)
		waiting = TRUE;
	else if (philo->right_fork->holded_by != philo->number)
		waiting = TRUE;
	else if (philo->left_fork->available)
		waiting = TRUE;
	else if (philo->right_fork->available)
		waiting = TRUE;
	else
		waiting = FALSE;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(philo->fork_mutex);
	return (waiting);
}
