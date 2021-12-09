/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparcer_01.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 13:48:16 by prranges          #+#    #+#             */
/*   Updated: 2021/12/08 13:48:17 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_start_and_double_pipe(char *str)
{
	int	i;
	int	count;

	i = 0;
	while (str[i])
	{
		count = 0;
		while (str[i] == ' ' || str[i] == '\t')
			i++;
		while (str[i] != '|' && str[i] != '\0')
		{
			if (str[i] != ' ' && str[i] != '\t')
				count++;
			i++;
		}
		if (count == 0 || (str[i] == '|' && str[i + 1] == '\0'))
			return (1);
		i++;
	}
	return (0);
}

int	check_filename(char *str, int *i, int *count)
{
	(*i)++;
	if (str[*i] == str[*i - 1])
		(*i)++;
	while (str[*i] == ' ' || str[*i] == '\t')
		(*i)++;
	while (str[*i] != ' ' && str[*i] != '\t' && str[*i] != '|'
		   && str[*i] != '>' && str[*i] != '<' && str[*i] != '\0')
	{
		(*count)++;
		(*i)++;
	}
	if (*count == 0)
		return (1);
	return (0);
}

int	check_redirect_without_filename(char *str)
{
	int	i;
	int	count;

	i = 0;
	while (str[i])
	{
		count = 0;
		if (str[i] == '>' || str[i] == '<')
		{
			if (check_filename(str, &i, &count))
				return (1);
		}
		i++;
	}
	return (0);
}

int	check_open_quotes(char *str)
{
	int	i;
	int	flag_sq;
	int	flag_dq;

	i = 0;
	flag_sq = 2;
	flag_dq = 2;
	while (str[i])
	{
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
		i++;
	}
	if ((flag_sq % 2) || (flag_dq % 2))
		return (1);
	return (0);
}

int	preparcer(char *str)
{
	int	i;
	int	count;

	count = 0;
	i = -1;
	if (check_open_quotes(str))
		return (2);
	while (str[++i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			count++;
		if (str[i] == '|')
		{
			if (check_start_and_double_pipe(str))
				return (1);
		}
		if (str[i] == '>' || str[i] == '<')
		{
			if (check_redirect_without_filename(str))
				return (1);
		}
	}
	if (count == 0)
		return (-1);
	return (0);
}
