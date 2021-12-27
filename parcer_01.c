/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_01.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 16:31:02 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 16:31:05 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_cmd_lists(t_arg *args, char **sub_strs, \
						 int **start_end_i, int num_of_parts)
{
	int		s;
	char	**cmd;

	s = -1;
	cmd = malloc(sizeof(char **) * num_of_parts + 1);
	if (!cmd || !start_end_i || !start_end_i[0] || !start_end_i[1])
		my_exit(args, "malloc", 12, 0);
	while (num_of_parts > ++s)
	{
		cmd[s] = ft_substr(sub_strs[0], start_end_i[0][s], \
						   start_end_i[1][s] - start_end_i[0][s]);
		if (!cmd[s])
			my_exit(args, "malloc", 12, 0);
		cmd[s] = lexe(cmd[s], args, 0);
	}
	cmd[s] = NULL;
	args->num = add_token(&args->tokens, cmd, args);
}

void	parcer(char *str, t_arg *args)
{
	char	**sub_strs;
	int		num_of_parts;
	int		**start_end_i;

	sub_strs = make_substrs_pipe_devided(str, args);
	while (*sub_strs)
	{
		args->num++;
		num_of_parts = find_number_of_parts(args, *sub_strs);
		start_end_i = malloc(sizeof(int **));
		start_end_i[0] = malloc(sizeof(int *) * num_of_parts);
		start_end_i[1] = malloc(sizeof(int *) * num_of_parts);
		if (!start_end_i || !start_end_i[0] || !start_end_i[1])
			my_exit(args, "malloc", 12, 0);
		find_parts_of_str(*sub_strs, start_end_i, args, args->num);
		create_cmd_lists(args, sub_strs, start_end_i, num_of_parts);
		free(start_end_i[0]);
		free(start_end_i[1]);
		free(start_end_i);
		sub_strs++;
	}
	add_redirs_to_cmd(args->redir, args->tokens);
}

void	set_quotes_flag(t_arg *args, char *str, int i)
{
	if (str[i] == '\'' && (!(args->flag_dq % 2)))
		args->flag_sq++;
	if (str[i] == '\"' && (!(args->flag_sq % 2)))
		args->flag_dq++;
}
