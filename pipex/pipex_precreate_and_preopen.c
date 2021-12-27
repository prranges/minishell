/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_precreate_and_preopen.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:47:02 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:47:03 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	precreate_or_preopen(t_arg *data)
{
	t_redir	*redirect;

	redirect = data->redir;
	while (redirect)
	{
		if (redirect->out_in == 0 || redirect->dbl == 0)
		{
			if (open_file(redirect, data))
				return (1);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
			heredoc(redirect->file_name, data);
			signal(SIGINT, &sig_int);
			signal(SIGQUIT, &sig_int);
		}
		redirect = redirect->next;
	}
	return (0);
}

void	redirect_dbl(t_redir *redirect, t_arg *data)
{
	free(redirect->file_name);
	redirect->file_name = ft_strdup("heredoc_file");
	if (!redirect->file_name)
		my_exit(data, "malloc", 12, 0);
}

int	open_file(t_redir *redirect, t_arg *data)
{
	int	fd;

	fd = 0;
	if (redirect->out_in == 0 && redirect->dbl == 1)
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		my_exit(data, redirect->file_name, errno, 0);
	else if (redirect->out_in == 0 && redirect->dbl == 0)
	{
		fd = open(redirect->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			my_exit(data, redirect->file_name, errno, 0);
	}
	else
	{
		if (redirect->dbl == 0)
			redirect_dbl(redirect, data);
		fd = open(redirect->file_name, O_RDONLY);
		if (fd == -1)
			my_exit(data, redirect->file_name, errno, 0);
	}
	check_fd_exist(fd, redirect->file_name, data);
	close(fd);
	return (0);
}
