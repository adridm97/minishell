/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:19:47 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/04 16:54:23 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

char	*ft_strndup(const char *s1, size_t n)
{
	size_t	c;
	char	*res;
	size_t	i;

	i = 0;
	c = 0;
	while (s1[c] && c < n)
		c++;
	res = (char *)malloc((c + 1) * sizeof(char));
	if (!res)
		return (NULL);
	while (i < c)
	{
		res[i] = s1[i];
		i++;
	}
	res[c] = '\0';
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
