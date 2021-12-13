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

char	**find_path(t_env *env)
{
	t_env	*p;
	char 	**path;
	int		i;

	p = env;
	i = 0;
	while (p != NULL)
	{
		if (ft_strncmp(p->key, "PATH", 4) == 0)
			break ;
		p = p->next;
	}
	path = ft_split(p->value, ':');
	return (path);
}

char *create_path(t_arg *data, char **all_paths)
{
	int i;
	char *temp;
	char *path;
	(void)data;

	i = 1;

	//printf("111\n");
	while (*all_paths)
	{
		//printf("111\n");
		temp = ft_strjoin(*all_paths, "/");
		//printf("222\n");
		path = ft_strjoin(temp, data->tokens->cmd[0]);
		//printf("%s\n", path);
		if (access(path, X_OK) != -1)
			return (path);
		//free(temp);
		//free(path);
		all_paths++;
	}
	return (NULL);
}

//void close_fd(int fd)

int pipex(int argc, char **argv, char **env, t_arg *data)
{
	(void)argv;

	int **fd;
	int fd2;
	char **path;
	char *path_exec;
	pid_t *pid;
	int i;
	//int j = 0;
	//int file;
	//int fd_out;
	char *flag;

	i = 0;
    errno = 0;
	flag = "-l";
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
	path = find_path(data->env);
//	while (*path)
//	{
//		printf("%s\n", *path);
//		path++;
//	}
	path_exec = create_path(data, path);
	//printf("%s\n", path_exec);
	while(i < data->num)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("minishell");
			exit(1);
		}
		else if (pid[i] == 0)
		{
			//printf("111\n");
			if (i == 0)
			{
//				char **arr;
//				arr = malloc(sizeof(char *) * 2);
//				arr[0] = malloc(sizeof(char) * 8);
//				arr[1] = NULL;
//				arr[0] = "/bin/pwd";
//				file = open("/Users/caniseed/Desktop/minishell/minishell/minishell/pipex/input.txt", O_WRONLY);
//				if (!file){}
				//printf("fd before - %d\n", file);
//				fd_out = open("/Users/caniseed/Desktop/minishell/minishell/minishell/pipex/output.txt", O_WRONLY | O_CREAT | O_TRUNC, \
//			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
				//printf("fd out - %d\n", fd_out);
				fd2 = open(path_exec, O_WRONLY);
				dup2(fd2, 1);
				//printf("fd after - %d\n", file);
				//dup2(fd_out, 1);
			//	printf("111\n");
//				close(fd2);
				execve(path_exec, data->tokens->cmd, env);
			}
			else
			{
				dup2(fd[i][0], 0);//input
				dup2(fd[i][1], 1);//output
				printf("222\n");
//				close_fds(info, fd);
//				execve(cmd[0], cmd, env);
				execve(path_exec, data->tokens->cmd, env);
			}
		}
		i++;
	}
	i = 0;
	while (i < data->num) {
		waitpid(pid[i], NULL, 0);
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
