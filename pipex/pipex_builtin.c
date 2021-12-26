/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:46:19 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:46:21 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	dup2_builtin_close_error(int file, int fd, t_arg *data)
{
	close(file);
	close(fd);
	my_exit(data, "dup2", errno);
}

int	make_builtin_dup(t_token *token, t_arg *data)
{
	int	fd;
	int	file;

	file = -2;
	if (!token->out)
		return (-1);
	if (token->out->dbl)
		file = open(token->out->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		file = open(token->out->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (!file)
		my_exit(data, token->cmd[0], errno);
	fd = dup(STDOUT_FILENO);
	if (fd == -1)
	{
		close(file),
		my_exit(data, "dup", errno);
	}
	if (dup2(file, STDOUT_FILENO) == -1)
		dup2_builtin_close_error(file, fd, data);
	close(file);
	return (fd);
}

void	builtin_dup_error_check(int fd, t_arg *data)
{
	if (fd == -1)
		my_exit(data, "dup", errno);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		my_exit(data, "dup2", errno);
	}
	close(fd);
}
