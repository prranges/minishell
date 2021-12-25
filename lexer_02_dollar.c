/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_02_dollar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 15:44:47 by prranges          #+#    #+#             */
/*   Updated: 2021/12/10 15:44:50 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_key(char c)
{
	if (c != '_' && !ft_isalnum(c))
		return (0);
	return (1);
}

void	if_no_key_in_env(char *str, int *i, t_lexer *lex) //добавить exit(12) под malloc
{
	if (ft_isalpha(lex->key[0]) || !lex->key[1])
	{
		lex->in = ft_strdup("");
//		if (!lex->in)
//			my_exit(args, "malloc", errno);
	}

	else
	{
			lex->in = ft_substr(str, lex->j + 2, *i - lex->j - 1);
//			if (!lex->in)
//				my_exit(args, "malloc", errno);
		}
}

char	*find_key_in_env_lists(char *str, int *i, t_lexer *lex, t_env *p) //добавить exit(12) под malloc
{
	char	*ret;

	lex->before = ft_substr(str, 0, lex->j);
	while (p)
	{
		if ((ft_strcmp(p->key, lex->key)) == 0)
			lex->in = ft_strdup(p->value);
		p = p->next;
	}
	if (!lex->in)
		if_no_key_in_env(str, i, lex);
	lex->after = ft_strdup(str + *i);
	free(str);
	ret = ft_strjoin(lex->before, lex->in);
	*i = (int)ft_strlen(ret) - 1;
	ret = ft_strjoin(ret, lex->after);
	return (ret);
}

char	*if_question(char *str, int *i, t_lexer *lex, t_arg *args)
{
	char	*ret;
	(void)args;

	lex->before = ft_substr(str, 0, lex->j);
	if (!lex->before)
		my_exit(args, "malloc", errno);
	lex->in = ft_itoa(g_signals.exit_status);
	if (!lex->in)
		my_exit(args, "malloc", errno);
	lex->after = ft_strdup(str + *i + 2);
	if (lex->after)
		my_exit(args, "malloc", errno);
	free(str);
	ret = ft_strjoin(lex->before, lex->in);
	if (!ret)
		my_exit(args, "malloc", errno);
	*i = (int)ft_strlen(ret) + 1;
	ret = ft_strjoin(ret, lex->after);
	if (!ret)
		my_exit(args, "malloc", errno);
	return (ret);
}

char	*dollar(char *str, int *i, t_arg *args, t_lexer *lex) //добавить exit(12) под malloc
{
	t_env	*p;
	char	*ret;

	p = args->env;
	init_lexer(lex);
	lex->j = *i;
	lex->e = -1;
	if (str[*i + 1] == '?')
		ret = if_question(str, i, lex, args);
	else
	{
		while (if_key(str[++(*i)]))
			;
		if (*i != lex->j + 1)
			lex->key = ft_substr(str, lex->j + 1, *i - lex->j - 1);
		if (!lex->key)
			my_exit(args, "malloc", errno);
		if (lex->key)
			ret = find_key_in_env_lists(str, i, lex, p);
		else
		{
			free_lexer(lex);
			return (str);
		}
	}
	free_lexer(lex);
	return (ret);
}
