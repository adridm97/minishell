/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:36:46 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:16:21 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	c_dst;
	size_t	c_src;
	size_t	num;

	c_dst = 0;
	c_src = 0;
	while (dst[c_dst])
		c_dst++;
	if (c_dst > dstsize)
	{
		c_dst = 0;
		while (src[c_dst])
			c_dst++;
		return (dstsize + c_dst);
	}
	num = c_dst;
	while (c_dst +1 < (dstsize) && src[c_src])
		dst[c_dst++] = src[c_src++];
	dst[c_dst] = '\0';
	while (src[c_src++])
		c_dst++;
	return (c_dst);
}
/*
int	main(void)
{
	char	str1[50] = "";
	char	str2[15] = "123";
	char	ft_str1[50] = "";
	char	ft_str2[15] = "123";
	size_t	m = 0;
	size_t	i;

	printf("str1: %s, str2: %s \n", str1, str2);
	i = strlcat(str1, str2, m);
	printf("str1: %s, str2: %s, %zu\n", str1, str2, i);
	printf("ft_str1: %s, ft_str2: %s \n", ft_str1, ft_str2);
	i = ft_strlcat(ft_str1, ft_str2, m);
	printf("ft_str1: %s, ft_str2: %s, %zu\n", ft_str1, ft_str2, i);
	return (0);
}
*/
