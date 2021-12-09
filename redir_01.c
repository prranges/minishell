/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 14:52:05 by prranges          #+#    #+#             */
/*   Updated: 2021/12/06 14:52:07 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir *init_redir(void)
{
	t_redir	*redir;
	
	redir = (t_redir *)malloc(sizeof(t_redir));
    redir->file_name = NULL;
    redir->lim = NULL;
    redir->cmd_list_num = 0;
    redir->out_in = 0;
    redir->dbl = 0;
    redir->next = NULL;
	return (redir);
}

t_redir	*last_redir(t_redir *redir)
{
	while (redir->next)
		redir = redir->next;
	return (redir);
}

void	add_redir(t_redir **redir, t_redir *new)
{
	t_redir	*temp;

	temp = *redir;
	if (!temp)
	{
		
		*redir = new;
	}
	else
		last_redir(*redir)->next = new;
}

int check_redirect(char *str, int i, t_redir *redir)
{
    if (str[i + 1] == '>' || str[i + 1] == '<')
    {
        redir->dbl = 1;
        i++;
    }
    if (str[i] == '<')
        redir->out_in = 1;
    i++;
    return (i);
}

int redirect(char *str, int i, t_arg *args, int num)
{
    int j;
    t_redir *new;
    
	new = init_redir();
    i = check_redirect(str, i, new);
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    j = i;
    while (str[i] && str[i] != ' ' && str[i] != '\t' && str[i] != '>' && str[i] != '<')
        i++;
	new->file_name = lexe(ft_substr(str, j, i - j), args->env);
	new->cmd_list_num = num;
	add_redir(&args->redir, new);
	
//  printf("FILE NAME - %s\n", new->file_name);
//	printf("double - %d\n", new->dbl);
//	printf("cmd list # - %d\n", new->cmd_list);
//	printf("out/in - %d\n", new->out_in);
//	printf("\n");
//	printf("FILE NAME2 - %s\n", args->redir->file_name);
//	printf("double2 - %d\n", args->redir->dbl);
//	printf("cmd list2 # - %d\n", args->redir->cmd_list);
//	printf("out/in2 - %d\n", args->redir->out_in);
//	printf("\n");
	
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    if (str[i] == '<' || str[i] == '>')
        i = redirect(str, i, args, num);
    
    return (i);
}
