/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_runner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:14:12 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 17:10:56 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	simulation_ended(t_simulation *simulation);
static void		update_philo_health(t_philo *philo);
static void		stop_philo(t_philo *philo);

void	*monitor_runner(void *arg)
{
	t_simulation	*simulation;
	size_t			i;
	long			death_time;
	t_philo			*philo;

	simulation = (t_simulation *) arg;
	while (!simulation_ended(simulation))
		;
	i = 0;
	while (i < simulation->philos_num)
	{
		stop_philo(&simulation->philos[i]);
		i++;
	}
	i = 0;
	while (i < simulation->philos_num)
	{
		pthread_join(simulation->philos[i].thread, NULL);
		philo = &simulation->philos[i];
		death_time = time_millisec() - philo->began_at;
		if (simulation->philos[i].died)
			printf(RED DIED_MSG RESET, death_time, philo->number);
		i++;
	}
	return (NULL);
}

static t_bool	simulation_ended(t_simulation *simulation)
{
	size_t	i;
	size_t	philos_done;

	philos_done = 0;
	i = 0;
	while (i < simulation->philos_num)
	{
		pthread_mutex_lock(&simulation->philos[i].philo_mutex);
		update_philo_health(&simulation->philos[i]);
		if (simulation->philos[i].died)
		{
			pthread_mutex_unlock(&simulation->philos[i].philo_mutex);
			return (TRUE);
		}
		if (simulation->philos[i].meals >= simulation->meals_goal)
				philos_done++;
		pthread_mutex_unlock(&simulation->philos[i].philo_mutex);
		i++;
	}
	return (simulation->meals_goal && (philos_done >= simulation->philos_num));
}

static void	update_philo_health(t_philo *philo)
{
	long	compare_time;
	long	current_time;

	current_time = time_millisec();
	if (philo->is_eating)
	{
		return ;
	}
	if (!philo->ate_at)
		compare_time = philo->began_at;
	else
		compare_time = philo->ate_at;
	philo->died = (current_time - compare_time >= philo->timer->death_interv);
	if (philo->died)
		philo->died_at = current_time;
}

static void	stop_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->stopped = TRUE;
	philo->stopped_at = time_millisec();
	pthread_mutex_unlock(&philo->philo_mutex);
}
