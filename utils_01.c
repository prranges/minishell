/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 16:35:13 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 16:35:15 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list  *add_list(t_list *lst, char *str)
{
    t_list    *temp;
    t_list    *p;
    
    temp = (t_list *)malloc(sizeof(t_list));
    temp->str = ft_strdup(str);
    
    p = lst->next;
    temp->next = p;
    temp->prev = lst;
    lst->next = temp;
    if (p != NULL)
        p->prev = temp;
       
    return (temp);
}

void    print_all_lists(t_list *lst)
{
    t_list *p;
    
    p = lst;
    while (p != NULL)
    {
        printf("list - %s\n", p->str);
        p = p->next;
    }
}

void    delete_all_lists(t_list *lst)
{
    t_list *p;
    
    while (lst->next != NULL)
    {
        p = lst->next;
        lst->next = p->next;
        free(p);
    }
}
