/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 22:27:06 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 22:57:15 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	ft_isdigit(int c);
static int	calc(const char *s, int *number);

int	ft_atoi(const char *s)
{
	int		number;
	int		negative;

	number = 0;
	negative = 0;
	negative = *s == '-';
	if (*s == '+' || *s == '-')
		s++;
	if (!*s || !ft_isdigit(*s))
		return (0);
	calc(s, &number);
	if (negative)
		return (number * -1);
	return (number);
}

static int	calc(const char *s, int *number)
{
	int	multiple;

	if (!ft_isdigit(*s))
		return (1);
	multiple = calc(s + 1, number);
	*number += (*s - '0') * multiple;
	return (multiple * 10);
}

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
