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

void	init_args(t_arg *args)
{
	args->tokens = NULL;
	args->num = 0;
	args->redir = NULL;
	args->env = NULL;
	args->errnum = 0;
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
	int		num;
	int	**start_end_i;
	t_arg	*args;
	
	args = (t_arg *)malloc(sizeof(t_arg));
	init_args(args);

	env_read(args, arge);

	signals_ms();
	
	while(1)
	{
		num = 0;
		if (!(str = readline("\033[0;36m\033[1mminishell-0.32$ \033[0m")))
			exit (1);
		add_history(str);
		
		
		if (preparcer(str) == 1)
			printf("minishell: syntax error near unexpected token\n");
		else if (preparcer(str) == 2)
			printf("minishell: syntax error near unclosed quotes\n");
		else if (!preparcer(str))
		{
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
					cmd[s] = lexe(cmd[s], args->env);
				}
				cmd[s] = NULL;
				args->num = add_token(&args->tokens, cmd);
				free(start_end_i[0]);
				free(start_end_i[1]);
				free(start_end_i);
				sub_strs++;
			}
			add_redirs_to_cmd(args->redir, args->tokens);
				
			print_all_lists(args);
//			print_env(args->env);
			
			free(str);
	//		free(strs);
	//		status = execute(tokens);
			delete_all_redirs(args);
			delete_all_tokens(args);
		}
//		free(sub_strs);
	}
    return (status);
}
