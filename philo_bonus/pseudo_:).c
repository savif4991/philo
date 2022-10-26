#include "philosophers_bonus.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h> //O_CREAT flag
#include <sys/wait.h> //waitpid

#ifndef SEM_NAME
# define SEM_NAME "/mysem"
#endif

/*
모든 포크는 탁상 중앙에 위치한다.
메모리에는 아무런 상태가 없지만 사용 가능한 포크의 수는 세마포어로 나타낸다.
각 철학자는 프로세스가 되어야 한다. 하지만 메인 프로세스는 철학자가 되선 안된다.
*/

void	*routine(void *philo_info)
{
	t_philo_profile	*p_info;
	struct timeval	time;

	p_info = (t_philo_profile *)philo_info;
	gettimeofday(&time, NULL);
	p_info->r_eat = time.tv_sec / 100000 + time.tv_usec / 1000;
	while (is_termination(p_info, &time))
	{
		if (!(p_info->r_fork))
		{
			usleep(p_info->die_time * 1000);
			gettimeofday(&time, NULL);
			printf("%lu 1 died\n", time.tv_sec / 100000 + time.tv_usec / 1000);
			pthread_mutex_unlock(p_info->mtx);
			break ;
		}
		else if (*(p_info->l_fork) && *(p_info->r_fork))
			grab_eat_sleep(p_info, &time);
		else
			pthread_mutex_unlock(p_info->mtx);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo_args	args;
	t_philo_manager	manager;
	pid_t			*pid_arr;
	sem_t			*sem;
	int				child_count;
	pid_t			pid;
	int				status;


	if ((argc != 5 && argc != 6) || prep_args(&args, argv))
	{
		printf("invalid args\n");
		return (0);
	}
	sem = sem_open(SEM_NAME, O_CREAT, 0644, args.philo_num);
	if (sem == SEM_FAILED)
		return (1);
	pid_arr = (pid_t *)malloc(sizeof(pid_t) * args.philo_num);
	if (!pid_arr)
	{
		sem_close(sem);
		return (1);
	}
	child_count = args.philo_num;
	while (child_count)
	{
		pid = fork();
		if (pid == -1)
			return (fork_err());
		else if (!pid)
			routine();
		child_count--;
		pid_arr[child_count] = pid;
	}
	child_count = args.philo_num;
	while (child_count--)
		waitpid(pid_arr[child_count], &status, 0);



	if (init_manager(&manager, args))
		return (1);
	init_profile(&manager, &args);
	if (get_thread_mutex(args, manager.profile))
		return (recover_thr_free_mem(&manager, args));
	recover_thr_free_mem(&manager, args);
	return (0);
}