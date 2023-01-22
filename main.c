/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 16:06:50 by gasouza          ###   ########.fr       */
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
	if (philo->died)
		philo->died_at = time_millisec();
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
	philo->stopped_at = time_millisec();
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

void	*monitor(void *data)
{
	t_monitor *monitor = (t_monitor *) data;
	t_table	*table;
	t_bool end;
	
	int meals_count;

	end = FALSE;
	table = monitor->table;
	while (!end)
	{
		meals_count = 0;
		for(int i = 0; i < table->philos_num; i++)
			check_death(&table->philos[i]);
		for(int i = 0; i < table->philos_num; i++)
		{
			if (is_dead(&table->philos[i]))
			{
				printf(DIED_MSG, time_millisec(), table->philos[i].number);
				end = TRUE;
				break;
			}
		}
		for(int i = 0; i < table->philos_num; i++)
		{
			pthread_mutex_lock(&table->philos[i].philo_mutex);
			if (table->philos[i].meals >= table->meals_goal)
				meals_count++;
			pthread_mutex_unlock(&table->philos[i].philo_mutex);

			if (table->meals_goal && meals_count >= table->philos_num)
			{
				end = TRUE;
				break;
			}
		}
	}
	for(int x = 0; x < table->philos_num; x++)
		stop(&table->philos[x]);
	for(int x = 0; x < table->philos_num; x++)
		pthread_join(table->philos[x].thread, NULL);
	return NULL;
}

void	print_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	printf("Philo %02ld | ", philo->number);
	if (philo->died)
		printf("DEAD  | ");
	else
		printf("ALIVE | ");
	printf("meals: %2d | ", philo->meals);
	printf("began: %13ld | ", philo->began_at);
	printf("last meal: %13ld | ", philo->ate_at);
	printf("died at: %13ld | ", philo->died_at);
	printf("hungry time (ms): ");
	if (philo->died && philo->ate_at)
		printf("%ld\n", philo->died_at - philo->ate_at);
	else if (philo->died && !philo->ate_at)
		printf("%ld\n", philo->died_at - philo->began_at);
	else if (philo->ate_at)
	{
		if (philo->stopped_at > philo->ate_at)	
			printf("%ld\n", philo->stopped_at - philo->ate_at);
		else
			printf("%ld\n", 0L);
	}
	else
		printf("%ld\n", philo->stopped_at - philo->began_at);
}

void	summary(t_table *table)
{
	t_philo *philo;
	
	printf("\nSUMMARY ====\n");
	for(int i = 0; i < table->philos_num; i++)
	{
		philo = &table->philos[i];
		print_philo(philo);
	}
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
	for(int i = 0; i < table.philos_num; i++)
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
	for(int i = 0; i < table.philos_num; i++)
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