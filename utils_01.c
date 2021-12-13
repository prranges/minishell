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

//int	free_double_array(char **str)
//{
//	if (!*str)
//		return (0);
//	while (*str)
//		free(*str);
//	free(str);
//	return (0);
//}

void	print_all_lists(t_arg *args)
{
	int	i;
	t_arg *p;

	p = args;
	printf("Number of arguments - %d\n\n", p->num);
	while (p->tokens)
	{
		i = 0;
		printf("Token number - %d\n", p->tokens->list_num);
		while (p->tokens->cmd[i])
		{
			printf("str[%d] - %s\n", i, p->tokens->cmd[i]);
			i++;
		}
		if (p->tokens->in)
		{
			if (p->tokens->in->dbl)
				printf("double ");
			printf("redirect IN - %s\n", p->tokens->in->file_name);
		}
		if (p->tokens->out)
		{
			if (p->tokens->out->dbl)
				printf("double ");
			printf("redirect OUT - %s\n", p->tokens->out->file_name);
		}
		if (p->tokens->next)
			p->tokens = p->tokens->next;
		else
			break ;
		printf("\n");
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
