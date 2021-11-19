/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: caniseed <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 17:34:14 by caniseed          #+#    #+#             */
/*   Updated: 2021/11/19 19:13:54 by caniseed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int argc, char **argv)
{
	int fd[2];

	if (pipe(fd) == -1)
		return (1);

	int pid1 = fork();
	int pid2;
	if (pid1 > 0)
		pid2 = fork();

	if (pid1 < 0)
		return (2);
	if (pid1 == 0) //child process 1 (ping)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("ls", "ls", "-l", NULL);
	}

	if (pid2 == 0) //child process 1 (ping)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("cat", "cat", "-e", NULL);
	}
//	int i = 	while (i < q_of_pipes)
//	{
//		malloc fd;
//		pipe(fd[i]);
//		fork();
//		i++;
//	}0;
//



//	int pid2 = fork();
//
//	if (pid2 == 0)
//	{
//		dup2(fd[0], STDIN_FILENO);
//		close(fd[0]);
//		close(fd[1]);
//		execlp("grep", "grep", "rtt", NULL);
//	}


	waitpid(pid1, NULL, 0);
//	waitpid(pid2, NULL, 0);
	return (0);
}