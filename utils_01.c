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
	int i;
	
	printf("Number of arguments - %d\n\n", args->num);
	while (args->tokens)
	{
		i = 0;
		printf("Token number - %d\n", args->tokens->list_num);
		while (args->tokens->cmd[i])
		{
			printf("str[%d] - %s\n", i, args->tokens->cmd[i]);
			i++;
		}
		if (args->tokens->in)
		{
			if (args->tokens->in->dbl)
				printf("double ");
			printf("redirect IN - %s\n", args->tokens->in->file_name);
		}
		if (args->tokens->out)
		{
			if (args->tokens->out->dbl)
				printf("double ");
			printf("redirect OUT - %s\n", args->tokens->out->file_name);
		}
		args->tokens = args->tokens->next;
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
