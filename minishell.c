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

void	init_args(t_arg *args)
{
	args->tokens = NULL;
	args->num = 0;
	args->redir = NULL;
	args->env = NULL;
	args->errnum = 99;
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
		export_ms(args);
	else if (args->tokens->builtin == UNSET)
		unset_ms(args);
	else if (args->tokens->builtin == ENV)
		env_ms(args->env);
	else if (args->tokens->builtin == PWD)
		pwd_ms(args);
	else if (args->tokens->builtin == ECHO)
		echo_ms(args);
	else if (args->tokens->builtin == CD)
		cd_ms(args);
	else if (args->tokens->builtin == EXIT)
		exit_ms();
	return (0);
}

int	main(int argc, char **argv, char **arge)
{
	char	*str;
	int		num;
	t_arg	*args;

	(void)argc;
	(void)argv;
	args = (t_arg *)malloc(sizeof(t_arg));
	init_args(args);
	env_read(args, arge);
	signals_ms();
	while (1)
	{
		num = 0;
		args->num = 0;
		if (!(str = readline("\033[0;36m\033[1mminishell-0.40$ \033[0m")))
			exit (1);
		add_history(str);
		if (preparcer(str) == 1)
			printf("minishell: syntax error near unexpected token\n");
		else if (preparcer(str) == 2)
			printf("minishell: syntax error near unclosed quotes\n");
		else if (!preparcer(str))
			parcer(str, &num, args);
//		print_all_lists(args);
		if (num)
		{
			if (args->tokens->builtin)
				start_builtin(args);
			else
				pipex(argc, argv, arge, args);
		}
//		if ((ft_strcmp(args->tokens->cmd[0], "")) == 0)
//			printf("HELLO\n");
		free(str);
		delete_all_redirs(args);
		delete_all_tokens(args);
	}
	return (0);
}
