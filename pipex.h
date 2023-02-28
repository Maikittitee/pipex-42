/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@42bangkok.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 19:22:43 by maikittitee       #+#    #+#             */
/*   Updated: 2023/02/27 11:20:18 by ktunchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/errno.h>
# include "include/libft.h"
# include "include/ft_printf.h"


typedef struct s_pipex{
    int pid1;
    int pid2;
	int access_flag1;
	int	access_flag2;
    char **cmd1;
    char **cmd2;
    char **path;
} t_pipex;

char *ft_strjoin_free(char *s1, char *s2);
char **get_path(char **env);
void	ft_displayerr(int err, char *msg, int errnum, t_pipex *pipex);
void	ft_double_free(char **s);
void	ft_free_pipex(t_pipex *pipex);
int	is_access_cmd(char **path, char **cmd, char *pure_cmd);
void	ft_find_cmd(t_pipex *pipex, char **av);
void	ft_child1_process(t_pipex *pipex, char **av, char **env, int fd[2]);
void	ft_child2_process(t_pipex *pipex, char **av, char **env, int fd[2]);

# define ARG_ERR 1
# define FORK_ERR 2
# define PIPE_ERR 3
# define FILE_ERR 4
# define CMD_ERR 5


#endif

