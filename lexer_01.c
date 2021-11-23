/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_01.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 12:52:32 by prranges          #+#    #+#             */
/*   Updated: 2021/11/16 12:52:35 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    init_lexer(t_lexer *lex)
{
    lex->j = 0;
    lex->e = 0;
    lex->p = NULL;
    lex->key = NULL;
    lex->before = NULL;
    lex->in = NULL;
    lex->after = NULL;
}

void    free_lexer(t_lexer *lex)
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

char    *single_quotes(char *str, int *i, t_lexer *lex)
{
	init_lexer(lex);
	lex->j = *i;

	char	*ret;
	
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

char	*backslash(char *str, int *i, t_lexer *lex)
{
	char	*ret;
	
	init_lexer(lex);
	lex->before = ft_substr(str, 0, *i);
	lex->after = ft_strdup(str + *i + 1);
	ret = ft_strjoin(lex->before, lex->after);
	*i = (int)ft_strlen(lex->before);
	free_lexer(lex);
	++*i;
	if (ret[*i] == '\\' || ret[*i] == '\'')
		--*i;
	free(str);
	return (ret);
}

char	*double_quotes(char *str, int *i, char **env, t_lexer *lex)
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
		if (str[*i] == '\\' && (str[*i + 1] == '\"' || str[*i + 1] == '$' || str[*i + 1] == '\\'))
		{
			str = backslash(str, i, lex);
			if (str[*i] == '\"')
				--*i;
		}
		if (str[*i] == '$')
			str = dollar(str, i, env, lex);
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

int	ifkey(char c)
{
	if (c != '_' && !ft_isalnum(c))
		return (0);
	return (1);
}

void	if_no_key_in_env(char *str, int *i, t_lexer *lex)
{
	if (ft_isalpha(lex->key[0]) || !lex->key[1])
		lex->in = ft_strdup("");
	else
		lex->in = ft_substr(str, lex->j + 2, *i - lex->j - 1);
}

char	*dollar(char *str, int *i, char **env, t_lexer *lex)
{
	char	*ret = NULL;
	init_lexer(lex);
	lex->j = *i;
	lex->e = -1;
	while (ifkey(str[++(*i)]))
		;
	if (*i != lex->j + 1)
		lex->key = ft_substr(str, lex->j + 1, *i - lex->j - 1);
	if (lex->key)
	{
		lex->before = ft_substr(str, 0, lex->j);
		while (env[++lex->e])
		{
			lex->p = ft_strnstr(env[lex->e], lex->key, ft_strlen(lex->key));
			if (lex->p && lex->p[ft_strlen(lex->key)] == '=')
				lex->in = ft_strdup(env[lex->e] + ft_strlen(lex->key) + 1);
		}
		if (!lex->in)
			if_no_key_in_env(str, i, lex);
		lex->after = ft_strdup(str + *i); //*i - ???
		free(str);
		ret = ft_strjoin(lex->before, lex->in);
		*i = (int)ft_strlen(ret) - 1;
		ret = ft_strjoin(ret, lex->after);
	}
	else
	{
		free_lexer(lex);
		return (str);
	}
	free_lexer(lex);
	return (ret);
}

char    *lexe(char *str, char **env)
{
    t_lexer    *lex;
    int        i;
    lex = (t_lexer *)malloc(sizeof(t_lexer));
    i = -1;
	(void)env;
    while (str[++i])
    {
        if (str[i] == '\'')
            str = single_quotes(str, &i, lex);
        if (str[i] == '\\')
            str = backslash(str, &i, lex);
        if (str[i] == '\"')
            str = double_quotes(str, &i, env, lex);
        if (str[i] == '$')
            str = dollar(str, &i, env, lex);
    }
//    printf("str lexer - %s\n", str);
    return (str);
}
