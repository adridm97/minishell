/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 07:01:36 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:16:45 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int			count;
	char		*res;

	count = -1;
	res = NULL;
	while (s[++count])
	{
		if (s[count] == (unsigned char)c)
			res = (char *)&s[count];
	}
	if (s[count] == (unsigned char)c)
		res = (char *)&s[count];
	return (res);
}
/*
int	main(void)
{
	char	str[] = "Hola soy Kevin y me gusta el pollo frito";

	printf("%p\n", strrchr("teste", '\0'));
	printf("%p\n", ft_strrchr("teste", '\0'));
	return (0);
}
*/
