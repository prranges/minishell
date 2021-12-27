/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 16:35:13 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 16:35:15 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_all_env(t_arg *args)
{
	t_env	*p;

	while (args->env)
	{
		p = args->env;
		args->env = args->env->next;
		free(p->key);
		free(p->value);
		free(p);
	}
}

void	free_all(t_arg *args)
{
	delete_all_redirs(args);
	delete_all_tokens(args);
	if (!access("heredoc_file", F_OK))
		unlink("heredoc_file");
}

char	*find_name_ms(char *argv)
{
	int		i;
	char	*res;

	i = (int)ft_strlen(argv) - 1;
	while (argv[i] != '/' && i)
		i--;
	if (argv[i] == '/')
		i++;
	res = ft_strdup(argv + i);
	if (!res)
		exit(12);
	return (res);
}
