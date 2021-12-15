/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_01_cd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:30:28 by prranges          #+#    #+#             */
/*   Updated: 2021/11/16 10:30:30 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_dir_und_pwds(t_env *env, char *path)
{
	t_env	*p;
	char	*old_pwd;
	
	p = env;
	old_pwd = NULL;
	while(p)
	{
		if (ft_strcmp(p->key, "PWD") == 0)
			old_pwd = ft_strdup(p->value);
		p = p->next;
	}
	if (!(chdir(path)))
	{
		p = env;
		while(p)
		{
			if (ft_strcmp(p->key, "OLDPWD") == 0)
			{
				free(p->value);
				p->value = old_pwd;
			}
			if (ft_strcmp(p->key, "PWD") == 0)
			{
				free(p->value);
				p->value = getcwd(NULL, 0);
			}
			p = p->next;
		}
	}
	free(path);
}

int    cd_ms(t_arg *args) // relative or absolute path
{
	char	*path;
	char	**cmd;
	t_env	*env;
	
	args->errnum = 0;
	env = args->env;
	path = NULL;
	cmd = args->tokens->cmd;
	cmd++;
	if (!(*cmd))
	{
		while(env)
		{
			if (ft_strcmp(env->key, "HOME") == 0)
			{
				path = ft_strdup(env->value);
			}
			env = env->next;
		}
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else
	{
		path = ft_strdup(*cmd);
	}
	change_dir_und_pwds(env, path);
	return (0);
}
