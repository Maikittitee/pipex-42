/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maikittitee <maikittitee@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:24:53 by maikittitee       #+#    #+#             */
/*   Updated: 2023/01/27 09:28:59 by maikittitee      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int	main()
{
	int id = fork();

	if (id != 0)
		usleep(1000000);
	if (id == 0)
		printf("Hello");
	else
		printf("world");


}