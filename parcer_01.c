/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parce_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 16:31:02 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 16:31:05 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void  parce(t_token *lst, char **strs, char **env)
{
    (void)env;
    int i = -1;
    
    while (strs[++i])
		lst = add_token(lst, strs[i]);
}
