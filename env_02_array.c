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

char	*join_value(char *str, t_env *envp)
{
	char	*tmp;

	tmp = ft_strjoin(str, "=");
	free(str);
	if (envp->value)
	{
		str = ft_strjoin(tmp, envp->value);
		free(tmp);
	}
	else
	{
		str = ft_strdup(tmp);
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
		if (envp->separator == '=')
			str = join_value(str, envp);
		env_str[i] = ft_strdup(str);
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
		exit (1);
	create_env_array(args, args->env, len);
}
