/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktunchar <ktunchar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/17 19:13:56 by ktunchar          #+#    #+#             */
/*   Updated: 2023/02/17 19:36:37 by ktunchar         ###   ########.fr       */
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