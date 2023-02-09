/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maikittitee <maikittitee@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 15:28:05 by maikittitee       #+#    #+#             */
/*   Updated: 2023/02/09 23:38:44 by maikittitee      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//EXE		./pipex infile cmd1 cmd2 outfile
//ARG		   0	  1		2	 3		4

//fd[0] is for read from the pipe
//fd[1] is for write to the  pipe

int	get_path_index(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i],"PATH",5))
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
		path[i] = ft_strjoin(path[i],"/"); //Leak
		i++;
	}
	return (path);
}

void	execute(char **path, char **av, char **env,int	indexofcmd)
{
	char **cmd;
	int	i;

	i = 0;

	cmd = ft_split(av[indexofcmd],' ');	
	while (path[i])
	{
		path[i] = ft_strjoin(path[i],cmd[0]); //leak
		if (access(path[i],F_OK) == 0) //if can find cmd
		{
			cmd[0] = ft_strjoin(path[i],""); //leak
			execve(path[i],cmd,env);
		}
		i++;
	}
}

void	ft_child_process(char **path, char **av, char **env, int fd[2])
{
	int	infile_fd;

	infile_fd = open(av[1], O_RDONLY);

	dup2(infile_fd,STDIN_FILENO);
	dup2(fd[1],STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(infile_fd);
	execute(path,av,env,2);


}

void	ft_parent_process (char **path, char **av, char **env,int fd[2])
{
	int	outfile_fd;

	outfile_fd = open(av[4], O_RDWR | O_CREAT | O_TRUNC, 0777);

	dup2(fd[0],STDIN_FILENO);
	dup2(outfile_fd,STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	close(outfile_fd);

	execute(path,av,env,3);

}

int	main(int ac, char **av,char **env)
{
	char **path;
	int	id;
	int fd[2];

	if (ac != 5)
		return (1);

	path = ft_split(env[get_path_index(env)] + 5,':');	
	path = join_bs(path);
	pipe(fd);
	id = fork();
	if (id == 0) 
	{
		ft_child_process(path,av,env,fd);
	}
	else
	{
		wait(NULL);
		ft_parent_process(path,av,env,fd);
	}
	return (0);
}
