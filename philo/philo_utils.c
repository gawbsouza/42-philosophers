/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:48:22 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 10:33:00 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	philo_running_time(t_philo *philo)
{
	return (time_millisec() - philo->began_at);
}

void	philo_update_health(t_philo *philo)
{
	long	compare_time;
	long	current_time;

	current_time = time_millisec();
	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->is_eating)
	{
		pthread_mutex_unlock(&philo->philo_mutex);
		return ;
	}
	if (!philo->ate_at)
		compare_time = philo->began_at;
	else
		compare_time = philo->ate_at;
	philo->died = (current_time - compare_time >= philo->timer->death_interv);
	if (philo->died)
		philo->died_at = current_time;
	pthread_mutex_unlock(&philo->philo_mutex);
}

void	philo_take_forks(t_philo *philo)
{
	t_bool	waiting_fork;

	waiting_fork = (!philo->ate_at || philo_is_waiting_fork(philo));
	while (waiting_fork)
	{
		if (philo_is_dead(philo) || philo_is_stopped(philo))
			break ;
		pthread_mutex_lock(philo->fork_mutex);
		pthread_mutex_lock(&philo->philo_mutex);
		if (philo->left_fork->available)
		{
			philo->left_fork->available = FALSE;
			philo->left_fork->holded_by = philo->number;
			printf(FORK_MSG, philo_running_time(philo), philo->number);
		}
		if (philo->right_fork && philo->right_fork->available)
		{
			philo->right_fork->available = FALSE;
			philo->right_fork->holded_by = philo->number;
			printf(FORK_MSG, philo_running_time(philo), philo->number);
		}
		pthread_mutex_unlock(&philo->philo_mutex);
		pthread_mutex_unlock(philo->fork_mutex);
		waiting_fork = philo_is_waiting_fork(philo);
	}
}

void	philo_drop_forks(t_philo *philo)
{
	if (philo_is_dead(philo) || philo_is_stopped(philo))
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
