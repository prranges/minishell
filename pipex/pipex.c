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

void	builtin_dup_error_check(int fd, t_arg *data)
{
	if (fd == -1)
		return ;
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		my_exit(data, "dup2", errno, 0);
	}
	close(fd);
}

void	exec_start(t_arg *data, t_token *token)
{
	int		fd_builtin;
	char	*cmd_ex;

	if (token->builtin)
	{
		fd_builtin = make_builtin_dup(data->tokens, data);
		builtin_dup_error_check(fd_builtin, data);
		g_signals.exit_status = start_builtin(data, token);
		my_exit(data, NULL, g_signals.exit_status, 0);
	}
	else
	{
		cmd_ex = get_cmd_arg(data, token->cmd);
		if (execve(cmd_ex, token->cmd, data->env_str) && \
			ft_strcmp(data->tokens->cmd[0], ""))
		{
			my_exit(data, token->cmd[0], 127, 0);
			g_signals.exit_status = 127;
		}
	}
}

int	pipex(t_arg *data)
{
	t_token	*node;
	int		i;

	i = 0;
	create_fd(data);
	node = data->tokens;
	create_pipe_fd(data);
	g_signals.pid = malloc(sizeof(pid_t *) * data->num);
	if (!(g_signals.pid))
		my_exit(data, "malloc", 12, 0);
	while (i < data->num && ft_strcmp(data->tokens->cmd[0], ""))
	{
		check_minishell_in_minishell(node->cmd[0]);
		g_signals.pid[i] = fork();
		if (g_signals.pid[i] < 0)
			my_exit(data, "fork", errno, 0);
		if (g_signals.pid[i] == 0)
			child_process(i, data, data->fd, node);
		i++;
		if (node->next)
			node = node->next;
	}
	close_fds(data, data->fd, NULL);
	waitpid_pipex(data);
	return (0);
}
