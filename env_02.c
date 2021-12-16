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

int env_lists_len(t_env *env)
{
    t_env   *p;
    int     len;
    
    p = env;
    len = 0;
    while (p)
    {
        len++;
        p = p->next;
    }
    return (len);
}

void	create_env_array(t_arg *args, t_env *envp, int len)
{
	t_arg	*p;
	char	**env_str;
	char	*str;
	char	*tmp;
	int		i;
	
	p = args;
	envp = args->env;
	i = 0;
	env_str = p->env_str;
    while (i < len)
	{
		str = ft_strdup(envp->key);
		tmp = ft_strjoin(str, "=");
		free(str);
		str = ft_strjoin(tmp, envp->value);
		free(tmp);
		env_str[i] = ft_strdup(str);
		free(str);
		i++;
		envp = envp->next;
	}
	env_str[i] = NULL;
}

void    env_lists_to_str(t_arg *args)
{
    t_env   *env;
    int     len;
    
    env = args->env;
    len = env_lists_len(env);
    args->env_str = malloc(sizeof(char *) * (len + 1));
    // проверка на маллок
    create_env_array(args, args->env, len);
//	print_double_array(args->env_str);
    
    printf("len - %d\n", len);
}
