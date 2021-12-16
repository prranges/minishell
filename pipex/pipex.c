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

void	close_fds(t_arg *data, int **fd, int *file)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		close(fd[i][0]);
		close(fd[i][1]);
		i++;
	}
	if (!file)
		return ;
	if (file[0])
		close(file[0]);
	if (file[1])
		close(file[1]);
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
	//printf("cmd - %s\n", path_executive);
//	printf("PATH -- %s\n", path_executive);
	return (path_executive);
}

//void child_process(int i, t_arg *data, char **env, t_token *token)
void child_process(int i, t_arg *data, int **fd, char **env, t_token *token) {
	char *cmd_ex;
	int file[2];
	int dup2_fd;
//	(void)env;

	dup2_fd = 0;
	file[0] = -2;
	if (token->in)
		file[0] = open(token->in->file_name, O_RDONLY);
//	write(file[0], "qwerty\n", 7);
//	printf("file name - %s\n", token->in->file_name);
//	printf("fd - %d\n", file[0]);
	if (file[0] == -1) {
		perror("Error tut");
		exit(EXIT_FAILURE);
	}
	file[1] = -2;
	if (token->out && token->out->dbl)
		file[1] = open(token->out->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (token->out && !token->out->dbl)
		file[1] = open(token->out->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file[1] == -1) {
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (token->in && ft_strcmp(data->tokens->cmd[0], ""))
	{
//		printf("fd - %d\n", file[0]);
		dup2(file[0], STDIN_FILENO);
//		printf("fd - %d\n", file[0]);
//		file[0] = -5;
//		printf("fd - %d\n", file[0]);
	}
	else if (i > 0)// && data->fd)
		{
//			cmd_ex = get_cmd_arg(i, data, env, token->cmd);
//			dup2(data->fd[i - 1][0], STDIN_FILENO);//input
			dup2(fd[i - 1][0], STDIN_FILENO);
//			close_fds(data, data->fd);
//			close_fds(data, fd, file);
//			execve(cmd_ex, token->cmd, env);
		}
	if (token->out && ft_strcmp(data->tokens->cmd[0], ""))
		dup2(file[1], STDOUT_FILENO);
	else if (i < data->num - 1)// && data->fd)
		{
//			cmd_ex = get_cmd_arg(i, data, env, token->cmd);
//			dup2(data->fd[i][1], STDOUT_FILENO);//output
			dup2(fd[i][1], STDOUT_FILENO);//output
//			close_fds(data, fd, file);
//			close_fds(data, data->fd);
//			execve(cmd_ex, token->cmd, env);
		}
//		else if (data->num == 1)// && !data->fd)
//		{
//
//			cmd_ex = get_cmd_arg(i, data, env, token->cmd);
//			execve(cmd_ex, token->cmd, env);
//		}
    cmd_ex = get_cmd_arg(i, data, env, token->cmd);
    close_fds(data, fd, file);
    execve(cmd_ex, token->cmd, env);
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
//	char	**cmd;
//	int fd2 = 0;
//	char	**path;
//	char	*path_executive;
	pid_t	*pid;
	int		i;

	i = 0;
	fd = malloc(sizeof(int *) * data->num);
	while (i < data->num)
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
	i = 0;
	while (i < data->num)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("Error");
			exit(1);
		}
		if (pid[i] == 0) //child process
			 {
				child_process(i, data, fd, env, node);
//				child_process(i, data, env, node);
			}
			i++;
			if (node->next)
				node = node->next;
		}
		close_fds(data, fd, NULL);
		i = 0;
		while (i < data->num)
		{
//			waitpid(pid[i], NULL, 0);
			waitpid(-1, NULL, 0);
			i++;
		}
	return (0);
}
