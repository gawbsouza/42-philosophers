/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 16:55:50 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/26 00:38:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	philo_is_waiting_fork(t_philo *philo);
static void		take_fork(t_philo *philo, t_fork *fork);

void	philo_take_forks(t_philo *philo)
{
	t_bool	waiting_fork;

	waiting_fork = TRUE;
	while (waiting_fork)
	{
		usleep(200);
		pthread_mutex_lock(&philo->philo_mutex);
		if (!philo_can_run(philo))
		{
			pthread_mutex_unlock(&philo->philo_mutex);
			break ;
		}
		pthread_mutex_lock(philo->fork_mutex);
		take_fork(philo, philo->left_fork);
		if (philo->right_fork)
			take_fork(philo, philo->right_fork);
		waiting_fork = philo_is_waiting_fork(philo);
		pthread_mutex_unlock(&philo->philo_mutex);
		pthread_mutex_unlock(philo->fork_mutex);
	}
}

static void	take_fork(t_philo *philo, t_fork *fork)
{
	if (fork->available)
	{
		fork->available = FALSE;
		fork->holded_by = philo->number;
		printf(FORK_MSG, philo_running_time(philo), philo->number);
	}
}

static t_bool	philo_is_waiting_fork(t_philo *philo)
{
	t_bool	waiting;

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
	return (waiting);
}

void	philo_drop_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_mutex);
	philo->right_fork->available = TRUE;
	philo->right_fork->holded_by = 0;
	philo->left_fork->available = TRUE;
	philo->left_fork->holded_by = 0;
	pthread_mutex_unlock(philo->fork_mutex);
}
