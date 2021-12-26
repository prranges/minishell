/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:46:37 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:46:38 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	create_file_0(t_token *token, t_arg *data)
{
	int	fd;

	if (token->in && token->in->dbl)
		fd = open("heredoc_file", O_RDONLY);
	if (token->in && !token->in->dbl)
		fd = open(token->in->file_name, O_RDONLY);
	if (fd == -1)
		my_exit(data, token->in->file_name, errno);
	if (token->in)
		check_fd_exist(fd, token->in->file_name, data);
	return (fd);
}

int	create_file_1(t_token *token, t_arg *data)
{
	int	fd;

	fd = -2;
	if (token->out && token->out->dbl)
		fd = open(token->out->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (token->out && !token->out->dbl)
		fd = open(token->out->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		my_exit(data, token->out->file_name, errno);
	if (token->out)
		check_fd_exist(fd, token->out->file_name, data);
	return (fd);
}

int	child_process(int i, t_arg *data, int **fd, t_token *token)
{
	int	file[2];
	int	dup_result;

	dup_result = 0;
	file[0] = create_file_0(token, data);
	file[1] = create_file_1(token, data);
	if (token->in && ft_strcmp(data->tokens->cmd[0], ""))
		dup_result = dup2(file[0], STDIN_FILENO);
	if (dup_result == -1)
		my_exit(data, "dup2", errno);
	else if (i != 0 && data->fd)
		dup_result = dup2(fd[i - 1][0], STDIN_FILENO);
	if (dup_result == -1)
		my_exit(data, "dup2", errno);
	if (token->out && ft_strcmp(data->tokens->cmd[0], ""))
		dup_result = dup2(file[1], STDOUT_FILENO);
	if (dup_result == -1)
		my_exit(data, "dup2", errno);
	else if (i < data->num - 1 && data->fd)
		dup_result = dup2(fd[i][1], STDOUT_FILENO);
	if (dup_result == -1)
		my_exit(data, "dup2", errno);
	close_fds(data, fd, file);
	exec_start(data, token);
	return (0);
}
