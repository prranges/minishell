/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_02.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 19:01:30 by prranges          #+#    #+#             */
/*   Updated: 2021/12/16 19:01:32 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*init_env(t_arg *args)
{
	t_env	*lst;

	lst = (t_env *)malloc(sizeof(t_env));
	if (!lst)
		my_exit(args, "malloc", 12, 0);
	lst->key = NULL;
	lst->value = NULL;
	lst->separator = 0;
	lst->next = NULL;
	return (lst);
}

int	env_lists_len(t_env *env)
{
	t_env	*p;
	int		len;

	p = env;
	len = 0;
	while (p)
	{
		len++;
		p = p->next;
	}
	return (len);
}

char	*join_value(char *str, t_env *envp, t_arg *args)
{
	char	*tmp;

	tmp = ft_strjoin(str, "=");
	if (!tmp)
		my_exit(args, "malloc", 12, 0);
	free(str);
	if (envp->value)
	{
		str = ft_strjoin(tmp, envp->value);
		if (!str)
			my_exit(args, "malloc", 12, 0);
		free(tmp);
	}
	else
	{
		str = ft_strdup(tmp);
		if (!str)
			my_exit(args, "malloc", 12, 0);
		free(tmp);
	}
	return (str);
}

void	create_env_array(t_arg *args, t_env *envp, int len)
{
	t_arg	*p;
	char	**env_str;
	char	*str;
	int		i;

	p = args;
	envp = args->env;
	i = 0;
	env_str = p->env_str;
	while (i < len)
	{
		str = ft_strdup(envp->key);
		if (!str)
			my_exit(args, "malloc", 12, 0);
		if (envp->separator == '=')
			str = join_value(str, envp, args);
		env_str[i] = ft_strdup(str);
		if (!env_str[i])
			my_exit(args, "malloc", 12, 0);
		free(str);
		i++;
		envp = envp->next;
	}
	env_str[i] = NULL;
}

void	env_lists_to_str(t_arg *args)
{
	t_env	*env;
	int		len;

	env = args->env;
	len = env_lists_len(env);
	args->env_str = malloc(sizeof(char *) * (len + 1));
	if (!args->env_str)
		my_exit(args, "malloc", 12, 0);
	create_env_array(args, args->env, len);
}
