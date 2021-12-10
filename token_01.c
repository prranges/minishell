/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 12:29:51 by prranges          #+#    #+#             */
/*   Updated: 2021/12/10 12:29:56 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_tokens(void)
{
	t_token	*lst;

	lst = (t_token *)malloc(sizeof(t_token));
	lst->list_num = 1;
	lst->cmd = NULL;
	lst->in = NULL;
	lst->out = NULL;
	lst->next = NULL;
	return (lst);
}

t_token	*last_token(t_token *token)
{
	while (token->next)
		token = token->next;
	return (token);
}

int	add_token(t_token **token, char **cmd)
{
	t_token	*new;
	t_token	*temp;

	temp = *token;
	new = init_tokens();
	new->cmd = cmd;
	if (!temp)
		*token = new;
	else
	{
		new->list_num += last_token(*token)->list_num;
		last_token(*token)->next = new;
	}
	return (new->list_num);
}

void	delete_all_tokens(t_arg *args)
{
	t_token	*p;
	int		i;

	while (args->tokens)
	{
		i = 0;
		printf("DELETE\n");
		p = args->tokens;
		args->tokens = args->tokens->next;
		while (p->cmd[i])
		{
			free(p->cmd[i]);
			i++;
		}
		free(p->cmd);
		free(p);
	}
}
