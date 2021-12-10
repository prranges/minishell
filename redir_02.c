/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_02.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 14:57:33 by prranges          #+#    #+#             */
/*   Updated: 2021/12/09 14:57:35 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    delete_all_redirs(t_arg *args)
{
	t_redir *p;

	while (args->redir)
	{
		p = args->redir;
		args->redir = args->redir->next;
		free(p->file_name);
		free(p);
	}
}

void    add_redirs_to_cmd(t_redir *redir, t_token *tokens)
{
	int i;
	int	cmd_num;

	if (!tokens)
		return ;
	i = 1;
	while (redir)
	{
		cmd_num = redir->cmd_list_num;
		while (i != cmd_num)
		{
			tokens = tokens->next;
			i++;
		}
		if (redir->out_in)
			tokens->in = redir;
		else
			tokens->out = redir;
		redir = redir->next;
	}
}
