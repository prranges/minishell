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
	lst->str = NULL;
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

void	find_parts_of_str(char *str, int	**start_end_i)
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

int main(int argc, char **argv, char **arge)
{
	int		s;
	char	*str;
	char	**strs;
	int		status;
	int		number_of_parts;
	
	
	
	(void)argc;
	(void)argv;
	int	**start_end_i;
	t_token *tokens;
	t_env	*env;
	tokens = init_tokens();
	env = init_env();
	env_read(env, arge);
//	printf("env - %s\n", env->key);

	signals_ms();
	
	while(1)
	{
		if (!(str = readline("\033[0;36m\033[1mminishell-0.2$ \033[0m")))
			exit (1);
		add_history(str);
		number_of_parts = find_number_of_parts(str);
		if (number_of_parts < 0)
			error_print(-1);
		else
		{
			start_end_i = malloc(sizeof(int **) * 2);
			start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
			start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
			strs = malloc(sizeof(char **) * number_of_parts + 1);
			find_parts_of_str(str, start_end_i);
			s = -1;
			while (number_of_parts > ++s)
			{
				strs[s] = ft_substr(str, start_end_i[0][s], start_end_i[1][s] - start_end_i[0][s]);
				strs[s] = lexe(strs[s], arge);
			}
			strs[s] = NULL;
			parce(tokens, strs, arge);
			

			print_all_lists(tokens);
//			print_env(env);
			free(str);
			free(start_end_i[0]);
			free(start_end_i[1]);
			free(start_end_i);
			free(strs);
			status = execute(tokens);
			delete_all_lists(tokens);
		}
	}
    return (status);
}
