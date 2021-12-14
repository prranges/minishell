/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_02.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 10:32:06 by prranges          #+#    #+#             */
/*   Updated: 2021/11/16 10:32:08 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    unset_ms(t_arg *args)
{
	t_env	*remove_list;
	char	**cmd;
	t_env	*p;
	t_env	*prev_p;
	
	prev_p = NULL;
	remove_list = NULL;
	args->errnum = 0;
	cmd = args->tokens->cmd;
	if (!args->env)
		return (1);
	cmd++;
	while (*cmd)
	{
		if (!ft_isalpha(*cmd[0]) && *cmd[0] != '_')
		{
			args->errnum = 1;
			write(2, "export: `", 9);
			write(2, *cmd, ft_strlen(*cmd));
			write(2, "': not a valid identifier\n", 26);
		}
		else
		{
			p = args->env;
			while (p)
			{
				if ((ft_strcmp(p->key, *cmd)) == 0)
				{
					remove_env(args, p, prev_p);
					break ;
				}
				prev_p = p;
				p = p->next;
			}
		}
		cmd++;
	}
	return (0);
}

t_env	*find_key_str_in_env(t_env *env, char *str)
{
	int	i;
	char *key;
	t_env	*p;
	
	p = env;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	while (p)
	{
		if ((ft_strcmp(p->key, key)) == 0)
		{
			free(key);
			return (p);
		}
		p = p->next;
	}
	free(key);
	return (NULL);
}

int print_export(t_env *env)
{
	t_env	*p;
	
	p = env;
	while (p)
	{
		if ((ft_strcmp(p->key, "_") != 0))
		{
			printf("declare -x %s", p->key);
			printf("%c", p->separator);
			if (p->value)
				printf("\"%s\"", p->value);
			printf("\n");
		}
		p = p->next;
	}
	return (0);
}

int    export_ms(t_arg *args)
{
	t_env	*env_list;
	char	**cmd;
	
	args->errnum = 0;
	cmd = args->tokens->cmd;
	if (!args->env)
		return (1);
	cmd++;
	if (!*cmd)
		print_export(args->env);
	while (*cmd)
	{
		env_list = find_key_str_in_env(args->env, *cmd);
		if (env_list)
			edit_env(&env_list, *cmd);
		else if (ft_isalpha(*cmd[0]) || *cmd[0] == '_')
			add_env(&args->env, *cmd);
		else
		{
			args->errnum = 1;
			write(2, "export: `", 9);
			write(2, *cmd, ft_strlen(*cmd));
			write(2, "': not a valid identifier\n", 26);
		}
		cmd++;
	}
	return (0);
}

int	env_ms(t_env *env)
{
	t_env	*p;
	
	if (!env)
		return (1);
	p = env;
	while (p)
	{
		if (p->separator == '=')
		{
			printf("%s", p->key);
			printf("%c", p->separator);
			if (p->value)
				printf("%s", p->value);
			printf("\n");
		}
		p = p->next;
	}
	return (0);
}
