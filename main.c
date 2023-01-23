/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 21:28:36 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(void)
{
	t_ptimer		timer;
	t_simulation	simulation;

	timer_init(&timer, 500, 200, 200);
	simulation_init(&simulation, 30, 3);
	simulation_philos_init(&simulation, &timer);
	threads_init(&simulation);
	summary(&simulation);
	free(simulation.philos);
	free(simulation.forks);
}
