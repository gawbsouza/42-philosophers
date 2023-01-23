/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:48:57 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 21:25:06 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(void)
{
	t_ptimer		timer;
	t_table			table;

	timer_init(&timer, 500, 200, 200);
	table_init(&table, 30, 3);
	table_philos_init(&table, &timer);
	threads_init(&table);
	summary(&table);
	free(table.philos);
	free(table.forks);
}
