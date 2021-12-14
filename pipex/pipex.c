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

	p = env;
	while (p != NULL)
	{
		if (ft_strncmp(p->key, "PATH", 4) == 0)
			break ;
		p = p->next;
	}
	path = ft_split(p->value, ':');
	return (path);
}

char *create_cmd_path(t_arg *data, char **all_paths, char *cmd)
{
	char *temp;
	char *path;
	(void)data;
	char **str;

	str = all_paths;
	while (*str)
	{
		temp = ft_strjoin(*str, "/");
		path = ft_strjoin(temp, cmd);
		if (access(path, X_OK) != -1)
			return (path);
		free(temp);
		free(path);
		str++;
	}
	return (NULL);
}

void	close_fds(t_arg *data, int **fd)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
}

char *get_cmd_arg(int i, t_arg *data, char **env, char **cmd)
{
	(void)data;
	(void)i;
	(void)env;
	//int str_num;
	char **all_paths;
	char *path_executive;

	all_paths = find_path(data->env);//все пути
//	for (int j = 0; j != 10; j++)
//		printf("%s\n", all_paths[j]);
	path_executive = create_cmd_path(data, all_paths, cmd[0]);
	if (!path_executive)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	printf("cmd - %s\n", path_executive);
//	printf("PATH -- %s\n", path_executive);
//		execve(path_executive, &cmd[i], env);
	return (path_executive);
}

void child_process(int i, t_arg *data, int **fd, char **env, t_token *token)
{
	char *cmd_ex;
//	(void)env;

		if (i == 0)
		{
			cmd_ex = get_cmd_arg(i, data, env, token->cmd);
//			if (!file){}
//			printf("fd before - %d\n", file);
//			printf("fd out - %d\n", fd_out);
//			fd2 = open(cmd_ex, O_WRONLY);
		//	fd[i][0] =  open(cmd_ex, O_WRONLY);
//			dup2(fd2, 0);
			dup2(fd[i][0], STDIN_FILENO);
			//dup2(fd[i][1], STDOUT_FILENO);
		//	close_fds(data, fd);
			printf("cmd_ex - %s\n", cmd_ex);
			printf("&cmd - %s\n", token->cmd[0]);
			execve(cmd_ex, token->cmd, env);
		}
		else if (i == data->num - 1)
		{
			cmd_ex = get_cmd_arg(i, data, env, token->cmd);
//			dup2(fd[i - 1][0], STDOUT_FILENO);//input
//			dup2(fd[i][1], STDOUT_FILENO);//output
//			printf("222\n");
//			close_fds(data, fd);
			execve(cmd_ex, token->cmd, env);
		}
		else
		{
			cmd_ex = get_cmd_arg(i, data, env, token->cmd);
//			cmd_ex = create_cmd_path(data, path);
//			fd[i][0] = open(cmd_ex, O_WRONLY);
//			dup2(fd[i - 1][0], STDIN_FILENO);//input
//			dup2(fd[i][1], STDOUT_FILENO);//output
			//close_fds(data, fd);
			execve(cmd_ex, token->cmd, env);
		}
}

//char *make_cmd() {
//	path = find_path(data->env);
//	path_executive = create_cmd_path(data, path);
//}

int pipex(int argc, char **argv, char **env, t_arg *data)
{
	(void)argv;
	(void)argc;

	int		**fd;
	//char	*cmd_ex;
	t_token	*node;
	char	**cmd;
//	int fd2 = 0;
//	char	**path;
//	char	*path_executive;
	pid_t	*pid;
	int		i;

	i = 0;
	fd = malloc(sizeof(int *) * data->num);
	while(i < data->num)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!(fd[i]))
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	cmd = data->tokens->cmd;
	node = data->tokens;
	while (i < data->num)
	{
		pipe(fd[i]);
		if (pipe(fd[i]) == -1) {
			perror("Error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	pid = malloc(sizeof(pid_t *) * data->num);
	//path = find_path(data->env);
	//path_executive = create_cmd_path(data, path);
	i = 0;
	while(i < data->num)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("Error");
			exit(1);
		}
		if (pid[i] == 0) //child process
		{
			//cmd_ex = get_cmd_arg(i, data, env, node->cmd);
//			printf("%s\n", cmd_ex);
			child_process(i, data, fd, env, node);
		}
		i++;
		if (node->next)
		{
			node = node->next;
		}
	}
	close_fds(data, fd);
	i = 0;
	while (i != data->num)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	return (0);
}
