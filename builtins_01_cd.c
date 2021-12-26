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

int	print_cd_error(char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(path);
	return (1);
}

void	change_PWD_OLDPWD(t_env	*p, char *old_pwd)
{
	while (p)
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

int	dir_und_pwds(t_arg *args, char *path)
{
	t_env	*p;
	char	*old_pwd;

	p = args->env;
	old_pwd = NULL;
	while (p)
	{
		if (ft_strcmp(p->key, "PWD") == 0)
			old_pwd = ft_strdup(p->value);
		p = p->next;
	}
	if (!chdir(path))
	{
		p = args->env;
		change_PWD_OLDPWD(p, old_pwd);
	}
	else
		return (print_cd_error(path));
	free(path);
	return (0);
}

int	print_find_path_error(void)
{
	ft_putstr_fd("cd: HOME not set\n", 2);
	return (1);
}

int	cd_ms(t_arg *args, t_token *token)
{
	char	*path;
	char	**cmd;
	t_env	*env;

	env = args->env;
	path = NULL;
	cmd = token->cmd;
	cmd++;
	if (!*(cmd))
	{
		while (env)
		{
			if (ft_strcmp(env->key, "HOME") == 0)
				path = ft_strdup(env->value);
			env = env->next;
		}
		if (!path)
			return (print_find_path_error());
	}
	else if (*cmd[0] == '~')
		path = ft_strjoin(args->home, ft_strdup(*cmd + 1));
	else
		path = ft_strdup(*cmd);
	return (dir_und_pwds(args, path));
}
