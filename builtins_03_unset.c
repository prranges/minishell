/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_03_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 17:33:57 by prranges          #+#    #+#             */
/*   Updated: 2021/12/15 17:34:02 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_unset_error(char *cmd)
{
	g_signals.exit_status = 1;
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

void	find_env_und_remove(t_arg *args, char	**cmd)
{
	t_env	*prev_p;
	t_env	*p;

	prev_p = NULL;
	p = args->env;
	while (p)
	{
		if ((ft_strcmp(p->key, *cmd)) == 0)
			remove_env(args, p, prev_p);
		prev_p = p;
		p = p->next;
	}
}

int	unset_ms(t_arg *args, t_token *token)
{
	t_env	*remove_list;
	char	**cmd;

	remove_list = NULL;
	g_signals.exit_status = 0;
	cmd = token->cmd;
	if (!args->env)
		return (1);
	cmd++;
	while (*cmd)
	{
		if (!ft_isalpha(*cmd[0]) && *cmd[0] != '_')
			return (print_unset_error(*cmd));
		else
			find_env_und_remove(args, cmd);
		cmd++;
	}
	return (0);
}
