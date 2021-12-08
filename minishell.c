/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prranges <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/12 15:48:07 by prranges          #+#    #+#             */
/*   Updated: 2021/11/12 15:48:09 by prranges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_tokens(void)
{
	t_token *lst;
	
	lst = (t_token *)malloc(sizeof(t_token));
	lst->list_num = 0;
	lst->cmd = NULL;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}

int	find_number_of_parts(char *str)
{
	int	i;
	int count;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = - 1;
	count = 0;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t')
		count++;
	while (str[++i])
	{
		if ((str[i] == ' ' || str[i] == '\t')
		&& (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '>'
		&& str[i + 1] != '<' && str[i + 1] != '\0') && (!(flag_sq % 2))
		&& (!(flag_dq % 2)))
			count++;
		if ((str[i] == '<' || str[i] == '>') && (str[i + 1] == ' ' || str[i + 1] == '\t'))
			count--;
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if ((flag_sq % 2) || (flag_dq % 2))
		return (-1);
	return (count);
}

void	error_print(int id)
{
	if (id == -1)
		printf("minishell: syntax error with unclosed quotes\n");
	if (id == 0)
		printf("minishell: no str\n");
}

void	find_parts_of_str(char *str, int **start_end_i, t_arg *args)
{
	int	i;
	int s = 0;
	int e = 0;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = -1;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t' && str[i + 1] != '>' && str[i + 1] != '<')
		start_end_i[0][s++] = 0;
	while (str[++i])
	{
		if (!(flag_sq % 2) && !(flag_dq % 2))
		{
			if ((str[i] == ' ' || str[i] == '\t'))
			{
				if (i != 0 && (str[i - 1] != ' ' && str[i - 1] != '\t'))
					start_end_i[1][e++] = i;
				if ((str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '>' && str[i + 1] != '<' && str[i + 1] != '\0'))
					start_end_i[0][s++] = i + 1;
			}
			if ((str[i] == '>' || str[i] == '<'))
			{
				if (i != 0 && (str[i - 1] != ' ' && str[i - 1] != '\t'))
					start_end_i[1][e++] = i;
				i = redirect(str, i, args);
				start_end_i[0][s++] = i;
			}
		}
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if ((str[i] == '\0') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
		start_end_i[1][e++] = i;
}

int main(int argc, char **argv, char **arge)
{
	(void)argc;
	(void)argv;
	
	int		s;
	char	*str;
	char	**cmd;
	char	**sub_strs;
	int		status;
	int		number_of_parts;
	
	
	int	**start_end_i;
	t_arg	*args;
	t_env	*env;
	env = init_env();
	
	args = (t_arg *)malloc(sizeof(t_arg));
	args->env = env;
	args->redir = NULL;
	
	env_read(env, arge);

	signals_ms();
	
	while(1)
	{
		if (!(str = readline("\033[0;36m\033[1mminishell-0.3$ \033[0m")))
			exit (1);
		add_history(str);
		
		
		if (prepercer(str))
			printf("minishell: syntax error near unexpected token\n");
		else
		{
			args->tokens = init_tokens();
			sub_strs = make_substrs_pipe_devided(str);
			while (*sub_strs)
			{
				number_of_parts = find_number_of_parts(*sub_strs);
				start_end_i = malloc(sizeof(int **));
				start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
				start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
				cmd = malloc(sizeof(char **) * number_of_parts + 1);
				find_parts_of_str(*sub_strs, start_end_i, args);
				s = -1;
				while (number_of_parts > ++s)
				{
					cmd[s] = ft_substr(sub_strs[0], start_end_i[0][s], start_end_i[1][s] - start_end_i[0][s]);
					cmd[s] = lexe(cmd[s], env);
				}
				cmd[s] = NULL;
				args->tokens = add_token(args->tokens, cmd);
	//			print_double_array(strs);
				free(start_end_i[0]);
				free(start_end_i[1]);
				free(start_end_i);
	//			free(cmd);
				sub_strs++;
			}
			if (args->redir)
				printf("last redir - %s\n", last_redir(args->redir)->file_name); // print last redir


				free(str);
	//			free(start_end_i[0]);
	//			free(start_end_i[1]);
	//			free(start_end_i);
	//			free(strs);
	//			status = execute(tokens);
				delete_all_redirs(args);
				delete_all_lists(args->tokens);
		}
//		free(sub_strs);
//		free_double_massive(sub_strs);
	}
    return (status);
}
