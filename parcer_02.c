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

int	find_number_of_parts(char *str)
{
	int	i;
	int	count;
	int	flag_sq = 2;
	int	flag_dq = 2;

	i = - 1;
	count = 0;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t' && str[0] != '>' && str[0] != '<')
		count++;
	while (str[++i])
	{
		if ((str[i] == ' ' || str[i] == '\t')
		&& (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '>'
		&& str[i + 1] != '<' && str[i + 1] != '\0') && (!(flag_sq % 2))
		&& (!(flag_dq % 2)))
			count++;
		if ((str[i] == '<' || str[i] == '>') && (str[i + 1] == ' ' || str[i + 1] == '\t'))
			count--;
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if (!count)
		count = 1;
	return (count);
}

void	find_parts_of_str(char *str, int **start_end_i, t_arg *args, int num)
{
	int	i;
	int s = 0;
	int e = 0;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = -1;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t' && str[i + 1] != '>' && str[i + 1] != '<')
		start_end_i[0][s++] = 0;
	while (str[++i])
	{
		if (!(flag_sq % 2) && !(flag_dq % 2))
		{
			if ((str[i] == ' ' || str[i] == '\t'))
			{
				if (i != 0 && (str[i - 1] != ' ' && str[i - 1] != '\t'))
					start_end_i[1][e++] = i;
				if ((str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '>' && str[i + 1] != '<' && str[i + 1] != '\0'))
					start_end_i[0][s++] = i + 1;
			}
			if ((str[i] == '>' || str[i] == '<'))
			{
				if (i != 0 && (str[i - 1] != ' ' && str[i - 1] != '\t'))
					start_end_i[1][e++] = i;
				i = redirect(str, i, args, num);
				start_end_i[0][s++] = i;
			}
		}
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if ((str[i] == '\0') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
		start_end_i[1][e++] = i;
}
