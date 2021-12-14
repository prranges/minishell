/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_01_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:52:32 by prranges          #+#    #+#             */
/*   Updated: 2021/11/16 12:52:35 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_lexer(t_lexer *lex)
{
	lex->j = 0;
	lex->e = 0;
	lex->p = NULL;
	lex->key = NULL;
	lex->before = NULL;
	lex->in = NULL;
	lex->after = NULL;
}

void	free_lexer(t_lexer *lex)
{
	if (lex->p)
		lex->p = NULL;
	if (lex->key)
		free(lex->key);
	if (lex->before)
		free(lex->before);
	if (lex->in)
		free(lex->in);
	if (lex->after)
		free(lex->after);
	init_lexer(lex);
}

char	*single_quotes(char *str, int *i, t_lexer *lex)
{
	char	*ret;

	init_lexer(lex);
	lex->j = *i;
	lex->before = ft_substr(str, 0, lex->j);
	while (str[++*i] != '\'')
		;
	lex->in = ft_substr(str, lex->j + 1, *i - lex->j - 1);
	lex->after = ft_strdup(str + *i + 1);
	ret = ft_strjoin(lex->before, lex->in);
	*i = (int)ft_strlen(ret);
	ret = ft_strjoin(ret, lex->after);
	free_lexer(lex);
	free(str);
	if (ret[*i] == '\'')
		ret = single_quotes(ret, i, lex);
	return (ret);
}

//char	*backslash(char *str, int *i, t_lexer *lex)
//{
//	char	*ret;
//
//	init_lexer(lex);
//	lex->before = ft_substr(str, 0, *i);
//	lex->after = ft_strdup(str + *i + 1);
//	ret = ft_strjoin(lex->before, lex->after);
//	*i = (int)ft_strlen(lex->before);
//	free_lexer(lex);
//	++*i;
//	if (ret[*i] == '\\' || ret[*i] == '\'')
//		--*i;
//	free(str);
//	return (ret);
//}

char	*double_quotes(char *str, int *i, t_arg *args, t_lexer *lex)
{
	char	*ret;
	char	*before;
	int		j;

	init_lexer(lex);
	lex->j = *i;
	j = *i;
	before = ft_substr(str, 0, lex->j);
	while (str[++(*i)] != '\"')
	{
//		if (str[*i] == '\\' && (str[*i + 1] == '\"' || str[*i + 1] == '$' || str[*i + 1] == '\\'))
//		{
//			str = backslash(str, i, lex);
//			if (str[*i] == '\"')
//				--*i;
//		}
		if (str[*i] == '$')
			str = dollar(str, i, args, lex);
	}
	lex->in = ft_substr(str, j + 1, *i - j - 1);
	lex->after = ft_strdup(str + *i + 1);
	ret = ft_strjoin(before, lex->in);
	*i = (int)ft_strlen(ret) - 1;
	ret = ft_strjoin(ret, lex->after);
	free(str);
	free_lexer(lex);
	return (ret);
}

char	*lexe(char *str, t_arg *args)
{
	t_lexer	*lex;
	int		i;

	lex = (t_lexer *)malloc(sizeof(t_lexer));
	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'')
			str = single_quotes(str, &i, lex);
//        if (str[i] == '\\')
//            str = backslash(str, &i, lex);
		if (str[i] == '\"')
			str = double_quotes(str, &i, args, lex);
		if (str[i] == '$')
			str = dollar(str, &i, args, lex);
	}
//    printf("str lexer - %s\n", str);
	return (str);
}
