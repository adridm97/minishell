/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:12:39 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 13:00:04 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		c1;
	int		c2;
	char	*str;

	c1 = -1;
	c2 = -1;
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s1[++c1])
		str[++c2] = s1[c1];
	c1 = -1;
	while (s2[++c1])
		str[++c2] = s2[c1];
	str[++c2] = '\0';
	return (str);
}
/*
int	main(void)
{
	char *str = ft_strjoin("Hola soy ","Kevin");
	printf("%s\n", str);
	free(str);
	return (1);
}
*/