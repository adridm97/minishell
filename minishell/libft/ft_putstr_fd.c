/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 14:06:04 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:16:01 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	c;

	c = -1;
	while (s[++c])
		write(fd, &s[c], 1);
}
/*
int main(void)
{
	int file = open("file.txt", O_WRONLY);
	if (!file)
		return (0);
	ft_putstr_fd("Hola soy kevin y estoy escribiendo en un 
	fichero\nAhora en una nueva línea", file);
	return (0);
}
*/
