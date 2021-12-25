/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 13:48:57 by prranges          #+#    #+#             */
/*   Updated: 2021/11/16 13:48:59 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int signal)
{
	(void)signal;
	if (g_signals.pid == 0)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		g_signals.exit_status = 1;
	}
	else if (g_signals.pid != 0)
	{
		ft_putstr_fd("\n", 1);
		g_signals.exit_status = 130;
	}
	g_signals.sigint = 1;
}

void	sig_quit(int signal) //доделать
{
	char	*nbr;

	nbr = ft_itoa(signal);
	//if (!nbr)
		//my_exit(data, "malloc", 12);//добавить структуру
	if (g_signals.pid == 0)
	{
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("  \b\b", 1);
		rl_redisplay();
	}
	else if (g_signals.pid != 0)
	{
		ft_putstr_fd("Quit: ", 2);
		ft_putendl_fd(nbr, 2);
		g_signals.exit_status = 131;
		g_signals.sigquit = 1;
	}
	free(nbr);
}

void	sig_init(void)
{
	g_signals.sigint = 0;
	g_signals.sigquit = 0;
	g_signals.pid = 0;
}
