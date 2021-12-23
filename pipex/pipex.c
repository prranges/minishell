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

	path = NULL;
	p = env;
	while (p != NULL)
	{
		if (ft_strncmp(p->key, "PATH", 4) == 0)
		{
			path = ft_split(p->value, ':');
			return (path);
		}
		p = p->next;
	}
	return (NULL);
}

char *create_cmd_path(t_arg *data, char **all_paths, char *cmd)
{
	char *temp;
	char *path;
	(void)data;
	char **str;

	if (!all_paths)
		return (NULL);
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

char *get_cmd_arg(t_arg *data, char **cmd)
{
	char **all_paths;
	char *path_executive;

	if (access(cmd[0], X_OK) != -1)
		return (cmd[0]);
	all_paths = find_path(data->env);
	if (!all_paths)
	{
		printf("minishell: %s: %s\n", cmd[0], strerror(errno));
		data->errnum = 127;
		exit (data->errnum);
	}
	path_executive = create_cmd_path(data, all_paths, cmd[0]);
	return (path_executive);
}

void check_fd_exist(int fd, char *str)
{
    if (fd == -1)
    {
        printf("minishell: %s: %s\n", str, strerror(errno));
        exit(EXIT_FAILURE);
    }
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
	check_fd_exist(file, token->out->file_name);
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

int exec_start(t_arg *data, t_token *token)
{
    int     fd_builtin;
    char    *cmd_ex;
    
    if (token->builtin)
    {
		fd_builtin = make_builtin_dup(data->tokens);
		builtin_dup_error_check(fd_builtin);
        exit (start_builtin(data));
    }
	else
	{
		cmd_ex = get_cmd_arg(data, token->cmd);
		if (execve(cmd_ex, token->cmd, data->env_str) && ft_strcmp(data->tokens->cmd[0], ""))
		{
			printf("minishell: %s: command not found\n", token->cmd[0]);
			data->errnum = 127;
			exit (data->errnum);
		}
	}
    return (0);
}

int child_process(int i, t_arg *data, int **fd, t_token *token)
{
	int file[2];
	(void)fd;
    
	file[0] = -2;
    data->errnum = 0;
	if (token->in && token->in->dbl)
		file[0] = open("heredoc_file", O_RDONLY);
	if (token->in && !token->in->dbl)
		file[0] = open(token->in->file_name, O_RDONLY);
	if (token->in)
		check_fd_exist(file[0], token->in->file_name);
	file[1] = -2;
	if (token->out && token->out->dbl)
		file[1] = open(token->out->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (token->out && !token->out->dbl)
		file[1] = open(token->out->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file[1] == -1)
	if (token->out)
		check_fd_exist(file[1], token->out->file_name);
    
	if (token->in && ft_strcmp(data->tokens->cmd[0], ""))
		dup2(file[0], STDIN_FILENO);
	else if (i != 0 && data->fd)
			dup2(fd[i - 1][0], STDIN_FILENO);
	if (token->out && ft_strcmp(data->tokens->cmd[0], ""))
		dup2(file[1], STDOUT_FILENO);
	else if (i < data->num - 1 && data->fd)
			dup2(fd[i][1], STDOUT_FILENO);//output
    close_fds(data, fd, file);
    exec_start(data, token);

	return (0);
}

int	open_file(t_redir *redirect)
{
	int	fd;

	if (redirect->out_in == 0 && redirect->dbl == 1) //output && >>
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redirect->out_in == 0 && redirect->dbl == 0) //output && >
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else //if (redirect->out_in == 1 && redirect->dbl == 0) //input <
	{
		if (redirect->dbl == 0)
		{
			free(redirect->file_name);
			redirect->file_name = ft_strdup("heredoc_file");
		}
		fd = open(redirect->file_name, O_RDONLY);
	}
	check_fd_exist(fd, redirect->file_name);
	close(fd);
	return (0);
}

void heredoc(t_arg *data)
{
	int fd;
	char *line;

	fd = open("heredoc_file", O_WRONLY | O_CREAT | O_APPEND, 0644);
    check_fd_exist(fd, "heredoc_file");
	while (1)
	{
		write (1, "> ", 2);
		get_next_line(STDIN_FILENO, &line);
		if (ft_strcmp(line, data->redir->file_name) == 0)
			break ;
		if (write(fd, line, ft_strlen(line)) == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
		if (write(fd, "\n", 1) == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	close(fd);
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
			heredoc(data);
		redirect = redirect->next;
	}
	return (0);
}

int pipex(t_arg *data)
{
	t_token	*node;
	int		i;

	i = 0;
	data->fd = malloc(sizeof(int *) * data->num);
	while (i < data->num)
	{
		data->fd[i] = malloc(sizeof(int) * 2);
		if (!(data->fd[i]))
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
		pipe(data->fd[i]);
		if (pipe(data->fd[i]) == -1)
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	g_signals.pid = malloc(sizeof(pid_t *) * data->num);
	i = 0;
	while (i < data->num)
	{
		g_signals.pid[i] = fork();
		if (g_signals.pid[i] < 0)
		{
			perror("Error");
			exit(1);
		}
		if (g_signals.pid[i] == 0)
				child_process(i, data, data->fd, node);
			i++;
			if (node->next)
				node = node->next;
		}
		close_fds(data, data->fd, NULL);
		i = 0;
		while (i < data->num)
		{
			waitpid(g_signals.pid[i], NULL, 0);
			i++;
		}
	return (0);
}
