/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_04_env_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 17:41:04 by prranges          #+#    #+#             */
/*   Updated: 2021/12/15 17:41:11 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_ms(t_env *env)
{
	t_env	*p;

	if (!env)
		return (1);
	p = env;
	while (p)
	{
		if (p->separator == '=')
		{
			ft_putstr_fd(p->key, 1);
			ft_putchar_fd(p->separator, 1);
			if (p->value)
				ft_putstr_fd(p->value, 1);
			ft_putchar_fd('\n', 1);
		}
		p = p->next;
	}
	return (0);
}

int	exit_ms(t_arg *args, t_token *token)
{
	delete_all_env(args);
	free_double_array(args->env_str);
	if (g_signals.pid == 0)
		ft_putstr_fd("exit\n", 2);
	if (token->cmd[1] && token->cmd[2])
		my_exit(args, NULL, 1, 1);
	else if (token->cmd[1] && ft_strisnum(token->cmd[1]) == 0)
		my_exit(args, token->cmd[1], 255, 2);
	else if (token->cmd[1])
		my_exit(args, NULL, (unsigned char)ft_atoi(token->cmd[1]), 0);
	else
		my_exit(args, NULL, g_signals.exit_status, 0);
	return (0);
}

void	my_exit(t_arg *data, char *text, int errnum, int flag)
{
	char	*error;

	if (text)
	{
		error = strerror(errnum);
		write(2, "minishell: ", 11);
		if (flag == 2)
			ft_putstr_fd("exit: ", 2);
		write(2, text, ft_strlen(text));
		write(2, ": ", 2);
		if (errnum == 127)
			ft_putstr_fd("command not found\n", 2);
		else if (flag == 0)
		{
			write(2, error, ft_strlen(error));
			write(2, "\n", 1);
		}
	}
	if (flag == 1)
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	if (flag == 2)
		ft_putstr_fd("numeric argument required\n", 2);
	free_all(data);
	g_signals.exit_status = errnum;
	exit(errnum);
}
