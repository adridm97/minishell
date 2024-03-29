/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 13:52:28 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:16:36 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"

/*
char	bx(unsigned int i, char c)
{
	return (c + 1);
}*/

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		c;
	char	*res;

	c = 0;
	while (s[c])
		c++;
	res = (char *)malloc((++c) * sizeof(char));
	if (!res)
		return (NULL);
	res[--c] = '\0';
	while (--c >= 0)
		res[c] = f(c, s[c]);
	return (res);
}
/*
int	main(void)
{
	char	*str = ft_strmapi("Hola", &bx);

	printf("%s|", str);
	return (0);
}*/
