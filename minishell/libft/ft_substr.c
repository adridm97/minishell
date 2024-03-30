/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 11:32:53 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 13:46:28 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	c_s;
	char			*res;

	c_s = ft_strlen(s);
	if (c_s < len)
		len = c_s - start;
	if (start >= c_s)
		return (ft_strdup(""));
	if (c_s + 1 == start + len)
		len--;
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res || !s)
		return (NULL);
	if ((size_t)start >= ft_strlen(s) || !ft_strlcpy(res, s + start, len + 1))
		res[0] = '\0';
	return (res);
}
/*
int	main(void)
{
	char	*str = "Hola soy Kevin";
	char	*sub;

	sub = ft_substr(str, 9, 14);
	printf("%s\n", sub);
	free(sub);
	return (0);
}
*/
