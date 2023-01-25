/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:23:05 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/23 19:32:32 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	timer_init(t_ptimer *timer, long death, long eat, long sleep)
{
	timer->death_interv = death;
	timer->eat_interv = eat;
	timer->sleep_interv = sleep;
}

void	simulation_init(t_simulation *s, size_t philos_num, int meals_goal)
{
	size_t	i;

	s->meals_goal = meals_goal;
	s->philos_num = philos_num;
	s->forks = (t_fork *) malloc(sizeof(t_fork) * philos_num);
	s->philos = (t_philo *) malloc(sizeof(t_philo) * philos_num);
	pthread_mutex_init(&s->fork_mutex, NULL);
	i = 0;
	while (i < s->philos_num)
	{
		s->forks[i].available = TRUE;
		s->forks[i].holded_by = 0;
		i++;
	}
}

void	philosopher_init(t_philo *philo, size_t number)
{
	philo->number = number;
	philo->died = FALSE;
	philo->stopped = FALSE;
	philo->began_at = 0;
	philo->ate_at = 0;
	philo->died_at = 0;
	philo->stopped_at = 0;
	philo->meals = 0;
	philo->is_eating = FALSE;
	philo->fork_mutex = NULL;
	philo->timer = NULL;
	philo->left_fork = NULL;
	philo->right_fork = NULL;
	pthread_mutex_init(&philo->philo_mutex, NULL);
}

void	simulation_philos_init(t_simulation *simulation, t_ptimer *timer)
{
	size_t	i;

	i = 0;
	while (i < simulation->philos_num)
	{
		philosopher_init(&simulation->philos[i], i + 1);
		simulation->philos[i].fork_mutex = &simulation->fork_mutex;
		simulation->philos[i].timer = timer;
		simulation->philos[i].left_fork = &simulation->forks[i];
		if (simulation->philos_num <= 1)
			simulation->philos[i].right_fork = NULL;
		else if (i == simulation->philos_num - 1)
			simulation->philos[i].right_fork = &simulation->forks[0];
		else
			simulation->philos[i].right_fork = &simulation->forks[i + 1];
		i++;
	}
}

void	threads_init(t_simulation *simulation)
{
	pthread_t	monitor_thread;
	size_t		i;
	t_philo		*philo;
	long		start_time;

	start_time = time_millisec();
	i = 0;
	while (i < simulation->philos_num)
	{
		philo = &simulation->philos[i];
		philo->began_at = start_time;
		pthread_create(&philo->thread, NULL, philo_runner, (void *) philo);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor_runner, (void *) simulation);
	pthread_join(monitor_thread, NULL);
}
