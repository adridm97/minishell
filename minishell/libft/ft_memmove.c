/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 09:04:03 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:38 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*p_dst;
	unsigned char	*p_src;

	if (!(src) && !(dst))
		return (0);
	p_dst = (unsigned char *)dst;
	p_src = (unsigned char *)src;
	if ((src) < (dst))
	{
		p_dst = p_dst + len -1;
		p_src = p_src + len -1;
		while (len--)
			*p_dst-- = *p_src--;
	}
	else
	{
		while (len--)
			*p_dst++ = *p_src++;
	}
	return (dst);
}
/*
int	main(void)
{
	char	str1[] = "aabbcc";
	char	str2[] = "ddeeff";
	char	ftstr1[] = "aabbcc";
	char	ftstr2[] = "ddeeff";

	printf("str1: %s, str2: %s\n", str1, str2);
	//memmove(str2, str1, 4);
	printf("str1: %s, str2: %s\n", str1, memmove(str2, str1, 6));
	printf("str1: %s, str2: %s\n", ftstr1, ftstr2);
	//ft_memmove(ftstr2, ftstr1, 4);
	printf("str1: %s, str2: %s\n", ftstr1, ft_memmove(ftstr2, ftstr1, 6));
}
*/
