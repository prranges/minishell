/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_05_pwd_echo.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 20:26:53 by prranges          #+#    #+#             */
/*   Updated: 2021/12/15 20:26:55 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_n(char *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "-n") == 0)
		return (1);
	while (cmd[i])
	{
		while (cmd[i] == ' ' || cmd[i] == '\t')
			i++;
		if (cmd[i] == '-')
		{
			while (cmd[++i] == 'n')
				;
			if (!cmd[i])
				return (1);
			return (0);
		}
		i++;
	}
	return (0);
}

void	echo_print_args(char **cmd)
{
	while (*cmd)
	{
		if (*(cmd + 1))
		{
			ft_putstr_fd(*cmd, 1);
			ft_putstr_fd(" ", 1);
		}
		else if (*cmd)
			ft_putstr_fd(*cmd, 1);
		cmd++;
	}
}

int	echo_ms(t_arg *args, t_token *token)
{
	char	**cmd;
	int		n;

	(void)args;
	n = 0;
	cmd = token->cmd;
	if (*(cmd + 1))
	{
		cmd++;
		while (check_n(*cmd))
		{
			n = 1;
			cmd++;
		}
		echo_print_args(cmd);
	}
	if (!n)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	pwd_ms(t_arg *args)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		my_exit(args, "getwcd", errno, 0);
	ft_putstr_fd(pwd, 1);
	ft_putstr_fd("\n", 1);
	free(pwd);
	return (0);
}
