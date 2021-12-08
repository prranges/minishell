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

int	free_double_array(char **str)
{
	if (!*str)
		return (0);
	while (*str)
		free(*str);
	free(str);
	return (0);
}

t_token	*add_token(t_token *lst, char **cmd)
{
	t_token	*temp;
	t_token	*p;

	temp = (t_token *)malloc(sizeof(t_token));
	temp->cmd = cmd;
	temp->list_num = lst->list_num + 1;
	printf("%d\n", temp->list_num);
	print_double_array(temp->cmd);
	p = lst->next;
	temp->next = p;
	temp->prev = lst;
	lst->next = temp;
	if (p != NULL)
		p->prev = temp;
	return (temp);
}

void	print_all_lists(t_token *lst)
{
	t_token	*p;

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

void	delete_all_lists(t_token *lst)
{
	t_token	*p;

	while (lst->next != NULL)
	{
		p = lst->next;
		lst->next = p->next;
		free_double_array(p->cmd);
		free(p);
		free(lst);
	}
}

void	print_double_array(char **sub_strs)
{
	while (*sub_strs)
	{
		printf("sub_str - %s\n", *sub_strs);
		sub_strs++;
	}
}
