/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_05_pwd_echo.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 20:26:53 by prranges          #+#    #+#             */
/*   Updated: 2021/12/15 20:26:55 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    echo_ms(t_arg *args)
{
    char    **cmd;
    int        n;
    
    n = 0;
    cmd = args->tokens->cmd;
    cmd++;
    while (ft_strcmp(*cmd, "-n") == 0)
    {
        n = 1;
        cmd++;
    }
    while (*cmd)
    {
        if (*(cmd + 1))
            printf("%s ", *cmd);
        else
            printf("%s", *cmd);
        cmd++;
    }
    if (!n)
        printf("\n");
    return (0);
}

int    pwd_ms(t_arg *args)
{
    (void)args;
    char    *pwd;
    
    pwd = getcwd(NULL, 0);
    if (!pwd)
        exit(1); // exit_ms
    printf("%s\n", pwd);
    free(pwd);
    return (0);
}
