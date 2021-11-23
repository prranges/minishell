/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 15:48:07 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 15:48:09 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*init_list(void)
{
	t_list *lst;
	
	lst = (t_list *)malloc(sizeof(t_list));
	lst->type = 0;
	lst->str = NULL;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}

void	find_parts_of_str(char *str)
{
	int	i;
	int count;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = - 1;
	count = 0;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t')
		count++;
	while (str[++i])
	{
		if ((str[i] == ' ' || str[i] == '\t') && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '\0') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
		{
			count++;
		}
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if ((flag_sq % 2) || (flag_dq % 2))
	{
		printf("Error: unclosed quotes\n");
		exit (1);
	}
	else
		printf("NUM OF ARGs - %d\n", count);
//	return (strs);
}

int main(int argc, char **argv, char **env)
{
	char	*str;
//	char	**strs;
	int		status;
	
	(void)argc;
	(void)argv;
	t_list *arguments = NULL;
	arguments = init_list();
	signals_ms();
	
	while(1)
	{
		if (!(str = readline("\033[0;36m\033[1mminishell-0.1$ \033[0m")))
			exit (1);
		add_history(str);
		find_parts_of_str(str);//, strs);
		str = lexe(str, env);
		
		printf("STR: %s\n", str);
//		parce(arguments, str, env);
//		print_all_lists(arguments);
		free(str);
		status = execute(arguments);
		delete_all_lists(arguments);
	}
    return (status);
}
