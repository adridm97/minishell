/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 10:27:11 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 13:00:40 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*res;
	size_t	c;

	c = 0;
	res = malloc(count * size);
	if (!res)
		return (NULL);
	ft_bzero(res, count * size);
	return (res);
}
/*
int	main(void)
{
 	int size = 8539;

 	void * d1 = ft_calloc(size, sizeof(int));
 	void * d2 = calloc(size, sizeof(int));
 	if (memcmp(d1, d2, size * sizeof(int)))
 		printf("error");
 	free(d1);
 	free(d2);
		printf("ok");
	return (0);
}
*/
