/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:34:14 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/19 19:13:54 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int pipex(int argc, char **argv, char **env, t_arg *data)
{
	(void)data;
	(void)argv;

	int **fd;
	pid_t *pid;
	int i;
	//int j = 0;
	int file;
	int fd_out;

	i = 0;
	fd = malloc(sizeof(int *) * (argc - 4));
	while(i < argc - 4)
	{
		fd[i] = malloc(sizeof(int) * 2);
		i++;
	}
	i = 0;
	while (i < argc - 4)
	{
		pipe(fd[i]);
		if (pipe(fd[i]) == -1)
			exit(1);
		//printf("fd_1 - %d , %d\n", fd[i][0], fd[i][1]);
		i++;
	}
	pid = malloc(sizeof(pid_t *));
	i = 0;
	while(i < argc - 3)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("error");
			exit(1);
		}
		else if (pid[i] == 0)
		{
			if (i == 0)
			{
				char **arr;
				arr = malloc(sizeof(char *) * 2);
				arr[0] = malloc(sizeof(char) * 8);
				arr[1] = NULL;
				arr[0] = "/bin/pwd";
				file = open("/Users/caniseed/Desktop/minishell/minishell/minishell/pipex/input.txt", O_WRONLY);
//				if (!file){}
				printf("fd before - %d\n", file);
				fd_out = open("/Users/caniseed/Desktop/minishell/minishell/minishell/pipex/output.txt", O_WRONLY | O_CREAT | O_TRUNC, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
				printf("fd out - %d\n", fd_out);
				dup2(file, 0);
				printf("fd after - %d\n", file);
				dup2(fd_out, 1);
				printf("111\n");
//				close_fds(info, fd);
				execve(arr[0], arr, env);
			}
			else
			{
				dup2(fd[i][0], 0);//input
				dup2(fd[i][1], 1);//output
				printf("222\n");
//				close_fds(info, fd);
//				execve(cmd[0], cmd, env);
				//execve("/bin/pwd",  "-P", env);
			}
		}
		i++;
	}
	return (0);
//	int fd[2];
//
//	if (pipe(fd) == -1)
//		return (1);
//
//	int pid1 = fork();
//	int pid2;
//	if (pid1 > 0)
//		pid2 = fork();
//
//	if (pid1 < 0)
//		return (2);
//	if (pid1 == 0) //child process 1 (ping)
//	{
//		dup2(fd[1], STDOUT_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		execlp("ls", "ls", "-l", NULL);
//	}
//
//	if (pid2 == 0) //child process 1 (ping)
//	{
//		dup2(fd[0], STDIN_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		execlp("cat", "cat", "-e", NULL);
//	}
//	int i = 	while (i < q_of_pipes)
//	{
//		malloc fd;
//		pipe(fd[i]);
//		fork();
//		i++;
//	}0;
//



//	int pid2 = fork();
//
//	if (pid2 == 0)
//	{
//		dup2(fd[0], STDIN_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		execlp("grep", "grep", "rtt", NULL);
//	}


//	waitpid(pid1, NULL, 0);
//	waitpid(pid2, NULL, 0);
	return (0);
}