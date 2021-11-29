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

t_token  *add_token(t_token *lst, char *str)
{
    t_token    *temp;
    t_token    *p;
    
    temp = (t_token *)malloc(sizeof(t_token));
    temp->str = ft_strdup(str);
    
    p = lst->next;
    temp->next = p;
    temp->prev = lst;
    lst->next = temp;
    if (p != NULL)
        p->prev = temp;
       
    return (temp);
}

t_env 	*add_env(t_env *lst, char *str)
{
    t_env	*temp;
    t_env	*p;
	int		i;
    
	i = 0;
    temp = (t_env *)malloc(sizeof(t_env));
	while (*str && str[i] != '=')
		i++;
	temp->key = ft_substr(str, 0, i - 1);
	if (str[i] == '=')
	{
		temp->separator = '=';
		temp->value = ft_strdup(str + i + 1);
	}
	
    p = lst->next;
    temp->next = p;
    lst->next = temp;

    return (temp);
}

void    print_all_lists(t_token *lst)
{
    t_token *p;
    
    p = lst->next;
    while (p != NULL)
    {
        printf("token - %s\n", p->str);
        p = p->next;
    }
}

void    print_env(t_env *lst)
{
    t_env *p;
    
    p = lst->next;
    while (p != NULL)
    {
        printf("%s", p->key);
		printf("%c", p->separator);
		printf("%s\n", p->value);
        p = p->next;
    }
}

void    delete_all_lists(t_token *lst)
{
    t_token *p;
    
    while (lst->next != NULL)
    {
        p = lst->next;
        lst->next = p->next;
        free(p);
    }
}
