/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daejlee <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 19:41:48 by daejlee           #+#    #+#             */
/*   Updated: 2022/10/25 19:41:48 by daejlee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"
#include <stdlib.h>

void	recover_thr_free_mem(t_philo_manager *manager, t_philo_args args)
{
	int	i;

	i = 0;
	while (i < args.philo_num)
	{
		pthread_join(manager->profile[i].thr, NULL);
		i++;
	}
	purge_mtx(manager, args);
	i = 0;
	while (i < args.philo_num)
		free(manager->m_fork[i++]);
	free(manager->fork_stat);
	free(manager->m_fork);
	free(manager->profile);
	free(manager->must_eat_flags);
}

static const char	*blank_check(const char *nptr)
{
	unsigned int	i;

	i = 0;
	while ((nptr[i] >= 8 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	return (&nptr[i]);
}

static int	outlier_check(unsigned int res, int sign)
{
	if (res > 2147483648 && sign == -1)
		return (0);
	else if (res > 2147483647 && sign == 1)
		return (-2);
	else
		return (res * sign);
}

int	ft_atoi(const char *nptr)
{
	int					sign;
	unsigned int		i;
	unsigned int		res;

	nptr = blank_check(nptr);
	sign = 1;
	i = 0;
	res = 0;
	if (nptr[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] <= '9' && nptr[i] >= '0')
	{
		res = res * 10 + nptr[i] - '0';
		i++;
	}
	res = outlier_check(res, sign);
	return (res);
}

int	prep_args(t_philo_args *p, char **argv)
{
	p->philo_num = ft_atoi(argv[1]);
	p->die_time = ft_atoi(argv[2]);
	p->eat_time = ft_atoi(argv[3]);
	p->sleep_time = ft_atoi(argv[4]);
	if (argv[5])
		p->must_eat = ft_atoi(argv[5]);
	else
		p->must_eat = -1;
	if (p->philo_num < 1 || p->die_time < 1
		|| p->eat_time < 1 || p->sleep_time < 1
		|| p->must_eat < -1)
		return (1);
	return (0);
}
