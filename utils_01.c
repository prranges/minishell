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

int	free_double_massive(char **str)
{
	if (!*str)
		return (0);
	while (*str)
		free(*str);
	free(str);
	return (0);
}

t_token  *add_token(t_token *lst, char **cmd)
{
    t_token    *temp;
    t_token    *p;
    
    temp = (t_token *)malloc(sizeof(t_token));
	temp->cmd = cmd;
	
	temp->type = lst->type + 1;
	
	printf("%d\n", temp->type);
	print_double_massive(temp->cmd); //распечатка двумерника
		
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
	temp->key = ft_substr(str, 0, i);
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
		while (*p->cmd)
		{
			printf("token - %s\n", *p->cmd);
			p->cmd++;
		}
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
		free_double_massive(p->cmd);
        free(p);
		free(lst);
    }
}








void	print_double_massive(char **sub_strs)
{
	while (*sub_strs)
	{
		printf("sub_str - %s\n", *sub_strs);
		sub_strs++;
	}
}
