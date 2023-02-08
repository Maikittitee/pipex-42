//#include "pipex.h"

# include <stdio.h>
# include <unistd.h>
# include "include/libft.h"

//EXE		./pipex infile cmd1 cmd2 outfile
//ARG		   0	  1		2	 3		4	
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

void	execute(char **path, char **av, char **env)
{
	char **cmd;
	//char *used;
	int	i;

	i = 0;

	cmd = ft_split(av[2],' ');

	path = join_bs(path);
	// printf("--%s--\n",path[0]);
	// printf("++%s++\n",cmd[0]);
	
	while (path[i])
	{
		path[i] = ft_strjoin(path[i],cmd[0]); //leak
		if (access(path[i],F_OK) == 0) //if can find cmd
		{
			cmd[0] = ft_strjoin(path[i],""); //leak
			execve(path[i],cmd,env);
			break;
		}
		i++;
	}
}

int	main(int ac, char **av,char **env)
{
	(void) ac;
	char **path;


	//printf("inmain-==%s--\n",env[get_path_index(env)]);
	path = ft_split(env[get_path_index(env)] + 5,':');	
	//printf("inmain--%s--\n",path[0]);
	//printf("++%s++\n",av[2]);

	execute(path,av,env);

	return (0);
	
}
