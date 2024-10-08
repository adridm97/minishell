/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 14:23:09 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/07/29 20:52:35 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	c_src;
	size_t	c_dst;

	c_src = 0;
	c_dst = 0;
	while (src[c_src])
		c_src++;
	if (!dstsize)
		return (c_src);
	while (c_dst < (dstsize -1) && c_dst + 1 <= c_src)
	{
		dst[c_dst] = src[c_dst];
		c_dst++;
	}
	dst[c_dst] = '\0';
	return (c_src);
}

char	*ft_strncpy(char *dst, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dst[i] = '\0';
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*ft_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
/*
int	main(void)
{
	char str1[] = "";
	char str2[] = "aabbcc";
	char ft_str1[] = "";
	char ft_str2[] = "aabbcc";
	int num;

	printf("str1 %s str2 %s\n", str1, str2);
	//num = strlcpy(str1, "aaa", 3);
	printf("str1 %s str2 %s num =%i\n", str1, str2, num);
	printf("ft_str1 %s ft_str2 %s\n", ft_str1, ft_str2);
	num =  ft_strlcpy(ft_str1, "aaa", 3);
	printf("ft_str1 %s ft_str2 %s, num =%i\n", ft_str1, ft_str2, num);
	return (0);
}
*/
