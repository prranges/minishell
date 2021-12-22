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
	t_token *p;

	p = args->tokens;
	printf("Number of arguments - %d\n\n", args->num);
	while (p)
	{
		i = 0;
		printf("Token number - %d\n", p->list_num);
		while (p->cmd[i])
		{
			printf("str[%d] - %s\n", i, p->cmd[i]);
			i++;
		}
		if (p->in)
		{
			if (p->in->dbl)
				printf("double ");
			printf("redirect IN - %s\n", p->in->file_name);
		}
		if (p->out)
		{
			if (p->out->dbl)
				printf("double ");
			printf("redirect OUT - %s\n", p->out->file_name);
		}
		if (p->next)
			p = p->next;
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
