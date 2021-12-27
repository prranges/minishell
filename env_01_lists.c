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

void	env_read(t_arg *args, char **arge)
{
	int		i;
	t_env	*p;
	int		shlvl;

	shlvl = 1;
	i = -1;
	while (arge[++i])
		add_env(&args->env, arge[i], args);
	p = args->env;
	while (p)
	{
		if (ft_strcmp(p->key, "HOME") == 0)
			args->home = ft_strdup(p->value);
		if (ft_strcmp(p->key, "SHLVL") == 0)
		{
			p->separator = '=';
			shlvl += ft_atoi(p->value);
			free(p->value);
			p->value = ft_itoa(shlvl);
			shlvl = 0;
		}
		p = p->next;
	}
	if (shlvl)
		add_env(&args->env, "SHLVL=1", args);
}

t_env	*last_env(t_env *env)
{
	while (env->next)
		env = env->next;
	return (env);
}

void	add_env(t_env **env, char *str, t_arg *data)
{
	t_env	*new;
	t_env	*temp;
	int		i;

	temp = *env;
	i = 0;
	new = init_env(data);
	while (str[i] && str[i] != '=')
		i++;
	new->key = ft_substr(str, 0, i);
	if (!new->key)
		my_exit(data, "malloc", 12, 0);
	if (str[i] == '=')
	{
		new->separator = '=';
		new->value = ft_strdup(str + i + 1);
		if (!new->value)
			my_exit(data, "malloc", 12, 0);
	}
	if (!temp)
		*env = new;
	else
		last_env(*env)->next = new;
}

void	edit_env(t_env **env, char *str, t_arg *data)
{
	t_env	*p;
	int		i;

	p = *env;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
	{
		p->separator = '=';
		free(p->value);
		p->value = ft_strdup(str + i + 1);
		if (!(p->value))
			my_exit(data, "malloc", 12, 0);
	}
}

void	remove_env(t_arg *args, t_env *remove_list, t_env *prev_p)
{
	t_env	*prev;
	t_env	*remove;

	prev = prev_p;
	remove = remove_list;
	if (prev)
		prev->next = remove->next;
	else
		args->env = args->env->next;
	free(remove_list->key);
	free(remove_list->value);
	free(remove_list);
}
