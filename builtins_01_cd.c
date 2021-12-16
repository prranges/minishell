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

int	change_dir_und_pwds(t_arg *args, char *path)
{
	t_env	*p;
	char	*old_pwd;
	
	p = args->env;
	old_pwd = NULL;
	while(p)
	{
		if (ft_strcmp(p->key, "PWD") == 0)
			old_pwd = ft_strdup(p->value);
		p = p->next;
	}
	if (!chdir(path))
	{
		p = args->env;
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
	else
	{
		printf("minishell: %s: %s\n", path, strerror(errno));
		free(path);
		return (1);
	}
	free(path);
	return (0);
}

int    cd_ms(t_arg *args)
{
	char	*path;
	char	**cmd;
	t_env	*env;
	
	env = args->env;
	path = NULL;
	cmd = args->tokens->cmd;
	cmd++;
	if (!*(cmd))
	{
		while(env)
		{
			if (ft_strcmp(env->key, "HOME") == 0)
				path = ft_strdup(env->value);
			env = env->next;
		}
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else if (*cmd[0] == '~')
		path = ft_strjoin(args->home, ft_strdup(*cmd + 1));
	else
		path = ft_strdup(*cmd);
	return (change_dir_und_pwds(args, path));
}
