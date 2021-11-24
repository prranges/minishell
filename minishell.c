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

t_list	*init_list(void)
{
	t_list *lst;
	
	lst = (t_list *)malloc(sizeof(t_list));
	lst->type = 0;
	lst->str = NULL;
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
		printf("Error: Unclosed quotes.\n");
	if (id == 0)
		printf("Error: No str.\n");
	exit (1);
}

void	find_parts_of_str(char *str, int	**start_end_i)
{
	int	i;
	int s = 0;
	int e = 0;
	int flag_sq = 2;
	int flag_dq = 2;
	
	i = - 1;
	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t')
		start_end_i[0][s++] = 0;
	while (str[++i])
	{
		if (i != 0 && (str[i] == ' ' || str[i] == '\t') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			start_end_i[1][e++] = i;
		if ((str[i] == ' ' || str[i] == '\t') && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '\0') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
			start_end_i[0][s++] = i + 1;
		if (str[i] == '\'' && (!(flag_dq % 2)))
			flag_sq++;
		if (str[i] == '\"' && (!(flag_sq % 2)))
			flag_dq++;
	}
	if ((str[i] == '\0') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
		start_end_i[1][e++] = i;
}

//int	find_parts_of_str(char *str)
//{
//	int	i;
//	int s = 0;
//	int e = 0;
//	int count;
//	int flag_sq = 2;
//	int flag_dq = 2;
//	int	start_end_i[2][3];
//
//	char	*start;
//	char	*end;
//
//	i = - 1;
//	count = 0;
//	if (str[0] != '\0' && str[0] != ' ' && str[0] != '\t')
//	{
//		start = &str[0];
//		start_end_i[0][s] = 0;
//		s++;
//		printf("start - %s\n", start);
//		count++;
//	}
//	while (str[++i])
//	{
//		if ((str[i] == ' ' || str[i] == '\t') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
//		{
//			end = &str[i - 1];
//			start_end_i[1][e] = i - 1;
//			e++;
//			printf("end - %s\n", end);
//		}
//		if ((str[i] == ' ' || str[i] == '\t') && (str[i + 1] != ' ' && str[i + 1] != '\t' && str[i + 1] != '\0') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
//		{
//			start = &str[i + 1];
//			start_end_i[0][s] = i + 1;
//			s++;
//			printf("start - %s\n", start);
//			count++;
//		}
//		if (str[i] == '\'' && (!(flag_dq % 2)))
//			flag_sq++;
//		if (str[i] == '\"' && (!(flag_sq % 2)))
//			flag_dq++;
//	}
//	if ((str[i] == '\0') && (str[i - 1] != ' ' && str[i - 1] != '\t') && (!(flag_sq % 2)) && (!(flag_dq % 2)))
//	{
//		end = &str[i - 1];
//		start_end_i[1][e] = i - 1;
//		e++;
//		printf("end - %s\n", end);
//	}
//	if ((flag_sq % 2) || (flag_dq % 2))
//	{
//		printf("Error: unclosed quotes\n");
//		exit (1);
//	}
//	else
//		printf("NUM OF ARGs - %d\n", count);
//	printf("start - %d\n", start_end_i[0][0]);
//	printf("end - %d\n", start_end_i[1][0]);
//	printf("start - %d\n", start_end_i[0][1]);
//	printf("end - %d\n", start_end_i[1][1]);
//	printf("start - %d\n", start_end_i[0][2]);
//	printf("end - %d\n", start_end_i[1][2]);
//	return (count);
//}

int main(int argc, char **argv, char **env)
{
	int		s;
	char	*str;
	char	**strs;
	int		status;
	int		number_of_parts;
	
	(void)argc;
	(void)argv;
	int	**start_end_i;
	t_list *arguments;
	arguments = init_list();
	signals_ms();
	
	while(1)
	{
		if (!(str = readline("\033[0;36m\033[1mminishell-0.1$ \033[0m")))
			exit (1);
		add_history(str);
		number_of_parts = find_number_of_parts(str);
		if (number_of_parts < 0)
			error_print(-1);
		start_end_i = malloc(sizeof(int **) * 2);
		start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
		start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
		strs = malloc(sizeof(char **) * number_of_parts + 1);
		find_parts_of_str(str, start_end_i);
		s = -1;
		while (number_of_parts > ++s)
		{
			strs[s] = ft_substr(str, start_end_i[0][s], start_end_i[1][s] - start_end_i[0][s]);
			strs[s] = lexe(strs[s], env);
//			printf("substr - %s\n", strs[s]);
		}
		strs[s] = NULL;
		parce(arguments, strs, env);
		
//		str = lexe(str, env);
		
//		printf("STR: %s\n\n", str);

		print_all_lists(arguments);
		
		free(str);
		free(start_end_i[0]);
		free(start_end_i[1]);
		free(start_end_i);
		free(strs);
		status = execute(arguments);
		delete_all_lists(arguments);
	}
    return (status);
}
