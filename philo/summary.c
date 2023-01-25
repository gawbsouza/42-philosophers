/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   summary.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:53:33 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 10:25:17 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_philo(t_philo *philo);
static void	print_configuration(t_simulation *s);
static void	print_starve_time(t_philo *philo);

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
	printf("\n");
}

static void	print_philo(t_philo *philo)
{
	long	death_time;
	long	last_meal_time;

	last_meal_time = 0;
	death_time = 0;
	if (philo->ate_at)
		last_meal_time = philo->ate_at - philo->began_at;
	if (philo->died)
		death_time = philo->died_at - philo->began_at;
	printf("Philo: " BLUE "%02ld" RESET " | ", philo->number);
	if (philo->died)
		printf(RED "■" RESET " | ");
	else
		printf(GREEN "■" RESET " | ");
	printf("meals: " BLUE "%2d" RESET " | ", philo->meals);
	printf("last meal time: " BLUE "%-5ld" RESET " | ", last_meal_time);
	printf("death time: " BLUE "%-5ld" RESET " | ", death_time);
	print_starve_time (philo);
}

static void	print_starve_time(t_philo *philo)
{
	printf("starve time: ");
	if (philo->died && philo->ate_at)
	{
		printf(BLUE "%ld\n" RESET, philo->died_at - philo->ate_at);
		return ;
	}
	if (philo->died && !philo->ate_at)
	{
		printf(BLUE "%ld\n" RESET, philo->died_at - philo->began_at);
		return ;
	}
	if (!philo->ate_at)
	{
		printf(BLUE "%ld\n" RESET, philo->stopped_at - philo->began_at);
		return ;
	}
	if (philo->stopped_at > philo->ate_at)
	{
		printf(BLUE "%ld\n" RESET, philo->stopped_at - philo->ate_at);
		return ;
	}
	printf(BLUE "%ld\n" RESET, 0L);
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
	printf("Philos: " BLUE "%02ld" RESET " | ", s->philos_num);
	printf("death interval: " BLUE "%-4d" RESET " | ", death_interval);
	printf("eat interval: " BLUE "%-4d" RESET " | ", eat_interval);
	printf("sleep interval: " BLUE "%-4d" RESET " | ", sleep_interval);
	printf("must eat: " BLUE "%d\n" RESET, s->meals_goal);
}
