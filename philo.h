/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:46:50 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 17:34:00 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

//-fsanitize=thread  detectar race condition

# define EATING_MSG "%ld %ld is eating\n"
# define SLEEPING_MSG "%ld %ld is sleeping\n"
# define THINKING_MSG "%ld %ld is thinking\n"
# define FORK_MSG "%ld %ld has taken a fork\n"
# define DIED_MSG "%ld %ld died\n"

# define FALSE 0
# define TRUE 1

typedef int	t_bool;

typedef struct s_ptimer
{
	int		eat_interv;
	int		sleep_interv;
	int		death_interv;
}	t_ptimer;

typedef struct s_fork
{
	t_bool	available;
	size_t	holded_by;
}	t_fork;

typedef enum e_status
{
	WAITING_FORK,
	EATING,
	SLEEPING,
	THINKING
}	t_status;

typedef struct s_philo
{
	size_t			number;
	t_bool			died;
	int				meals;
	t_fork			*left_fork;
	t_fork			*right_fork;
	long			began_at;
	long			ate_at;
	long			died_at;
	t_ptimer		*timer;
	t_status		status;
	t_bool			stopped;
	long			stopped_at;
	pthread_mutex_t	philo_mutex;
	pthread_mutex_t	*fork_mutex;
	pthread_t		thread;
}	t_philo;

typedef struct s_table
{
	t_fork			*forks;
	t_philo			*philos;
	size_t			philos_num;
	int				meals_goal;
	pthread_mutex_t	fork_mutex;
}	t_table;

typedef struct s_monitor
{
	t_table		*table;
	pthread_t	thread;
}	t_monitor;

long	time_millisec(void);
t_bool	is_dead(t_philo *philo);
void	*monitor(void *data);
void	update_philo_health(t_philo *philo);
void	summary(t_table *table);
t_bool	is_dead(t_philo *philo);
t_bool	is_stopped(t_philo *philo);
t_bool	is_waiting_fork(t_philo *philo);
void	*philosopher(void *data);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

#endif