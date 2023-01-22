/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 16:54:07 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


t_bool is_dead(t_philo *philo)
{
	t_bool died;

	pthread_mutex_lock(&philo->philo_mutex);
	died = philo->died;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (died);
}

t_bool is_stopped(t_philo *philo)
{
	t_bool stopped;
	
	pthread_mutex_lock(&philo->philo_mutex);
	stopped = philo->stopped;
	pthread_mutex_unlock(&philo->philo_mutex);
	return (stopped);
}

t_bool is_waiting_fork(t_philo *philo)
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

void	drop_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_mutex);
	pthread_mutex_lock(&philo->philo_mutex);
	philo->right_fork->available = TRUE;
	philo->right_fork->holded_by = 0;
	philo->left_fork->available = TRUE;
	philo->left_fork->holded_by = 0;
	pthread_mutex_unlock(&philo->philo_mutex);
	pthread_mutex_unlock(philo->fork_mutex);
}

void *philosopher(void *data) {

	t_philo *philo = (t_philo *) data;
	t_bool	waiting_fork;

	waiting_fork = TRUE;
	while(!is_dead(philo) && !is_stopped(philo)) {

		while (waiting_fork && !is_dead(philo) && !is_stopped(philo)) {

			pthread_mutex_lock(philo->fork_mutex);
			pthread_mutex_lock(&philo->philo_mutex);

			if (philo->left_fork->available) {
				philo->left_fork->available = FALSE;
				philo->left_fork->holded_by = philo->number;
				printf(FORK_MSG, time_millisec(), philo->number);
			}

			if (philo->right_fork && philo->right_fork->available) {
				philo->right_fork->available = FALSE;
				philo->right_fork->holded_by = philo->number;
				printf(FORK_MSG, time_millisec(), philo->number);
			}

			waiting_fork = is_waiting_fork(philo);

			pthread_mutex_unlock(&philo->philo_mutex);
			pthread_mutex_unlock(philo->fork_mutex);
		}

		if (!is_dead(philo) && !is_stopped(philo))
		{
			pthread_mutex_lock(&philo->philo_mutex);
			printf(EATING_MSG, time_millisec(), philo->number);
			philo->status = EATING;
			pthread_mutex_unlock(&philo->philo_mutex);
			usleep(philo->timer->eat_interv * 1000);

			pthread_mutex_lock(&philo->philo_mutex);
			philo->meals++;
			philo->ate_at = time_millisec();
			pthread_mutex_unlock(&philo->philo_mutex);
			drop_forks(philo);
			waiting_fork = TRUE;
		}
		
		if (!is_dead(philo) && !is_stopped(philo))
		{
			pthread_mutex_lock(&philo->philo_mutex);
			printf(SLEEPING_MSG, time_millisec(), philo->number);
			philo->status = SLEEPING;
			pthread_mutex_unlock(&philo->philo_mutex);
			usleep(philo->timer->sleep_interv * 1000);
		}

		if (!is_dead(philo) && !is_stopped(philo))
		{
			pthread_mutex_lock(&philo->philo_mutex);
			printf(THINKING_MSG, time_millisec(), philo->number);
			philo->status = THINKING;
			pthread_mutex_unlock(&philo->philo_mutex);
			usleep(20 * 1000);
		}
	}
	return (NULL);
}



int main(void) {

	t_ptimer		timer;
	t_table			table;
	t_monitor		dmonitor;

	timer.death_interv = 500;
	timer.eat_interv = 200;
	timer.sleep_interv = 200;
	table.meals_goal = 3;
	table.philos_num = 30;
	table.forks = (t_fork *) malloc(sizeof(t_fork) * table.philos_num);
	table.philos = (t_philo *) malloc(sizeof(t_philo) * table.philos_num);
	pthread_mutex_init(&table.fork_mutex, NULL);

	dmonitor.table = &table;


	// Inicializa os dados dos philosopher
	for(size_t i = 0; i < table.philos_num; i++)
	{
		table.forks[i].available = TRUE;
		table.forks[i].holded_by = 0;
		table.philos[i].number = i + 1;
		table.philos[i].died = FALSE;
		table.philos[i].stopped = FALSE;
		table.philos[i].began_at = time_millisec();
		table.philos[i].ate_at = 0;
		table.philos[i].died_at = 0;
		table.philos[i].stopped_at = 0;
		table.philos[i].meals = 0;
		table.philos[i].status = WAITING_FORK;
		table.philos[i].fork_mutex = &table.fork_mutex;
		table.philos[i].timer = &timer;
		table.philos[i].left_fork = &table.forks[i];
		if (i == table.philos_num - 1)
			table.philos[i].right_fork = &table.forks[0];
		else if (table.philos_num <= 1)
			table.philos[i].right_fork = NULL;
		else
			table.philos[i].right_fork = &table.forks[i + 1];

		pthread_mutex_init(&table.philos[i].philo_mutex, NULL);
	}

	// Inicializada threads;
	for(size_t i = 0; i < table.philos_num; i++)
	{
		pthread_create(&table.philos[i].thread, NULL, philosopher, (void *) &table.philos[i]);
	}

	pthread_create(&dmonitor.thread, NULL, monitor, (void *) &dmonitor);
	pthread_join(dmonitor.thread, NULL);



	summary(&table);
	// liberar memória
	free(table.philos);
	free(table.forks);

}