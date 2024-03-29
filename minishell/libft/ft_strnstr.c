/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 07:59:30 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 12:40:41 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	c;
	size_t	need_len;

	i = 0;
	need_len = ft_strlen(needle);
	if (need_len == 0 || haystack == needle)
		return ((char *)haystack);
	while (haystack[i] != '\0' && i < len)
	{
		c = 0;
		while (haystack[i + c] && needle[c]
			&& haystack[i + c] == needle[c] && i + c < len)
			c++;
		if (c == need_len)
			return ((char *)&haystack[i]);
		i++;
	}
	return (0);
}
/*
int	main(void)
{

	printf("ft: %s\n", ft_strnstr("abc", "abcdef", 3));
	printf("or: %s\n", strnstr("abc", "abcdef", 3));
	printf("ft: %s\n", ft_strnstr("aaabcabcd", "aabc", 6));
	printf("or: %s\n", strnstr("aaabcabcd", "aabc", 6));
	printf("ft: %s\n", ft_strnstr("Hola soy Kevin", "bueno", 10));
	printf("or: %s\n", strnstr("Hola soy Kevin", "bueno", 10));
	printf("ft: %s\n", ft_strnstr("Hola soy Kevin", "a ", 5));
	printf("or: %s\n", strnstr("Hola soy Kevin", "a ", 5));
	printf("ft: %s\n", ft_strnstr("Hola soy Kevin", "Hola", 10));
	printf("or: %s\n", strnstr("Hola soy Kevin", "Hola", 10));
	printf("ft: %s\n", ft_strnstr("Hola soy Kevin", "Kevin", 10));
	printf("or: %s\n", strnstr("Hola soy Kevin", "Kevin", 10));
	return (0);
}
*/
