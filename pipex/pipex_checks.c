/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:46:30 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:46:31 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_fd_exist(int fd, char *str, t_arg *data)
{
	if (fd == -1)
	{
		printf("minishell: %s: %s\n", str, strerror(errno));
		my_exit(data, str, errno);
	}
}

void	check_minishell_in_minishell(char *cmd)
{
	if (ft_strcmp(find_name_ms(cmd), g_signals.name) == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
}
