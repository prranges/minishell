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
	lst->type = 0;
	lst->cmd = NULL;
	lst->next = NULL;
	lst->prev = NULL;
	return (lst);
}

t_env	*init_env(void)
{
	t_env *lst;
	
	lst = (t_env *)malloc(sizeof(t_env));
	lst->key = NULL;
	lst->value = NULL;
	lst->separator = 0;
	lst->next = NULL;
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
		if ((str[i] == ' ' || str[i] == '\t') && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '\0') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			count++;
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

void	find_parts_of_str(char *str, int **start_end_i)
{
	int	i;
	int s = 0;
	int e = 0;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = -1;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t')
		start_end_i[0][s++] = 0;
	while (str[++i])
	{
		if (i != 0 && (str[i] == ' ' || str[i] == '\t') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			start_end_i[1][e++] = i;
		if ((str[i] == ' ' || str[i] == '\t') && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '\0') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			start_end_i[0][s++] = i + 1;
		
		
		
		
		
		if (str[i] == '>')
		{
			if (str[i + 1] == '>')
			{
				printf("redirect >>\n");
				i++;
			}
			else
				printf("redirect >\n");
		}
		if (str[i] == '<')
		{
			if (str[i + 1] == '<')
			{
				printf("redirect <<\n");
				i++;
			}
			else
				printf("redirect <\n");
		}
		
		
		
		
		
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if ((str[i] == '\0') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
		start_end_i[1][e++] = i;
}

void	env_read(t_env *env, char **arge)
{
	int	i;
	
	i = -1;
//	printf("arge - %s\n", arge[0]);
	while (arge[++i])
		env = add_env(env, arge[i]);
}

int	number_of_pipes(char *str)
{
	int count;
	int flag_sq = 2;
	int flag_dq = 2;
	
	count = 0;
	while (*str)
	{
		if ((*str == '|') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			count++;
		if (*str == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (*str == '\"' && (!(flag_sq % 2)))
			flag_dq++;
		str++;
	}
	return (count);
}

int	find_pipe_index(char *str)
{
	int i;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = -1;
	while (str[++i])
	{
		if ((str[i] == '|') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			return (i);
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	return (i);
}

char	**make_substrs_pipe_devided(char *str)
{
	int 	i;
	int		start;
	int 	len;
	char	*p;
	char 	**sub_strs;
	
	i = 0;
	p = str;
//	printf("p - %s\n", p);
	sub_strs = malloc(sizeof(char **) + (sizeof(char *) * number_of_pipes(p) + 2));

	start = 0;
	while (p[start])
	{
		len = find_pipe_index(p + start);

		sub_strs[i] = ft_substr(p, start, len);
		start += len + 1;
		i++;
	}
	sub_strs[i] = NULL;
	return (sub_strs);
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
	t_token *tokens;
	t_env	*env;
	env = init_env();
	env_read(env, arge);

	signals_ms();
	
	while(1)
	{
		if (!(str = readline("\033[0;36m\033[1mminishell-0.2$ \033[0m")))
			exit (1);
		add_history(str);
		tokens = init_tokens();
		sub_strs = make_substrs_pipe_devided(str);
				
		
		while (*sub_strs)
		{
			number_of_parts = find_number_of_parts(*sub_strs);
			start_end_i = malloc(sizeof(int **));
			start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
			start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
			cmd = malloc(sizeof(char **) * number_of_parts + 1);
			find_parts_of_str(*sub_strs, start_end_i);
			s = -1;
			while (number_of_parts > ++s)
			{
				cmd[s] = ft_substr(sub_strs[0], start_end_i[0][s], start_end_i[1][s] - start_end_i[0][s]);
				cmd[s] = lexe(cmd[s], env);
			}
			cmd[s] = NULL;
			tokens = add_token(tokens, cmd);
//			print_double_massive(strs);
			free(start_end_i[0]);
			free(start_end_i[1]);
			free(start_end_i);
//			free(cmd);
			sub_strs++;
//
		}
//		printf("list 1 srt 0 - %s\n", tokens->);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
//		number_of_parts = find_number_of_parts(sub_strs[0]);
//		if (number_of_parts < 0)
//			error_print(-1);
////		printf("number of parts - %d\n", number_of_parts);
//		else
//		{
//			start_end_i = malloc(sizeof(int **));
//			start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
//			start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
//			strs = malloc(sizeof(char **) * number_of_parts + 1);
//
//			find_parts_of_str(sub_strs[0], start_end_i);
//			s = -1;
//			while (number_of_parts > ++s)
//			{
//				strs[s] = ft_substr(sub_strs[0], start_end_i[0][s], start_end_i[1][s] - start_end_i[0][s]);
//				strs[s] = lexe(strs[s], env);
//			}
//			strs[s] = NULL;
//
//			print_double_massive(strs);
//
////			parce(tokens, strs, env);
//
//
//
//			print_all_lists(tokens);
////			print_env(env);
			free(str);
//			free(start_end_i[0]);
//			free(start_end_i[1]);
//			free(start_end_i);
//			free(strs);
//			status = execute(tokens);
			delete_all_lists(tokens);
//		}
//		free(sub_strs);
//		free_double_massive(sub_strs);
	}
    return (status);
}
