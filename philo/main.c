/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 11:28:19 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		print_usage_message(void);
static t_bool	is_valid_args(int argc, const char **argv);

int	main(int argc, const char **argv)
{
	t_simulation	simulation;

	if (!is_valid_args(argc, argv))
		return (1);
	simulation.philos_num = ft_atoi(argv[1]);
	simulation.timer.death_interv = ft_atoi(argv[2]);
	simulation.timer.eat_interv = ft_atoi(argv[3]);
	simulation.timer.sleep_interv = ft_atoi(argv[4]);
	simulation.meals_goal = 0;
	if (argc == 6)
		simulation.meals_goal = ft_atoi(argv[5]);
	simulation_run(&simulation, SHOW_SUMMARY);
}

static void	print_usage_message(void)
{
	printf("\nusage:\t");
	printf("philo ");
	printf("number_of_philosophers ");
	printf("time_to_die ");
	printf("time_to_eat ");
	printf("time_to_sleep ");
	printf("[times_each_philosopher_must_eat] \n\n");
	printf("  number_of_philosophers \t\t");
	printf("number of philosophers into the simulation.\n");
	printf("  time_to_die \t\t\t\t");
	printf("time in milliseconds that a philosopher die of starve.\n");
	printf("  time_to_eat \t\t\t\t");
	printf("time in milliseconds it takes a philosopher to eat.\n");
	printf("  time_to_sleep \t\t\t");
	printf("time in milliseconds it takes a philosopher to sleep.\n");
	printf("  [times_each_philosopher_must_eat] \t");
	printf("the simulation ends after all philosophers have eaten\n");
	printf("\t\t\t\t\tthis minimum times.\n\n");
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
