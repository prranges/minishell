/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:46:43 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:46:45 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_cycle(char *name, char *line, int fd, t_arg *data)
{
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("  \033[A", 1);
			break ;
		}
		if (ft_strcmp(line, name) == 0)
			break ;
		if (write(fd, line, ft_strlen(line)) == -1)
			my_exit(data, name, errno, 0);
		if (write(fd, "\n", 1) == -1)
			my_exit(data, name, errno, 0);
		free(line);
	}
}

static void	ft_here_sig(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\n", 1);
		rl_replace_line("", 1);
		g_signals.exit_status = 1;
		exit(1);
	}
	if (signal == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b", 1);
		rl_redisplay();
	}
}

void	heredoc(char *name, t_arg *data)
{
	int		fd;
	char	*line;
	pid_t	pid;

	line = NULL;
	pid = fork();
	if (pid < 0)
		my_exit(data, "fork", errno, 0);
	if (pid == 0)
	{
		signal(SIGINT, &ft_here_sig);
		signal(SIGQUIT, &ft_here_sig);
		fd = open("heredoc_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			my_exit(data, name, errno, 0);
		check_fd_exist(fd, "heredoc_file", data);
		heredoc_cycle(name, line, fd, data);
		close(fd);
		exit (0);
	}
	waitpid(pid, &g_signals.exit_status, 0);
	if (WIFEXITED(g_signals.exit_status))
		g_signals.exit_status = WEXITSTATUS(g_signals.exit_status);
}
