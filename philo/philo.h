/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:46:50 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/22 21:57:58 by gasouza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

# define RESET "\033[0m"
# define RED "\033[0;31m"

# define EATING_MSG "%ld %ld is eating\n"
# define SLEEPING_MSG "%ld %ld is sleeping\n"
# define THINKING_MSG "%ld %ld is thinking\n"
# define FORK_MSG "%ld %ld has taken a fork\n"
# define DIED_MSG "%ld %ld died\n"

# define FALSE 0
# define TRUE 1

# ifndef DEBUG_MODE
#  define DEBUG_MODE 0
# endif

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

typedef struct s_simulation
{
	t_fork			*forks;
	t_philo			*philos;
	size_t			philos_num;
	int				meals_goal;
	pthread_mutex_t	fork_mutex;
}	t_simulation;

long	time_millisec(void);
t_bool	is_dead(t_philo *philo);
void	*monitor_runner(void *arg);
void	update_philo_health(t_philo *philo);
void	summary(t_simulation *simulation);
t_bool	is_dead(t_philo *philo);
t_bool	is_stopped(t_philo *philo);
t_bool	is_waiting_fork(t_philo *philo);
void	*philo_runner(void *arg);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
void	timer_init(t_ptimer *timer, long death, long eat, long sleep);
void	simulation_init(t_simulation *s, size_t philos_num, int meals_goal);
void	philosopher_init(t_philo *philo, size_t number);
void	simulation_philos_init(t_simulation *simulation, t_ptimer *timer);
void	threads_init(t_simulation *simulation);

#endif