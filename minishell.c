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

int main(int argc, char **argv, char **env)
{
	char	*str;
	int		status;
	
	(void)argc;
	(void)argv;
	t_list *arguments = NULL;
	arguments = init_list();
	signals_ms();
	while(1)
	{
		if (!(str = readline("minishell-0.1$ ")))
			exit (1);
		add_history(str);
		str = lexe(str, env);
		parce(arguments, str, env);
		print_all_lists(arguments);
		free(str);
		status = execute(arguments);
		delete_all_lists(arguments);
	}
    return (status);
}
