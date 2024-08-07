/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 07:10:04 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/07 11:21:23 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	*str1;

	i = 0;
	if (!s1 || !s2)
		return (1);
	str = (unsigned char *)s1;
	str1 = (unsigned char *)s2;
	while ((str[i] != '\0' || str1[i] != '\0'))
	{
		if (str[i] != str1[i])
			return (str[i] - str1[i]);
		i++;
	}
	return (0);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	count;

	if (!s1 || !s2)
		return (-1);
	count = 0;
	while (n && s1[count] && s2[count] && (s1[count] == s2[count]))
	{
		n--;
		count++;
	}
	if (!n)
		return (0);
	return ((unsigned char)s1[count] - (unsigned char)s2[count]);
}
/*
int	main(void)
{
	printf("ft: %d\n", ft_strncmp("Hola soy Kevin", "Hola soy Kevin", 10));
	printf("or: %d\n", strncmp("Hola soy Kevin", "Hola soy Kevin", 10));
	printf("ft: %d\n", ft_strncmp("Hola so Kevin", "Hola soy Kevin", 10));
	printf("or: %d\n", strncmp("Hola so Kevin", "Hola soy Kevin", 10));
	printf("ft: %d\n", ft_strncmp("Hola soy Kevi", "Hola soy Kevin", 15));
	printf("or: %d\n", strncmp("Hola soy Kevi", "Hola soy Kevin", 15));
	printf("ft: %d\n", ft_strncmp("ola soy Kevin", "Hola soy Kevin", 10));
	printf("or: %d\n", strncmp("ola soy Kevin", "Hola soy Kevin", 10));
	printf("ft: %d\n", ft_strncmp("", "", 10));
	printf("or: %d\n", strncmp("", "", 10));
	return (0);
}
*/
