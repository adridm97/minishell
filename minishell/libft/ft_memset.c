/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 10:03:36 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:41 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	*ft_memset(void *b, int c, size_t n)
{
	char	*c_str;

	c_str = (char *)b;
	while (n)
		c_str[--n] = c;
	return (b);
}
/*
int main(void)
{
	#include <string.h>
	#include <stdio.h>
	char str[50] = "GeeksForGeeks is for programming geeks.";
	printf("\nBefore memset(): %s\n", str);
	memset(str + 13, '.', 8 * sizeof(char));
	printf("After memset():  %s", str);
	char ft_str[50] = "GeeksForGeeks is for programming geeks.";
	printf("\nBefore memset(): %s\n", ft_str);
	ft_memset(ft_str + 13, '.', 8 * sizeof(char));

	printf("After memset():  %s", ft_str);
	return (0);
}*/
