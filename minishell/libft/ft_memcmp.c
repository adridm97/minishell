/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 07:45:49 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:31 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			count;
	unsigned char	*cast_s1;
	unsigned char	*cast_s2;

	count = -1;
	cast_s1 = (unsigned char *)s1;
	cast_s2 = (unsigned char *)s2;
	while (++count < n)
	{
		if (cast_s1[count] != cast_s2[count])
			return (cast_s1[count] - cast_s2[count]);
	}
	return (0);
}
/*
int	main(void)
{
	printf("ft: %d\n", ft_memcmp("Hola soy Kevin", "Hola soy Kevin", 10));
	printf("or: %d\n", memcmp("Hola soy Kevin", "Hola soy Kevin", 10));
	printf("ft: %d\n", ft_memcmp("Hola so Kevin", "Hola soy Kevin", 10));
	printf("or: %d\n", memcmp("Hola so Kevin", "Hola soy Kevin", 10));
	printf("ft: %d\n", ft_memcmp("Hola soy Kevi", "Hola soy Kevin", 15));
	printf("or: %d\n", memcmp("Hola soy Kevi", "Hola soy Kevin", 15));
	printf("ft: %d\n", ft_memcmp("ola soy Kevin", "Hola soy Kevin", 10));
	printf("or: %d\n", memcmp("ola soy Kevin", "Hola soy Kevin", 10));
	printf("ft: %d\n", ft_memcmp("", "", 10));
	printf("or: %d\n", memcmp("", "", 10));
	return (0);
}
*/
