/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 17:08:29 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:16:07 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int		count;

	count = -1;
	while (s[++count])
	{
		if (s[count] == (unsigned char)c)
			return ((char *)&s[count]);
	}
	if (s[count] == (unsigned char)c)
		return ((char *)&s[count]);
	else
		return (NULL);
}
/*
int	main(void)
{
	char	str[] = "Hola soy Kevin y me gusta el pollo frito";

	printf("%s\n", strchr(str, '\0')-1);
	printf("%s\n", ft_strchr(str, '\0')-1);
	return (0);
}
*/
