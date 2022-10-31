#include "philosophers_bonus.h"
#include <stdlib.h>
#include <fcntl.h> //O_CREAT flag

int	free_mem(t_philo_manager *manager)
{
	sem_close(manager->f_sem);
	sem_close(manager->m_sem);
	sem_close(manager->t_sem);
	free(manager->pid_arr);
	return (1);
}

void	init_profile(t_philo_profile *profile, t_philo_args args, sem_t *t_sem)
{
	profile->idx = 0;
	profile->r_eat = 0;
	profile->r_sleep = 0;
	profile->r_think = 0;
	profile->die_time = args.die_time;
	profile->eat_time = args.eat_time;
	profile->sleep_time = args.sleep_time;
	profile->t_sem = t_sem;
}

int	init_manager(t_philo_manager *manager, t_philo_args args)
{
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_TERMINATE);
	sem_unlink(SEM_MUST_EAT);
	manager->philo_num = args.philo_num;
	manager->f_sem = sem_open(SEM_FORK, O_CREAT, 0644, args.philo_num);
	if (manager->f_sem == SEM_FAILED)
		return (1);
	manager->t_sem = sem_open(SEM_TERMINATE, O_CREAT, 0644, 1);
	if (manager->t_sem == SEM_FAILED)
	{
		sem_close(manager->t_sem);
		return (1);
	}
	if (args.must_eat_times == -1)
		manager->m_sem = NULL;
	else
	{
		manager->m_sem = sem_open(SEM_MUST_EAT, O_CREAT, 0644, args.philo_num);
		if (!manager->m_sem)
		{
			sem_close(manager->f_sem);
			sem_close(manager->t_sem);
			return (1);
		}
	}
	manager->pid_arr = (pid_t *)malloc(sizeof(pid_t) * args.philo_num);
	if (!manager->pid_arr)
	{
		sem_close(manager->f_sem);
		sem_close(manager->t_sem);
		if (manager->m_sem)
			sem_close(manager->m_sem);
		return (1);
	}
	return (0);
}