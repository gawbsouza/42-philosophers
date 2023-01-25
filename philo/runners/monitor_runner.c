/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_runner.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:14:12 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 08:27:45 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void		update_philos_health(t_simulation *simulation);
static t_bool	meals_goal_reached(t_simulation *simulation);
static t_bool	has_philo_dead(t_simulation *simulation);
static void		stop_philo(t_philo *philo);

void	*monitor_runner(void *arg)
{
	t_simulation	*simulation;
	size_t			i;
	long			death_time;
	t_philo			*philo;

	simulation = (t_simulation *) arg;
	while (!has_philo_dead(simulation) && !meals_goal_reached(simulation))
		update_philos_health(simulation);
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
		death_time = philo->died_at - philo->began_at;
		if (simulation->philos[i].died)
			printf(RED DIED_MSG RESET, death_time, philo->number);
		i++;
	}
	return (NULL);
}

static void	update_philos_health(t_simulation *simulation)
{
	size_t	i;

	i = 0;
	while (i < simulation->philos_num)
	{
		update_philo_health(&simulation->philos[i]);
		i++;
	}
}

static t_bool	meals_goal_reached(t_simulation *s)
{
	size_t	i;
	size_t	reached_count;

	reached_count = 0;
	i = 0;
	while (i < s->philos_num)
	{
		pthread_mutex_lock(&s->philos[i].philo_mutex);
		if (s->philos[i].meals >= s->meals_goal)
				reached_count++;
		pthread_mutex_unlock(&s->philos[i].philo_mutex);
		i++;
	}
	return ((s->meals_goal && reached_count >= s->philos_num));
}

static t_bool	has_philo_dead(t_simulation *s)
{
	size_t	i;

	i = 0;
	while (i < s->philos_num)
	{
		if (is_dead(&s->philos[i]))
			return (TRUE);
		i++;
	}
	return (FALSE);
}

static void	stop_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->stopped = TRUE;
	philo->stopped_at = time_millisec();
	pthread_mutex_unlock(&philo->philo_mutex);
}
