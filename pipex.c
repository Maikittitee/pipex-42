/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:28:05 by maikittitee       #+#    #+#             */
/*   Updated: 2023/02/24 00:41:55 by ktunchar         ###   ########.fr       */
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

void	ft_double_free(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

void	ft_free_pipex(t_pipex *pipex)
{
		ft_double_free(pipex->cmd1);
		ft_double_free(pipex->cmd2);
		ft_double_free(pipex->path);
}

void	ft_find_cmd(t_pipex *pipex, char **av)
{
	int	cmd1_access_flag;
	int	cmd2_access_flag;
	int	i;
	char	*pure_cmd1;
	char	*pure_cmd2;
	char	*temp;
	
	temp = NULL;
	cmd1_access_flag = 0;
	cmd2_access_flag = 0;
	i = 0;
	pipex->cmd1 = ft_split(av[2],' ');
	pipex->cmd2 = ft_split(av[3],' ');
	pure_cmd1 = ft_strdup((pipex->cmd1)[0]);
	pure_cmd2 = ft_strdup((pipex->cmd2)[0]);
	temp = (pipex->cmd1)[0];
	if (access((pipex->cmd1)[0], F_OK) == 0)
	{
		cmd1_access_flag = 1; 
	}
	if (access((pipex->cmd2)[0], F_OK) == 0)
	{
		cmd2_access_flag = 1; 
	}
	while (!cmd1_access_flag && (pipex->path)[i])
	{
		(pipex->cmd1)[0] = ft_strjoin((pipex->path)[i],pure_cmd1);
		if (temp)
			free(temp);
		temp = (pipex->cmd1)[0];
		if (access((pipex->cmd1)[0], F_OK) == 0) 
		{	
			cmd1_access_flag = 1; 	
		}
		i++;
	}
	i = 0;
	temp = (pipex->cmd2)[0];
	while (!cmd2_access_flag && (pipex->path)[i])
	{
		(pipex->cmd2)[0] = ft_strjoin((pipex->path)[i], pure_cmd2);
		if (temp)
			free(temp);
		temp = (pipex->cmd2)[0];
		if (access((pipex->cmd2)[0], F_OK) == 0) 
		{	
			cmd2_access_flag = 1; 	
		}
		i++;
	}
	if (!cmd1_access_flag)
		ft_displayerr(CMD_ERR, av[2], 127);
	if (!cmd2_access_flag)
		ft_displayerr(CMD_ERR, av[3], 127);
	if (pure_cmd1)
		free(pure_cmd1);
	if (pure_cmd2)
		free(pure_cmd2);


}

void	ft_child1_process(t_pipex pipex, char **av, char **env, int fd[2])
{
	int	infile_fd;

	infile_fd = open(av[1], O_RDONLY);
	if (infile_fd < 0)
	{
		ft_double_free(pipex.path);
		ft_displayerr(FILE_ERR, av[1], EXIT_FAILURE);
	}
	dup2(infile_fd, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(infile_fd);
	execve((pipex.cmd1)[0], pipex.cmd1, env);
}

void	ft_child2_process(t_pipex pipex, char **av, char **env, int fd[2])
{
	int	outfile_fd;

	outfile_fd = open(av[4], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (outfile_fd < 0)
	{	
		ft_double_free(pipex.path);
		ft_displayerr(FILE_ERR, av[4], EXIT_FAILURE);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(outfile_fd);
	execve((pipex.cmd2)[0], pipex.cmd2, env);
}


int	main(int ac, char **av, char **env)
{
	int		fd[2];
	t_pipex pipex;
	int	status;
	

	if (ac != 5)
	{
		ft_putstr_fd("This program take 4 argument", 2);
		exit(1);
	}
	pipex.path = get_path(env);
	ft_find_cmd(&pipex, av);
	if (pipe(fd) != 0)
	 		ft_displayerr(PIPE_ERR, NULL , errno);
	pipex.pid1 = fork();
	if (pipex.pid1 == -1)
	{
		ft_double_free(pipex.path);
		ft_displayerr(FORK_ERR, NULL, errno);
	}
	if (pipex.pid1 == 0)
	{
		ft_child1_process(pipex, av, env, fd);
	}
	pipex.pid2 = fork();
	if (pipex.pid2 == 0)
	{
		ft_child2_process(pipex, av, env, fd);
	}
	close(fd[0]);
	close(fd[1]);	
	waitpid(pipex.pid1,NULL,0);
	waitpid(pipex.pid2, &status ,0);
	ft_double_free(pipex.path);
	ft_double_free(pipex.cmd1);
	ft_double_free(pipex.cmd2);
	ft_printf("Path after free is %s\n",pipex.path);
	ft_printf("cmd1 after free is %s\n",pipex.cmd1);
	ft_printf("cmd2 after free is %s\n",pipex.cmd2);
	return (WEXITSTATUS(status));
	
}
