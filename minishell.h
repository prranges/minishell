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
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>

# define ERROR		1
# define SUCCESS	0

//# define
//# define
//# define
//# define
//# define
//# define
//# define

typedef struct s_redir
{
	char			*file_name;
	char			*lim;
	int				cmd_list_num;
	int				out_in;
	int				dbl;
	struct s_redir	*next;
}	t_redir;

typedef struct s_token
{
	int				list_num;
	char			**cmd;
	t_redir			*in;
	t_redir			*out;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			separator;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_lexer
{
	int		j;
	int		e;
	char	*p;
	char	*key;
	char	*before;
	char	*in;
	char	*after;
}	t_lexer;

typedef struct s_arg
{
	t_token	*tokens;
	int		num;
	t_redir	*redir;
	t_env	*env;
//	char	**env;
//	int		**fd;
	int		errnum;
}	t_arg;

int		preparcer(char *str);
t_token	*init_tokens(void);
void	add_env(t_env **env, char *str);
t_env	*init_env(void);
void	env_read(t_arg *arg, char **arge);
int		add_token(t_token **lst, char **str);
void	signals_ms(void);
void	init_lexer(t_lexer *lex);
void	free_lexer(t_lexer *lex);
char	*lexe(char *str, t_env *env);
char	*dollar(char *str, int *i, t_env *env, t_lexer *lex);
void	parce(t_token *lst, char **strs, t_env *env);
void	print_all_lists(t_arg *args);
void	print_env(t_env *lst);
void	delete_all_tokens(t_arg *args);
int		execute(t_token *lst);
int		free_double_array(char **str);
void	print_double_array(char **sub_strs);
char	**make_substrs_pipe_devided(char *str);
int		redirect(char *str, int i, t_arg *args, int num);
t_redir	*init_redir(void);
void	delete_all_redirs(t_arg *args);
t_redir	*last_redir(t_redir *redir);
void	add_redirs_to_cmd(t_redir *redir, t_token *tokens);
int		find_number_of_parts(char *str);
void	find_parts_of_str(char *str, int **start_end_i, t_arg *args, int num);
int		pipex(int argc, char **argv, char **env, t_arg *data);

#endif
