/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 19:13:56 by ktunchar          #+#    #+#             */
/*   Updated: 2023/02/23 22:53:34 by ktunchar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *ft_strjoin_free(char *s1, char *s2)
{
    char *buffer;

    buffer = ft_strjoin(s1, s2);
    if (s1)
        free(s1);
    return (buffer);
}

char *ft_strjoin_free_back(char *s1, char *s2)
{
    char *buffer;

    buffer = ft_strjoin(s1, s2);
    if (s2)
        free(s2);
    return (buffer);
}

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