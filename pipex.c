/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:28:05 by maikittitee       #+#    #+#             */
/*   Updated: 2023/02/17 13:40:49 by ktunchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//EXE		./pipex infile cmd1 cmd2 outfile
//ARG		   0	  1		2	 3		4

//fd[0] is for read from the pipe
//fd[1] is for write to the  pipe

void	ft_displayerr(int err, char *msg, int errnum)
{
	if (err == ARG_ERR)
		ft_putstr_fd("Invalid number of argument.\n", 2);
	else if (err == FORK_ERR)
		perror("Fork error :");
	else if (err == PIPE_ERR)
		perror("Pipe error :");
	else if (err == FILE_ERR)
		msg = ft_strjoin(msg, ": no such file or directory\n");
	else if (err == CMD_ERR)
		msg = ft_strjoin(msg, ": command not found\n");
	if (msg && (err == FILE_ERR || err == CMD_ERR))
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		free(msg);
	}
	exit (errnum);
}

int	get_path_index(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH", 5))
			return (i);
		i++;
	}
	return (0);
}

char 	**join_bs(char **path)
{
	int	i;

	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/"); //Leak
		i++;
	}
	return (path);
}

void	execute(char **path, char **av, char **env, int indexofcmd)
{
	char **cmd;
	int		i;

	i = 0;
	cmd = ft_split(av[indexofcmd], ' ');
	if ((access(cmd[0], F_OK)) == -1) // not found
	{
		while (path[i])
		{
			path[i] = ft_strjoin(path[i], cmd[0]); //leak
			if (access(path[i], F_OK) == 0) 
			{
				cmd[0] = ft_strjoin(path[i], ""); //leak
				execve(cmd[0], cmd, env);	
			}
			i++;
		}
		ft_displayerr(CMD_ERR, av[indexofcmd], 127);
	}
	execve(cmd[0], cmd, env);	
}

void	ft_child1_process(char **path, char **av, char **env, int fd[2])
{
	int	infile_fd;

	infile_fd = open(av[1], O_RDONLY);
	if (infile_fd < 0)
		ft_displayerr(FILE_ERR, av[1], errno);
	dup2(infile_fd, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(infile_fd);
	execute(path, av, env, 2);
}

void	ft_child2_process (char **path, char **av, char **env, int fd[2])
{
	int	outfile_fd;

	outfile_fd = open(av[4], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (outfile_fd < 0)
		ft_displayerr(FILE_ERR, av[4], errno);
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(outfile_fd);
	execute(path, av, env, 3);
}


int	main(int ac, char **av, char **env)
{
	char	**path;
	int		id;
	int 	id_in_child;
	int		fd[2];

	if (ac != 5)
	{
		ft_putstr_fd("This program take 4 argument", 2);
		exit(1);
	}
	path = ft_split(env[get_path_index(env)] + 5, ':');
	path = join_bs(path);
	id = fork();
	if (id == -1)
		ft_displayerr(FORK_ERR, NULL, errno);
	if (id == 0)
	{
		if (pipe(fd) != 0)
			ft_displayerr(PIPE_ERR, NULL , errno);
		id_in_child = fork();
		if (id_in_child == -1)
			ft_displayerr(FORK_ERR, NULL, errno);
		if (id_in_child == 0)
			ft_child1_process(path, av, env, fd);
		else
		{
			wait(NULL);
			ft_child2_process(path, av, env, fd);	
		}
	}
	wait(NULL);
//	ft_putstr_fd("Execute Done c1,c2",2);
	return (0);
}
