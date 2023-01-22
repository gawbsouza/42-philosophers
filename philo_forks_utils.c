/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forks_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 17:25:09 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 17:46:04 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	t_bool	waiting_fork;

	waiting_fork = (!philo->ate_at || is_waiting_fork(philo));
	while (waiting_fork)
	{
		if (is_dead(philo) || is_stopped(philo))
			break ;
		pthread_mutex_lock(philo->fork_mutex);
		pthread_mutex_lock(&philo->philo_mutex);
		if (philo->left_fork->available)
		{
			philo->left_fork->available = FALSE;
			philo->left_fork->holded_by = philo->number;
			printf(FORK_MSG, time_millisec(), philo->number);
		}
		if (philo->right_fork && philo->right_fork->available)
		{
			philo->right_fork->available = FALSE;
			philo->right_fork->holded_by = philo->number;
			printf(FORK_MSG, time_millisec(), philo->number);
		}
		pthread_mutex_unlock(&philo->philo_mutex);
		pthread_mutex_unlock(philo->fork_mutex);
		waiting_fork = is_waiting_fork(philo);
	}
}

void	drop_forks(t_philo *philo)
{
	if (is_dead(philo) || is_stopped(philo))
		return ;
	pthread_mutex_lock(philo->fork_mutex);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->right_fork->available = TRUE;
	philo->right_fork->holded_by = 0;
	philo->left_fork->available = TRUE;
	philo->left_fork->holded_by = 0;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(philo->fork_mutex);
}
