/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_03_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 17:33:57 by prranges          #+#    #+#             */
/*   Updated: 2021/12/15 17:34:02 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    unset_ms(t_arg *args)
{
    t_env    *remove_list;
    char    **cmd;
    t_env    *p;
    t_env    *prev_p;
    
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
            ft_putstr_fd("export: `", 2);
            ft_putstr_fd(*cmd, 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
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
