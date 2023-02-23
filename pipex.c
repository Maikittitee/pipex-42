/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:28:05 by maikittitee       #+#    #+#             */
/*   Updated: 2023/02/23 13:19:27 by ktunchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//EXE		./pipex infile cmd1 cmd2 outfile
//ARG		   0	  1		2	 3		4

//fd[0] is for read from the pipe
//fd[1] is for write to the  pipe

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

void	ft_find_cmd(t_pipex *pipex, char **av)
{
	int	cmd1_access_flag;
	int	cmd2_access_flag;
	int	i;
	char	*pure_cmd1;
	char	*pure_cmd2;
	

	cmd1_access_flag = 0;
	cmd2_access_flag = 0;
	i = 0;
	pipex->cmd1 = ft_split(av[2],' ');
	pipex->cmd2 = ft_split(av[3],' ');
	pure_cmd1 = ft_strdup((pipex->cmd1)[0]);
	pure_cmd2 = ft_strdup((pipex->cmd2)[0]);
	
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
		if (access((pipex->cmd1)[0], F_OK) == 0) 
		{	
			cmd1_access_flag = 1; 	
		}
		i++;
	}
	i = 0;
	while (!cmd2_access_flag && (pipex->path)[i])
	{
		(pipex->cmd2)[0] = ft_strjoin((pipex->path)[i],pure_cmd2);
		if (access((pipex->cmd2)[0], F_OK) == 0) 
		{	
			cmd2_access_flag = 1; 	
		}
		i++;
	}


}

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
		path[i] = ft_strjoin_free(path[i], "/"); //Leak
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
		while (path[i])
		{
			path[i] = ft_strjoin_free(path[i], cmd[0]);
			if (access(path[i], F_OK) == 0) 
			{
				cmd[0] = ft_strjoin_free(path[i], "");
				execve(cmd[0], cmd, env);	
				// yang mai dai free cmd
			}
			i++;
		}
		ft_displayerr(CMD_ERR, av[indexofcmd], 127);
}

void	ft_child1_process(char **path, char **av, char **env, int fd[2])
{
	int	infile_fd;

	infile_fd = open(av[1], O_RDONLY);
	if (infile_fd < 0)
	{
		ft_double_free(path);
		ft_displayerr(FILE_ERR, av[1], EXIT_FAILURE);
	}
	dup2(infile_fd, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(infile_fd);
	execute(path, av, env, 2);
}

void	ft_child2_process(char **path, char **av, char **env, int fd[2])
{
	int	outfile_fd;

	outfile_fd = open(av[4], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (outfile_fd < 0)
	{	
		ft_double_free(path);
		ft_displayerr(FILE_ERR, av[4], EXIT_FAILURE);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(outfile_fd);
	execute(path, av, env, 3);
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
	pipex.path = ft_split(env[get_path_index(env)] + 5, ':');
	pipex.path = join_bs(pipex.path);
	ft_find_cmd(&pipex, av);
	int i = 0;
	while ((pipex.cmd1)[i])
	{
		ft_printf("All in cmd1 is %s\n",(pipex.cmd1)[i]);
		i++;
	}
	i = 0;
	while ((pipex.cmd2)[i])
	{
		ft_printf("All in cmd2 is %s\n",(pipex.cmd2)[i]);
		i++;
	}
	// if (pipe(fd) != 0)
	//  		ft_displayerr(PIPE_ERR, NULL , errno);
	// pipex.pid1 = fork();
	// if (pipex.pid1 == -1)
	// {
	// 	ft_double_free(pipex.path);
	// 	ft_displayerr(FORK_ERR, NULL, errno);
	// }
	// if (pipex.pid1 == 0)
	// {
	// 	ft_child1_process(pipex.path, av, env, fd);
	// }
	// pipex.pid2 = fork();
	// if (pipex.pid2 == 0 && status == 0)
	// {
	// 	ft_child2_process(pipex.path, av, env, fd);
	// }
	// close(fd[0]);
	// close(fd[1]);	
	// waitpid(pipex.pid1,NULL,0);
	// waitpid(pipex.pid2, &status ,0);
	// ft_double_free(pipex.path);
	// return (WEXITSTATUS(status));
	
}
