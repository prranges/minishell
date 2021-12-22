/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_01.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 10:56:13 by prranges          #+#    #+#             */
/*   Updated: 2021/12/06 10:56:15 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	number_of_pipes(char *str)
{
	int	count;
	int	flag_sq;
	int	flag_dq;

	flag_sq = 2;
	flag_dq = 2;
	count = 0;
	while (*str)
	{
		if ((*str == '|') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			count++;
		if (*str == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (*str == '\"' && (!(flag_sq % 2)))
			flag_dq++;
		str++;
	}
	return (count);
}

int	find_pipe(char *str)
{
	int	i;
	int	flag_sq;
	int	flag_dq;

	flag_sq = 2;
	flag_dq = 2;
	i = 0;
	while (str[i])
	{
		if ((str[i] == '|') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			return (i);
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
		i++;
	}
	return (i);
}

char	**make_substrs_pipe_devided(char *str)
{
	int		i;
	int		start;
	int		len;
	char	*p;
	char	**sub_strs;

	i = 0;
	p = str;
	sub_strs = malloc(sizeof(char **) \
					  + (sizeof(char *) * number_of_pipes(p) + 2));
	start = 0;
	while (i < number_of_pipes(p) + 1)
	{
		len = find_pipe(p + start);
		sub_strs[i] = ft_substr(p, start, len);
		start += len + 1;
		i++;
	}
	sub_strs[i] = NULL;
	return (sub_strs);
}
