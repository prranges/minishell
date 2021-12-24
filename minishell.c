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
	args->errnum = 99;
	args->home = NULL;
//	args->exit = 0;
}

void	parcer(char *str, int *num, t_arg *args)
{
	int		s;
	char	**sub_strs;
	int		number_of_parts;
	int		**start_end_i;
	char	**cmd;

	sub_strs = make_substrs_pipe_devided(str);
	while (*sub_strs)
	{
		(*num)++;
		number_of_parts = find_number_of_parts(*sub_strs);
		start_end_i = malloc(sizeof(int **));
		start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
		start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
		cmd = malloc(sizeof(char **) * number_of_parts + 1);
		find_parts_of_str(*sub_strs, start_end_i, args, *num);
		s = -1;
		while (number_of_parts > ++s)
		{
			cmd[s] = ft_substr(sub_strs[0], start_end_i[0][s], start_end_i[1][s] - start_end_i[0][s]);
			cmd[s] = lexe(cmd[s], args);
		}
		cmd[s] = NULL;
		args->num = add_token(&args->tokens, cmd);
		free(start_end_i[0]);
		free(start_end_i[1]);
		free(start_end_i);
		sub_strs++;
	}
	add_redirs_to_cmd(args->redir, args->tokens);
}

int	start_builtin(t_arg *args)
{
	if (args->tokens->builtin == EXPORT)
		return (export_ms(args));
	else if (args->tokens->builtin == UNSET)
		return (unset_ms(args));
	else if (args->tokens->builtin == ENV)
		return (env_ms(args->env));
	else if (args->tokens->builtin == PWD)
		return (pwd_ms(args));
	else if (args->tokens->builtin == ECHO)
		return (echo_ms(args));
	else if (args->tokens->builtin == CD)
		return (cd_ms(args));
	else if (args->tokens->builtin == EXIT)
		return (exit_ms(args));
	return (0);
}

void free_all(t_arg *args)
{
	delete_all_redirs(args);
	delete_all_tokens(args);
//	if (!access("heredoc_file", F_OK))
//		unlink("heredoc_file");
}

char	*find_name_ms(char *argv)
{
	int	i;
	i = (int)ft_strlen(argv) - 1;
	while (argv[i] != '/' && i)
		i--;
	if (argv[i] == '/')
		i++;
	return (ft_strdup(argv + i));
}

int	main(int argc, char **argv, char **arge)
{
	char	*str;
	int		num;
	t_arg	*args;

	(void)argc;
	(void)argv;
	rl_outstream = stderr;
	g_signals.name = find_name_ms(argv[0]);
	args = (t_arg *)malloc(sizeof(t_arg));
	init_args(args);
	env_read(args, arge);
	while (1)
	{
		num = 0;
		args->num = 0;
		sig_init();
		signal(SIGINT, &sig_int);
		signal(SIGQUIT, &sig_quit);
		if (!(str = readline("🔷 minishell-0.60$ ")))
		{
			ft_putstr_fd("\033[A🔷 minishell-0.60$ exit\n", 1);
			rl_redisplay();
			exit (1);
		}
		else if (str && str[0])
			add_history(str);
		if (preparcer(str) == 1)
			ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		else if (preparcer(str) == 2)
			ft_putstr_fd("minishell: syntax error near unclosed quotes\n", 2);
		else if (!preparcer(str))
			parcer(str, &num, args);
		free(str);
//		print_all_lists(args);
		env_lists_to_str(args);
		precreate_or_preopen(args);
		if (num == 1 && args->tokens->builtin && !args->redir)
			start_builtin(args);
		else
			pipex(args);
		free_all(args);
	}
	return (0);
}
