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
	init_lexer(lex);
	lex->j = *i;
	while (str[++(*i)] != '\'')
		;
	lex->before = ft_substr(str, 0, lex->j);
	printf("before single - %s\n", lex->before);
	lex->in = ft_substr(str, lex->j + 1, *i - lex->j - 1);
	lex->after = ft_strdup(str + *i + 1);
	str = ft_strjoin(lex->before, lex->in);
	str = ft_strjoin(str, lex->after);
	free_lexer(lex);
	return (str);
}

char	*backslash(char *str, int *i, t_lexer *lex)
{
	init_lexer(lex);
	lex->before = ft_substr(str, 0, *i);
	printf("before slash - %s\n", lex->before);
	lex->after = ft_strdup(str + *i + 1);
	printf("after slash - %s\n", lex->after);
	str = ft_strjoin(lex->before, lex->after);
	++(*i);
	free_lexer(lex);
	return (str);
}

char	*double_quotes(char *str, int *i, char **env, t_lexer *lex)
{
	init_lexer(lex);
	lex->j = *i;
	while (str[++(*i)] != '\"')
	{
		if (str[*i] == '\\' && (str[*i + 1] == '\"' || str[*i + 1] == '$' || str[*i + 1] == '\\'))
			str = backslash(str, i, lex);
		if (str[*i] == '$')
		{
			str = dollar(str, i, env, lex);
			printf("str after dollar - %s\n", str);
		}
	}
	lex->before = ft_substr(str, 0, lex->j);
	printf("before double - %s\n", lex->before);
	lex->in = ft_substr(str, lex->j + 1, *i - lex->j - 1);
	printf("in double - %s\n", lex->in);
	lex->after = ft_strdup(str + *i + 1);
	printf("after double - %s\n", lex->after);
	str = ft_strjoin(lex->before, lex->in);
	str = ft_strjoin(str, lex->after);
	free_lexer(lex);
	return (str);
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
	init_lexer(lex);
	lex->j = *i;
	lex->e = -1;
	while (ifkey(str[++(*i)]))
		;
	if (*i != lex->j + 1)
		lex->key = ft_substr(str, lex->j + 1, *i - lex->j - 1);
	printf("key - %s\n", lex->key);
	if (lex->key)
	{
		
		while (env[++lex->e])
		{
			lex->p = ft_strnstr(env[lex->e], lex->key, ft_strlen(lex->key));
			if (lex->p && lex->p[ft_strlen(lex->key)] == '=')
			{
				lex->in = ft_strdup(env[lex->e] + ft_strlen(lex->key) + 1);
				*i = *i + ft_strlen(lex->in) - ft_strlen(lex->key) - 3;
			}
		}
		if (!lex->in)
			if_no_key_in_env(str, i, lex);
		lex->before = ft_substr(str, 0, lex->j - 1);
		printf("before dollar - %s\n", lex->before);
		printf("in dollar-%s\n", lex->in);
		printf("i - %d\n", *i);
		lex->after = ft_strdup(str + *i - 1); //*i - ???
		free(str);
		printf("after dollar-%s\n", lex->after);
		
		
		str = ft_strjoin(lex->before, lex->in);
		str = ft_strjoin(str, lex->after);
//		*i -= ft_strlen(lex->key) + 2;
//		printf("str dollar - %s\n", str);
	}
	free_lexer(lex);
	return (str);
}

char	*lexe(char *str, char **env)
{
	t_lexer	*lex;
	int		i;

	lex = (t_lexer *)malloc(sizeof(t_lexer));
	i = -1;
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
	printf("str lexer - %s\n", str);
	return (str);
}
