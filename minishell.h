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

typedef struct s_list
{
    int				type;
    char			*str;
    struct s_list	*next;
    struct s_list	*prev;
} t_list;

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

t_list	*add_list(t_list *lst, char *str);
void	signals_ms(void);
char	*lexe(char *str, char **env);
char	*dollar(char *str, int *i, char **env, t_lexer *lex);
void  	parce(t_list *lst, char **strs, char **env);
void    print_all_lists(t_list *lst);
void    delete_all_lists(t_list *lst);
int 	execute(t_list *lst);

#endif
