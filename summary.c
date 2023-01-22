/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   summary.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:53:33 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 16:57:54 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_philo(t_philo *philo);

void	summary(t_table *table)
{
	t_philo	*philo;
	size_t	i;

	printf("\nSUMMARY ====\n");
	i = 0;
	while (i < table->philos_num)
	{
		philo = &table->philos[i];
		print_philo(philo);
		i++;
	}
}

static void	print_philo(t_philo *philo)
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
