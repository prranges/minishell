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

void	env_read(t_arg *args, char **arge)
{
	int	i;

	i = -1;
	while (arge[++i])
		add_env(&args->env, arge[i]);
}

t_env	*last_env(t_env *env)
{
	while (env->next)
		env = env->next;
	return (env);
}

void	add_env(t_env **env, char *str)
{
	t_env	*new;
	t_env	*temp;
	int		i;

	temp = *env;
	i = 0;
	new = init_env();
	while (*str && str[i] != '=')
		i++;
	new->key = ft_substr(str, 0, i);
	if (str[i] == '=')
	{
		new->separator = '=';
		new->value = ft_strdup(str + i + 1);
	}
	if (!temp)
	{
		*env = new;
	}
	else
		last_env(*env)->next = new;
	
}

void	print_env(t_env *env)
{
	t_env	*p;

	p = env;
	while (p != NULL)
	{
		printf("%s", p->key);
		printf("%c", p->separator);
		printf("%s\n", p->value);
		p = p->next;
	}
}
