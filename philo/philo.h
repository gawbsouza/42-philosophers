/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasouza <gasouza@student.42sp.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 08:46:50 by gasouza           #+#    #+#             */
/*   Updated: 2023/01/25 17:02:56 by gasouza          ###   ########.fr       */
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
# define BLUE "\033[0;34m"
# define GREEN "\033[0;32m"
# define GREY "\033[1;30m"

# define EATING_MSG "%-5ld %-2ld is eating\n"
# define SLEEPING_MSG "%-5ld %-2ld is sleeping\n"
# define THINKING_MSG "%-5ld %-2ld is thinking\n"
# define FORK_MSG "%-5ld %-2ld has taken a fork\n"
# define DIED_MSG "%-5ld %-2ld died\n"

# define FALSE 0
# define TRUE 1

# ifndef SHOW_SUMMARY
#  define SHOW_SUMMARY 0
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
	t_bool			is_eating;
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
	t_ptimer		timer;
	pthread_mutex_t	fork_mutex;
}	t_simulation;

// philo functions
t_bool	philo_can_run(t_philo *philo);
void	philo_take_forks(t_philo *philo);
void	philo_drop_forks(t_philo *philo);
long	philo_running_time(t_philo *philo);

// runners functions
void	*monitor_runner(void *arg);
void	*philo_runner(void *arg);

// simulation functions
void	simulation_run(t_simulation *simulation, t_bool debug_mode);
void	summary(t_simulation *simulation);

// others functions
int		ft_atoi(const char *s);
long	time_millisec(void);

#endif