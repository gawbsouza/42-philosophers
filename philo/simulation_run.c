/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_run.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 09:23:14 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 19:19:55 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	initialize_forks(t_simulation *simulation);
static void	initialize_philos(t_simulation *simulation);
static void	start_theads(t_simulation *simulation);
static void	philo_init(t_philo *philo, size_t number);

void	simulation_run(t_simulation *simulation, t_bool show_summary)
{
	initialize_forks(simulation);
	initialize_philos(simulation);
	start_theads(simulation);
	if (show_summary)
		summary(simulation);
	free(simulation->philos);
	free(simulation->forks);
}

static void	initialize_philos(t_simulation *simulation)
{
	size_t	i;
	t_philo	*philo;
	long	start_time;

	simulation->philos = malloc(sizeof(t_philo) * simulation->philos_num);
	i = 0;
	start_time = time_millisec();
	while (i < simulation->philos_num)
	{
		philo = &simulation->philos[i];
		philo_init(philo, i + 1);
		philo->began_at = start_time;
		philo->fork_mutex = &simulation->fork_mutex;
		philo->timer = &simulation->timer;
		philo->left_fork = &simulation->forks[i];
		if (simulation->philos_num <= 1)
			philo->right_fork = NULL;
		else if (i == simulation->philos_num - 1)
			philo->right_fork = &simulation->forks[0];
		else
			philo->right_fork = &simulation->forks[i + 1];
		i++;
	}
}

static void	initialize_forks(t_simulation *simulation)
{
	size_t	i;

	simulation->forks = malloc(sizeof(t_fork) * simulation->philos_num);
	i = 0;
	while (i < simulation->philos_num)
	{
		simulation->forks[i].available = TRUE;
		simulation->forks[i].holded_by = 0;
		i++;
	}
}

static void	start_theads(t_simulation *simulation)
{
	pthread_t	monitor_thread;
	size_t		i;
	t_philo		*philo;

	pthread_mutex_init(&simulation->fork_mutex, NULL);
	i = 1;
	while (i < simulation->philos_num)
	{
		philo = &simulation->philos[i];
		pthread_create(&philo->thread, NULL, philo_runner, (void *) philo);
		i += 2;
	}
	i = 0;
	usleep(500);
	while (i < simulation->philos_num)
	{
		philo = &simulation->philos[i];
		pthread_create(&philo->thread, NULL, philo_runner, (void *) philo);
		i += 2;
	}
	pthread_create(&monitor_thread, NULL, monitor_runner, (void *) simulation);
	pthread_join(monitor_thread, NULL);
}

static void	philo_init(t_philo *philo, size_t number)
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
