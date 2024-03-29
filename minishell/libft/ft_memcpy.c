/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 08:42:11 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/09/24 20:41:21 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <libc.h>

void	*ft_memcpy(void *dst, const void *src, size_t length)
{
	unsigned char	*p_dest;
	unsigned char	*p_src;

	if (!(src) && !(dst))
		return (0);
	p_dest = (unsigned char *)dst;
	p_src = (unsigned char *)src;
	while (length--)
	{
		*p_dest = *p_src;
		p_dest++;
		p_src++;
	}
	return (dst);
}
/*
int	main(void)
{
	char	*str1;
	char	*res1;
	char	*str2;
	char	*res2;

	str2 = calloc(3,4);
	res1 = memcpy(str1, "", 2);
	res2 = ft_memcpy(str2, "", 2);

	printf("%s\n", res1);
	printf("%s\n", res2);
	free(str2);
	return (0);
}
*/
