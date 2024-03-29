/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 08:25:22 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:14:30 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	char	*dest;
	size_t	c;

	dest = (char *)s;
	c = 0;
	while (c < n)
		dest[c++] = 0;
}
/*
int main(void)
{
	char str[10];

	ft_bzero(str,5);
	printf("|%i|",str[4]);
}
*/
