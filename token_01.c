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

t_token	*init_tokens(t_arg *args)
{
	t_token	*lst;

	lst = (t_token *)malloc(sizeof(t_token));
	if (!lst)
		my_exit(args, "malloc", 12, 0);
	lst->list_num = 1;
	lst->cmd = NULL;
	lst->in = NULL;
	lst->out = NULL;
	lst->builtin = 0;
	lst->next = NULL;
	return (lst);
}

t_token	*last_token(t_token *token)
{
	while (token->next)
		token = token->next;
	return (token);
}

int	check_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "export") == 0)
		return (EXPORT);
	if (ft_strcmp(cmd, "unset") == 0)
		return (UNSET);
	if (ft_strcmp(cmd, "env") == 0)
		return (ENV);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (PWD);
	if (ft_strcmp(cmd, "echo") == 0)
		return (ECHO);
	if (ft_strcmp(cmd, "cd") == 0)
		return (CD);
	if (ft_strcmp(cmd, "exit") == 0)
		return (EXIT);
	return (0);
}

int	add_token(t_token **token, char **cmd, t_arg *args)
{
	t_token	*new;
	t_token	*temp;

	temp = *token;
	new = init_tokens(args);
	new->cmd = cmd;
	new->builtin = check_builtin(cmd[0]);
	if (!temp)
		*token = new;
	else
	{
		new->list_num += last_token(*token)->list_num;
		last_token(temp)->next = new;
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
		p = args->tokens;
		args->tokens->list_num = 0;
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
