/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:47:10 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:47:11 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	create_fd(t_arg *data)
{
	int	i;

	i = 0;
	data->fd = malloc(sizeof(int *) * data->num);
	if (!data->fd)
		my_exit(data, "malloc", 12);
	while (i < data->num)
	{
		data->fd[i] = malloc(sizeof(int) * 2);
		if (!(data->fd[i]))
			my_exit(data, "malloc", 12);
		i++;
	}
}

void	create_pipe_fd(t_arg *data)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		pipe(data->fd[i]);
		if (pipe(data->fd[i]) == -1)
			my_exit(data, "pipe", g_signals.exit_status);
		i++;
	}
}

void	waitpid_pipex(t_arg *data)
{
	int	i;

	i = 0;
	while (i < data->num)
	{
		waitpid(g_signals.pid[i], &g_signals.exit_status, 0);
		if (WIFEXITED(g_signals.exit_status))
			g_signals.exit_status = WEXITSTATUS(g_signals.exit_status);
		i++;
	}
}
