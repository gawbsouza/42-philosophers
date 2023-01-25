/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 16:48:22 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 16:56:01 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	philo_running_time(t_philo *philo)
{
	return (time_millisec() - philo->began_at);
}

t_bool	philo_can_run(t_philo *philo)
{
	return (!philo->died && !philo->stopped);
}
