/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/26 20:46:52 by caniseed          #+#    #+#             */
/*   Updated: 2021/12/26 20:46:53 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**find_path(t_env *env)
{
	t_env	*p;
	char	**path;

	path = NULL;
	p = env;
	while (p != NULL)
	{
		if (ft_strncmp(p->key, "PATH", 4) == 0)
		{
			path = ft_split(p->value, ':');
			return (path);
		}
		p = p->next;
	}
	return (NULL);
}

char	*create_cmd_path(t_arg *data, char **all_paths, char *cmd)
{
	char	*temp;
	char	*path;
	char	**str;

	if (!all_paths)
		return (NULL);
	str = all_paths;
	while (*str)
	{
		temp = ft_strjoin(*str, "/");
		if (!temp)
			my_exit(data, "malloc", 12);
		path = ft_strjoin(temp, cmd);
		if (!path)
			my_exit(data, "malloc", 12);
		if (access(path, X_OK) != -1)
			return (path);
		free(temp);
		free(path);
		str++;
	}
	return (NULL);
}

char	*get_cmd_arg(t_arg *data, char **cmd)
{
	char	**all_paths;
	char	*path_executive;

	if (access(cmd[0], X_OK) != -1)
		return (cmd[0]);
	all_paths = find_path(data->env);
	if (!all_paths)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		my_exit(data, NULL, 127);
	}
	path_executive = create_cmd_path(data, all_paths, cmd[0]);
	return (path_executive);
}
