/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 07:28:52 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:26 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	while (n-- > 0)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((void *)s);
		s++;
	}
	return (NULL);
}
/*
int	main(void)
{
	char	str[] = "Hola soy Kevin y me gusta el pollo frito";

	printf("%s\n", memchr(str, '3', 50));
	printf("%s\n", ft_memchr(str, '3', 50));
	return (0);
}
*/
