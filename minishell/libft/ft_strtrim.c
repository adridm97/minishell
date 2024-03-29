/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:21:08 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/09/23 11:21:08 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

size_t	ft_strlen(const char *s);

static int	isinset(char c, char const *set)
{
	int	count;

	count = -1;
	while (set[++count])
	{
		if (set[count] == c)
			return (1);
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		size;
	int		c;

	if (!set || !s1)
		return (NULL);
	size = ft_strlen(s1) - 1;
	if (size < 0)
		return (ft_substr("", 0, 1));
	c = 0;
	while (isinset(s1[c], set))
		c++;
	while (isinset(s1[size], set))
		size--;
	if (size < c)
		return (ft_substr("", 0, 1));
	return (ft_substr(s1, c, (size - c +1)));
}
/*
int	main(void)
{
	char *s1 = "  \t \t \n   \n\n\n\t";
 	char *s2 = "";
 	char *ret = ft_strtrim(s1, " \n\t");

 	if (!strcmp(ret, s2))
 		printf("OK");
	else
 		printf("MAL");

	return (0);
}
*/
