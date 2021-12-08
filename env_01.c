/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 09:31:30 by prranges          #+#    #+#             */
/*   Updated: 2021/12/06 09:31:32 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(void)
{
	t_env	*lst;

	lst = (t_env *)malloc(sizeof(t_env));
	lst->key = NULL;
	lst->value = NULL;
	lst->separator = 0;
	lst->next = NULL;
	return (lst);
}

void	env_read(t_env *env, char **arge)
{
	int	i;

	i = -1;
	while (arge[++i])
		env = add_env(env, arge[i]);
}

t_env	*add_env(t_env *lst, char *str)
{
	t_env	*temp;
	t_env	*p;
	int		i;

	i = 0;
	temp = (t_env *)malloc(sizeof(t_env));
	while (*str && str[i] != '=')
		i++;
	temp->key = ft_substr(str, 0, i);
	if (str[i] == '=')
	{
		temp->separator = '=';
		temp->value = ft_strdup(str + i + 1);
	}
	p = lst->next;
	temp->next = p;
	lst->next = temp;
	return (temp);
}

void	print_env(t_env *lst)
{
	t_env	*p;

	p = lst->next;
	while (p != NULL)
	{
		printf("%s", p->key);
		printf("%c", p->separator);
		printf("%s\n", p->value);
		p = p->next;
	}
}
