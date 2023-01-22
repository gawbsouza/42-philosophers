/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 17:12:41 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


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