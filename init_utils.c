/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 21:23:05 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 21:25:16 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	timer_init(t_ptimer *timer, long death, long eat, long sleep)
{
	timer->death_interv = death;
	timer->eat_interv = eat;
	timer->sleep_interv = sleep;
}

void	table_init(t_table *table, size_t philos_num, int meals_goal)
{
	table->meals_goal = meals_goal;
	table->philos_num = philos_num;
	table->forks = (t_fork *) malloc(sizeof(t_fork) * philos_num);
	table->philos = (t_philo *) malloc(sizeof(t_philo) * philos_num);
	pthread_mutex_init(&table->fork_mutex, NULL);
}

void	philosopher_init(t_philo *philo, size_t number)
{
	philo->number = number;
	philo->died = FALSE;
	philo->stopped = FALSE;
	philo->began_at = time_millisec();
	philo->ate_at = 0;
	philo->died_at = 0;
	philo->stopped_at = 0;
	philo->meals = 0;
	philo->status = WAITING_FORK;
	philo->fork_mutex = NULL;
	philo->timer = NULL;
	philo->left_fork = NULL;
	philo->right_fork = NULL;
	pthread_mutex_init(&philo->philo_mutex, NULL);
}

void	table_philos_init(t_table *table, t_ptimer *timer)
{
	size_t	i;

	i = 0;
	while (i < table->philos_num)
	{
		table->forks[i].available = TRUE;
		table->forks[i].holded_by = 0;
		philosopher_init(&table->philos[i], i + 1);
		table->philos[i].fork_mutex = &table->fork_mutex;
		table->philos[i].timer = timer;
		table->philos[i].left_fork = &table->forks[i];
		if (i == table->philos_num - 1)
			table->philos[i].right_fork = &table->forks[0];
		else if (table->philos_num <= 1)
			table->philos[i].right_fork = NULL;
		else
			table->philos[i].right_fork = &table->forks[i + 1];
		i++;
	}
}

void	threads_init(t_table *table)
{
	pthread_t	t;
	size_t		i;
	t_philo		*philo;

	i = 0;
	while (i < table->philos_num)
	{
		philo = &table->philos[i];
		pthread_create(&philo->thread, NULL, philosopher, (void *) philo);
		i++;
	}
	pthread_create(&t, NULL, monitor, (void *) table);
	pthread_join(t, NULL);
}
