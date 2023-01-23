/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 23:06:48 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		print_usage_message(void);
static t_bool	is_valid_args(int argc, const char **argv);

int	main(int argc, const char **argv)
{
	t_ptimer		timer;
	t_simulation	simulation;
	int				meals_goal;

	if (!is_valid_args(argc, argv))
		return (1);
	if (argc == 6)
		meals_goal = ft_atoi(argv[5]);
	else
		meals_goal = 0;
	timer_init(&timer, ft_atoi(argv[2]), ft_atoi(argv[3]), ft_atoi(argv[4]));
	simulation_init(&simulation, ft_atoi(argv[1]), meals_goal);
	simulation_philos_init(&simulation, &timer);
	threads_init(&simulation);
	if (DEBUG_MODE)
		summary(&simulation);
	free(simulation.philos);
	free(simulation.forks);
}

static void	print_usage_message(void)
{
	printf(BLUE "Correct usage: \n" RESET);
	printf("philo ");
	printf("number_of_philosophers ");
	printf("time_to_die ");
	printf("time_to_eat ");
	printf("time_to_sleep ");
	printf(GREY "[times_each_philosopher_must_eat] \n" RESET);
}

static t_bool	is_valid_args(int argc, const char **argv)
{
	if (argc < 5 || argc > 6)
	{
		print_usage_message();
		return (FALSE);
	}
	if (ft_atoi(argv[1]) <= 0)
		printf("invalid [number_of_philosophers] provided\n");
	else if (ft_atoi(argv[2]) <= 0)
		printf("invalid [time_to_die] provided.\n");
	else if (ft_atoi(argv[3]) <= 0)
		printf("invalid [time_to_eat] provided.\n");
	else if (ft_atoi(argv[4]) <= 0)
		printf("invalid [time_to_sleep] provided.\n");
	else if (argc == 6 && ft_atoi(argv[5]) <= 0)
		printf("invalid [times_each_philosopher_must_eat] provided.\n");
	else
		return (TRUE);
	return (FALSE);
}
