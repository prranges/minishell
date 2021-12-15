/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_04_env_exit.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 17:41:04 by prranges          #+#    #+#             */
/*   Updated: 2021/12/15 17:41:11 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    env_ms(t_env *env)
{
    t_env    *p;
    
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

int    exit_ms(void)
{
    return (0);
}
