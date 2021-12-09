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
	int		num;
	
	
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
		num = 0;
		if (!(str = readline("\033[0;36m\033[1mminishell-0.31$ \033[0m")))
			exit (1);
		add_history(str);
		
		
		if (preparcer(str) == 1)
			printf("minishell: syntax error near unexpected token\n");
		else if (preparcer(str) == 2)
			printf("minishell: syntax error near unclosed quotes\n");
		else if (!preparcer(str))
		{
			args->tokens = init_tokens();
			sub_strs = make_substrs_pipe_devided(str);
			while (*sub_strs)
			{
				num++;
				number_of_parts = find_number_of_parts(*sub_strs);
				start_end_i = malloc(sizeof(int **));
				start_end_i[0] = malloc(sizeof(int *) * number_of_parts);
				start_end_i[1] = malloc(sizeof(int *) * number_of_parts);
				cmd = malloc(sizeof(char **) * number_of_parts + 1);
				find_parts_of_str(*sub_strs, start_end_i, args, num);
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
			add_redirs_to_cmd(args->redir, args->tokens->next);
			if (args->redir)
			{
				printf("\n");
				printf("LAST REDIR - %s\n", last_redir(args->redir)->file_name); // print last redir
				printf("DOUBLE - %d\n", last_redir(args->redir)->dbl);
				printf("CMD LIST# - %d\n", last_redir(args->redir)->cmd_list_num);
				printf("OUT/IN - %d\n", last_redir(args->redir)->out_in);
				printf("\n");
			}


				free(str);

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
