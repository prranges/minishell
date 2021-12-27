/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_02.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:23:06 by prranges          #+#    #+#             */
/*   Updated: 2021/12/26 20:23:13 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_number_of_parts(t_arg *args, char *s)
{
	int	i;
	int	count;

	args->flag_sq = 2;
	args->flag_dq = 2;
	i = -1;
	count = 0;
	if (s[0] && s[0] != ' ' && s[0] != '\t' && s[0] != '>' && s[0] != '<')
		count++;
	while (s[++i])
	{
		if ((s[i] == ' ' || s[i] == '\t') && (s[i + 1] != ' '
				&& s[i + 1] != '\t' && s[i + 1] != '>'
				&& s[i + 1] != '<' && s[i + 1] != '\0')
			&& (!(args->flag_sq % 2)) && (!(args->flag_dq % 2)))
			count++;
		if ((s[i] == '<' || s[i] == '>')
			&& (s[i + 1] == ' ' || s[i + 1] == '\t'))
			count--;
		set_quotes_flag(args, s, i);
	}
	if (!count)
		count = 1;
	return (count);
}

void	if_space_tab(t_arg *args, char *s, int i, int **start_end_i)
{
	if ((s[i] == ' ' || s[i] == '\t'))
	{
		if (i != 0 && (s[i - 1] != ' ' && s[i - 1] != '\t'))
			start_end_i[1][args->ed++] = i;
		if ((s[i + 1] != ' ' && s[i + 1] != '\t' && s[i + 1] != '>'
				&& s[i + 1] != '<' && s[i + 1] != '\0'))
			start_end_i[0][args->st++] = i + 1;
	}
}

void	if_first_char(t_arg *args, char *s, int i, int **start_end_i)
{
	if (s[0] != '\0' && s[0] != ' ' && s[0] != '\t'
		&& s[i + 1] != '>' && s[i + 1] != '<')
		start_end_i[0][args->st++] = 0;
}

void	if_last_char(t_arg *args, char *s, int i, int **start_end_i)
{
	if ((s[i] == '\0') && (s[i - 1] != ' ' && s[i - 1] != '\t')
		&& (!(args->flag_sq % 2)) && (!(args->flag_dq % 2)))
		start_end_i[1][args->ed++] = i;
}

void	find_parts_of_str(char *s, int **start_end_i, t_arg *args, int num)
{
	int	i;

	i = -1;
	args->st = 0;
	args->ed = 0;
	args->flag_sq = 2;
	args->flag_dq = 2;
	if_first_char(args, s, i, start_end_i);
	while (s[++i])
	{
		if (!(args->flag_sq % 2) && !(args->flag_dq % 2))
		{
			if_space_tab(args, s, i, start_end_i);
			if ((s[i] == '>' || s[i] == '<'))
			{
				if (i != 0 && (s[i - 1] != ' ' && s[i - 1] != '\t'))
					start_end_i[1][args->ed++] = i;
				i = redirect(s, i, args, num);
				start_end_i[0][args->st++] = i;
			}
		}
		set_quotes_flag(args, s, i);
	}
	if_last_char(args, s, i, start_end_i);
}
