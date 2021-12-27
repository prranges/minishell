/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 15:48:07 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 15:48:09 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_signals;

void	init_args(t_arg *args)
{
	args->tokens = NULL;
	args->num = 0;
	args->redir = NULL;
	args->env = NULL;
	args->env_str = NULL;
	args->fd = NULL;
	args->home = NULL;
}

int	start_builtin(t_arg *args, t_token *token)
{
	if (token->builtin == EXPORT)
		return (export_ms(args, token));
	else if (token->builtin == UNSET)
		return (unset_ms(args, token));
	else if (token->builtin == ENV)
		return (env_ms(args->env));
	else if (token->builtin == PWD)
		return (pwd_ms(args));
	else if (token->builtin == ECHO)
		return (echo_ms(args, token));
	else if (token->builtin == CD)
		return (cd_ms(args, token));
	else if (token->builtin == EXIT)
		return (exit_ms(args, token));
	return (0);
}

void	read_and_parce_input(t_arg *args, char *str)
{
	args->num = 0;
	sig_init();
	str = readline("🔷 minishell-0.80$ ");
	if (!str)
	{
		ft_putstr_fd("\033[A🔷 minishell-0.80$ exit\n", 1);
		rl_redisplay();
		exit (0);
	}
	else if (str && str[0])
		add_history(str);
	if (preparcer(str) == 1)
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
	else if (preparcer(str) == 2)
		ft_putstr_fd("minishell: syntax error near unclosed quotes\n", 2);
	else if (!preparcer(str))
		parcer(str, args);
	free(str);
}

int	main(int argc, char **argv, char **arge)
{
	char	*str;
	t_arg	*args;

	(void)argc;
	(void)argv;
	str = NULL;
	g_signals.name = find_name_ms(argv[0]);
	args = (t_arg *)malloc(sizeof(t_arg));
	if (!args)
		my_exit(args, "malloc", 12, 0);
	init_args(args);
	env_read(args, arge);
	g_signals.exit_status = 0;
	while (1)
	{
		read_and_parce_input(args, str);
		env_lists_to_str(args);
		precreate_or_preopen(args);
		if (args->num == 1 && args->tokens->builtin && !args->redir)
			g_signals.exit_status = start_builtin(args, args->tokens);
		else
			pipex(args);
		free_all(args);
	}
	return (0);
}
