/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 15:48:20 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 15:48:22 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>

# define ERROR		1
# define SUCCESS	0

//# define
//# define
//# define
//# define
//# define
//# define
//# define

typedef struct s_token
{
    int				type;
    char			*str;
    struct s_token	*next;
    struct s_token	*prev;
} t_token;

typedef struct s_env
{
    char			*key;
	char			separator;
	char			*value;
    struct s_env	*next;
} t_env;

typedef struct s_lexer
{
	int 	j;
	int		e;
	char	*p;
	char	*key;
	char	*before;
	char	*in;
	char	*after;
} t_lexer;


t_token	*add_token(t_token *lst, char *str);
t_env 	*add_env(t_env *env, char *str);
void	signals_ms(void);
char	*lexe(char *str, char **env);
char	*dollar(char *str, int *i, char **env, t_lexer *lex);
void  	parce(t_token *lst, char **strs, char **env);
void    print_all_lists(t_token *lst);
void    print_env(t_env *lst);
void    delete_all_lists(t_token *lst);
int 	execute(t_token *lst);

#endif
