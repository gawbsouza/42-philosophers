/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_runner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:14:12 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 19:19:27 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_bool	simulation_ended(t_simulation *simulation);
static void		update_philo_health(t_philo *philo);
static void		stop_philo(t_philo *philo);
static void		print_philo_death(t_philo *philo);

void	*monitor_runner(void *arg)
{
	t_simulation	*simulation;
	size_t			i;

	simulation = (t_simulation *) arg;
	usleep(10000);
	while (!simulation_ended(simulation))
		usleep(500);
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
		i++;
	}
	return (NULL);
}

static t_bool	simulation_ended(t_simulation *simulation)
{
	size_t	i;
	t_philo	*philo;
	size_t	philos_done;

	philos_done = 0;
	i = 0;
	while (i < simulation->philos_num)
	{
		philo = &simulation->philos[i];
		pthread_mutex_lock(&philo->philo_mutex);
		update_philo_health(philo);
		if (philo->died)
		{
			print_philo_death(philo);
			pthread_mutex_unlock(&philo->philo_mutex);
			return (TRUE);
		}
		if (philo->meals >= simulation->meals_goal)
			philos_done++;
		pthread_mutex_unlock(&philo->philo_mutex);
		i++;
	}
	if (!simulation->meals_goal)
		return (FALSE);
	return (philos_done >= simulation->philos_num);
}

static void	print_philo_death(t_philo *philo)
{
	long	death_time;

	death_time = time_millisec() - philo->began_at;
	printf(RED DIED_MSG RESET, death_time, philo->number);
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
