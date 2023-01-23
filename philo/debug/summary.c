/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   summary.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:53:33 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/23 17:07:09 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static void	print_philo(t_philo *philo);
static void	print_configuration(t_simulation *s);

void	summary(t_simulation *simulation)
{
	t_philo	*philo;
	size_t	i;

	print_configuration(simulation);
	printf(GREY "\nSUMMARY ====\n" RESET);
	i = 0;
	while (i < simulation->philos_num)
	{
		philo = &simulation->philos[i];
		print_philo(philo);
		i++;
	}
}

static void	print_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->philo_mutex);
	printf("Philo %02ld | ", philo->number);
	if (philo->died)
		printf(RED "DEAD " RESET " | ");
	else
		printf(GREEN "ALIVE" RESET " | ");
	printf("meals: " BLUE "%2d" RESET " | ", philo->meals);
	printf("began: " BLUE "%13ld" RESET " | ", philo->began_at);
	printf("last meal: " BLUE "%13ld" RESET " | ", philo->ate_at);
	printf("died at: " BLUE "%13ld" RESET " | ", philo->died_at);
	printf("starve time (ms): ");
	if (philo->died && philo->ate_at)
		printf(BLUE "%ld\n" RESET, philo->died_at - philo->ate_at);
	else if (philo->died && !philo->ate_at)
		printf(BLUE "%ld\n" RESET, philo->died_at - philo->began_at);
	else if (philo->ate_at)
	{
		if (philo->stopped_at > philo->ate_at)
			printf(BLUE "%ld\n" RESET, philo->stopped_at - philo->ate_at);
		else
			printf(BLUE "%ld\n" RESET, 0L);
	}
	else
		printf(BLUE "%ld\n" RESET, philo->stopped_at - philo->began_at);
}

static void	print_configuration(t_simulation *s)
{
	int	death_interval;
	int	eat_interval;
	int	sleep_interval;

	death_interval = s->philos[0].timer->death_interv;
	eat_interval = s->philos[0].timer->eat_interv;
	sleep_interval = s->philos[0].timer->sleep_interv;
	printf(GREY "\nCONFIG ====\n" RESET);
	printf("Philos count: %02ld | ", s->philos_num);
	printf("death interval: " BLUE "%4d" RESET " | ", death_interval);
	printf("eat interval: " BLUE "%4d" RESET " | ", eat_interval);
	printf("sleep interval: " BLUE "%4d" RESET " | ", sleep_interval);
	printf("must eat: " BLUE "%d\n" RESET, s->meals_goal);
}
