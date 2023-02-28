/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 19:13:56 by ktunchar          #+#    #+#             */
/*   Updated: 2023/02/27 13:59:52 by ktunchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char **get_path(char **env)
{
    int i;
    char *path;
    char **ret_path;

    i = 0;
    while(env[i])
    {
        if (ft_strnstr(env[i], "PATH",5))
        {
            path = ft_strtrim(env[i],"PATH=");
            break;
        }
        i++;
    }
    ret_path = ft_split(path,':');
    i = 0;
    while (ret_path[i])
    {
        ret_path[i] = ft_strjoin_free(ret_path[i], "/");
        i++;
    }
    free(path);
    return (ret_path);
}


int	is_access_cmd(char **path, char **cmd, char *pure_cmd)
{
	char *temp;
	int	i;
	int	access_flag;

	i = 0;
	access_flag = 0;
	temp = cmd[0];
	while (!access_flag && path[i])
	{
		cmd[0] = ft_strjoin(path[i], pure_cmd);
		if (temp)
			free(temp);
		temp = cmd[0];
		if (access(cmd[0], F_OK) == 0) 
			return (1);
		i++;
	}
	return (0);

}

void	ft_find_cmd(t_pipex *pipex, char **av)
{
	//int	i;
	char	*pure_cmd[2];
	//char	*temp;
	
	//temp = NULL;
	pipex->access_flag1 = 0;
	pipex->access_flag2 = 0;
	//i = 0;
	pipex->cmd1 = ft_split(av[2],' ');
	pipex->cmd2 = ft_split(av[3],' ');
	pure_cmd[0] = ft_strdup((pipex->cmd1)[0]);
	pure_cmd[1] = ft_strdup((pipex->cmd2)[0]);
	//temp = (pipex->cmd1)[0];
	if (access((pipex->cmd1)[0], F_OK) == 0)
		pipex->access_flag1 = 1; 
	if (access((pipex->cmd2)[0], F_OK) == 0)
		pipex->access_flag2 = 1;
	// if Leak --> del this 4 line
	if (!pipex->access_flag1)
		pipex->access_flag1 = is_access_cmd(pipex->path,pipex->cmd1, pure_cmd[0]);
	if (!pipex->access_flag2)
		pipex->access_flag2 = is_access_cmd(pipex->path,pipex->cmd2, pure_cmd[1]);
		
	// and uncomment this
	// while (!pipex->access_flag1 && (pipex->path)[i])
	// {
	// 	(pipex->cmd1)[0] = ft_strjoin((pipex->path)[i],pure_cmd[0]);
	// 	if (temp)
	// 		free(temp);
	// 	temp = (pipex->cmd1)[0];
	// 	if (access((pipex->cmd1)[0], F_OK) == 0) 
	// 		pipex->access_flag1 = 1; 
	// 	i++;
	// }
	// i = 0;
	// temp = (pipex->cmd2)[0];
	// while (!pipex->access_flag2 && (pipex->path)[i])
	// {
	// 	(pipex->cmd2)[0] = ft_strjoin((pipex->path)[i], pure_cmd[1]);
	// 	if (temp)
	// 		free(temp);
	// 	temp = (pipex->cmd2)[0];
	// 	if (access((pipex->cmd2)[0], F_OK) == 0) 
	// 		pipex->access_flag2 = 1; 
	// 	i++;
	// }
	if (pure_cmd[0])
		free(pure_cmd[0]);
	if (pure_cmd[1])
		free(pure_cmd[1]);


}