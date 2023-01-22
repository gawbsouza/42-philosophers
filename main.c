/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 09:25:51 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_millisec(void)
{
	struct timeval	current_time;

	if(gettimeofday(&current_time, NULL))
		return (-1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void check_death(t_philo *philo)
{
	long	compare_time;

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
	philo->died = (time_millisec() - compare_time >= philo->timer->death_interv);
	// printf("time differ %ld\n", time_millisec() - compare_time);
	pthread_mutex_unlock(&philo->philo_mutex);
}

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

void stop(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	philo->stopped = TRUE;
	pthread_mutex_unlock(&philo->philo_mutex);
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

void	*monitor(void *data)
{
	t_monitor *monitor = (t_monitor *) data;
	t_bool end;

	end = FALSE;
	while (!end)
	{
		for(int i = 0; i < monitor->philos_num; i++)
		{
			check_death(&monitor->philos[i]);
			if (is_dead(&monitor->philos[i]))
			{
				printf(DIED_MSG, time_millisec(), monitor->philos[i].number);
				end = TRUE;
				for(int x = 0; x < monitor->philos_num; x++)
				{
					stop(&monitor->philos[x]);
				}
				break;
			}
		}
	}
	for(int x = 0; x < monitor->philos_num; x++)
	{
		pthread_join(monitor->philos[x].thread, NULL);
		printf("philo %ld ended\n", monitor->philos[x].number);
	}
	return NULL;
}

int main(void) {

	pthread_mutex_t	fork_mutex;
	int				philos_num;
	t_fork			*forks;
	t_philo			*philos;
	t_ptimer		timer;
	t_monitor		monitor_data;

	philos_num = 5;
	forks = (t_fork *) malloc(sizeof(t_fork) * philos_num);
	philos = (t_philo *) malloc(sizeof(t_philo) * philos_num);


	timer.death_interv = 800;
	timer.eat_interv = 200;
	timer.sleep_interv = 200;

	// Inicializa os dados dos philosopher
	for(int i = 0; i < philos_num; i++)
	{
		forks[i].available = TRUE;
		forks[i].holded_by = 0;

		philos[i].number = i + 1;
		philos[i].died = FALSE;
		philos[i].stopped = FALSE;
		philos[i].began_at = time_millisec();
		philos[i].ate_at = 0;
		philos[i].meals = 0;
		philos[i].status = WAITING_FORK;
		philos[i].fork_mutex = &fork_mutex;
		philos[i].timer = &timer;
		philos[i].left_fork = &forks[i];
		if (i == philos_num - 1)
			philos[i].right_fork = &forks[0];
		else if (philos_num <= 1)
			philos[i].right_fork = NULL;
		else
			philos[i].right_fork = &forks[i + 1];
	}

	// Inicializada threads;
	for(int i = 0; i < philos_num; i++)
	{
		pthread_create(&philos[i].thread, NULL, philosopher, (void *) &philos[i]);
	}

	monitor_data.meals_goal = 0;
	monitor_data.philos = philos;
	monitor_data.philos_num = philos_num;

	pthread_create(&monitor_data.thread, NULL, monitor, (void *) &monitor_data);
	pthread_join(monitor_data.thread, NULL);


	// liberar memória
	free(philos);
	free(forks);

}