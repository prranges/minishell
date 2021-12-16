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
	char **all_paths;
	char *path_executive;

	all_paths = find_path(data->env);//все пути
	path_executive = create_cmd_path(data, all_paths, cmd[0]);
//	if (!path_executive)
//	{
//		perror("Path");
//		exit(EXIT_FAILURE);
//	}
	return (path_executive);
}

int	make_builtin_dup(t_token *token)
{
	int	fd;
	int	file;

	if (!token->out)
		return (-1);
	if (token->out->dbl)
		file = open(token->out->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		file = open(token->out->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	fd = dup(STDOUT_FILENO);
	if (fd == -1)
	{
		close(file);
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		close(file);
		close(fd);
		perror("Error");
		exit(EXIT_FAILURE);
	}
	close(file);
	return (fd);
}

void	builtin_dup_error_check(int fd)
{
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("Error");
	}
	close(fd);
}

int child_process(int i, t_arg *data, int **fd, char **env, t_token *token)
{
	char *cmd_ex;
	int file[2];
	int fd_builtin;

	file[0] = -2;
    data->errnum = 0;
	if (token->in)
		file[0] = open(token->in->file_name, O_RDONLY);
	if (file[0] == -1)
	{
		perror("Error tut");
		exit(EXIT_FAILURE);
	}
	file[1] = -2;
	if (token->out && token->out->dbl)
		file[1] = open(token->out->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (token->out && !token->out->dbl)
		file[1] = open(token->out->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file[1] == -1)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}
	if (token->in && ft_strcmp(data->tokens->cmd[0], ""))
		dup2(file[0], STDIN_FILENO);
	else if (i > 0)// && data->fd)
			dup2(fd[i - 1][0], STDIN_FILENO);
	if (token->out && ft_strcmp(data->tokens->cmd[0], ""))
		dup2(file[1], STDOUT_FILENO);
	else if (i < data->num - 1)// && data->fd)
			dup2(fd[i][1], STDOUT_FILENO);//output
    cmd_ex = get_cmd_arg(i, data, env, token->cmd);
    close_fds(data, fd, file);
	if (token->builtin)
	{
		fd_builtin = make_builtin_dup(data->tokens);
		start_builtin(data);
		builtin_dup_error_check(fd_builtin);
	}
    if (execve(cmd_ex, token->cmd, env))
    {
        printf("minishell: %s: command not found\n", token->cmd[0]);
        data->errnum = 127;
    }
	return (0);
}

int	open_file(t_redir *redirect)
{
	int	fd;

//	fd = -2;
	if (redirect->out_in == 0 && redirect->dbl == 1) //output && >>
	{
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
	}
	else if (redirect->out_in == 0 && redirect->dbl == 0) //output && >
	{
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		printf("file name - %s\n", redirect->file_name);
		if (fd == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
	}
	else //if (redirect->out_in == 1 && redirect->dbl == 0) //input <
	{
		fd = open(redirect->file_name, O_RDONLY);
		if (fd == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
	}
	close(fd);
	return (0);
}

void heredoc(t_arg *data, char *limiter)//, char *file_name)
{
	int fd;
	int n = 0;
	char *line;
//	(void)data;

	fd = open(data->redir->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	printf ("%d\n", fd);
//	get_next_line()
	while (1)
	{
		printf("heredoc> ");
		n = get_next_line(1, &line);
		printf ("2\n");
		if (ft_strcmp(line, limiter) == 1)
		{
			printf ("2\n");
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
			free(line);
		}
		else if (ft_strcmp(line, limiter) == 0)
			break;
	}
	close (fd);
}

int precreate_or_preopen(t_arg *data)
{
	t_redir *redirect;

	redirect = data->redir;
	while (redirect)
	{
		if (redirect->out_in == 0 || redirect->dbl == 0)
		{
			if (open_file(redirect))
				return (1);
		}
		else
			heredoc(data, data->redir->lim);//, data->redir->file_name);
		redirect = redirect->next;
	}
	return (0);
}

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
