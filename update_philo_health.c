/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_philo_health.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:48:22 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 16:51:57 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	update_philo_health(t_philo *philo)
{
	long	compare_time;
	long	current_time;

	current_time = time_millisec();
	pthread_mutex_lock(&philo->philo_mutex);
	if (philo->status == EATING)
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
