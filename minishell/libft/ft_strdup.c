/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:19:47 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:16:11 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libc.h>
#include <stdio.h>
#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		c;
	char	*res;

	c = 0;
	while (s1[c])
		c++;
	res = (char *)malloc((++c) * sizeof(char));
	if (!res)
		return (NULL);
	while (c--)
		res[c] = s1[c];
	return (res);
}
/*
int	main(void)
{
	char *str = "Hola soy Kevin";
	char *str2 = ft_strdup(str);

	printf("%s, %s\n", str, str2);
	free(str2);
}
*/
