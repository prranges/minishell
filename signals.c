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

void	signal_child(int signal)
{
	if (signal == SIGINT)
		exit(2);
	if (signal == SIGQUIT)
		exit(3);
}

void	signal_pipex(int signal)
{
	if (signal == SIGINT)
		write(2, "\n", 1);
	if (signal == SIGQUIT)
		write(2, "Quit: 3\n", 8);
}

void	signal_main(int signal)
{
	(void)signal;
	rl_on_new_line();
	rl_redisplay();
	write(1, "  \b\b\n", 5);
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	signals_ms(int i)
{
	if (i == MAIN)
	{
		signal(SIGINT, signal_main);
		signal(SIGQUIT, SIG_IGN);
	}
	if (i == CHILD)
	{
		signal(SIGINT, signal_child);
		signal(SIGQUIT, signal_child);
	}
	if (i == PIPEX)
	{
		signal(SIGINT, signal_pipex);
		signal(SIGQUIT, signal_pipex);
	}
}
