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

t_redir	*init_redir(t_arg *args)
{
	t_redir	*redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		my_exit(args, "malloc", 12);
	redir->file_name = NULL;
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
		*redir = new;
	else
		last_redir(*redir)->next = new;
}

int	check_redirect(char *str, int i, t_redir *redir)
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

int	redirect(char *str, int i, t_arg *args, int num)
{
	int		j;
	t_redir	*new;

	new = init_redir(args);
	i = check_redirect(str, i, new);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	j = i;
	while (str[i] && str[i] != ' ' && str[i] != '\t'
		   && str[i] != '>' && str[i] != '<')
		i++;
	new->file_name = lexe(ft_substr(str, j, i - j), args, 1);
	if (!new->file_name)
		my_exit(args, "malloc", 12);
	new->cmd_list_num = num;
	add_redir(&args->redir, new);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '<' || str[i] == '>')
		i = redirect(str, i, args, num);
	return (i);
}
