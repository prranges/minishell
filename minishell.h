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

# define EXPORT		1
# define UNSET		2
# define ENV		3
# define PWD		4
# define ECHO		5
# define CD			6
# define EXIT		7
# define BUFFER_SIZE 1

typedef struct s_redir
{
	char			*file_name;
	char			*lim; //limiter
	int				cmd_list_num;
	int				out_in;//out = 0 in = 1
	int				dbl;
	struct s_redir	*next;
}	t_redir;

typedef struct s_token
{
	int				list_num;
	char			**cmd;
	t_redir			*in;
	t_redir			*out;
	int				builtin;
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
	char	**env_str;
	int		**fd;
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
char	*lexe(char *str, t_arg *args);
char	*dollar(char *str, int *i, t_arg *args, t_lexer *lex);
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
int		env_ms(t_env *env);
int		export_ms(t_arg *args);
void	edit_env(t_env **env, char *str);
int		if_key(char c);
void	remove_env(t_arg *args, t_env *remove_list, t_env *prev_p);
int		unset_ms(t_arg *args);
int		pwd_ms(t_arg *args);
int		echo_ms(t_arg *args);
int    	cd_ms(t_arg *args);
int		exit_ms(void);
int		start_builtin(t_arg *args);
int		make_builtin_dup(t_token *token);
void	builtin_dup_error_check(int fd);
int		precreate_or_preopen(t_arg *data);
void	heredoc(t_arg *data, char *limiter);//, char *file_name);

/*gnl*/
char	*get_line(char *saved_buf);
char	*clear_saved_buf(char *saved_buf);
int		get_next_line(int fd, char **line);
size_t	strlen_gnl(char *s);
char	*strjoin_gnl(char *saved_buf, char *buf);

#endif
