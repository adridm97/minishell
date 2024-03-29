/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 13:45:33 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:22 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

static char	*printstr(int num, char *res, int size, int neg)
{
	int	n;

	n = num;
	while ((--size) >= 0 && n)
	{
		if (n > 0)
			res[size] = (n % 10) + 48;
		else
			res[size] = (n % 10) * -1 + 48;
		n /= 10;
	}
	if (neg && num)
		res[size] = '-';
	if (!num)
		res[size] = (n % 10) + 48;
	return (res);
}

char	*ft_itoa(int n)
{
	int		tmp;
	int		neg;
	int		c;
	char	*res;

	neg = 0;
	c = 0;
	if (n <= 0)
	{
		neg = 1;
		c++;
	}
	tmp = n;
	while (tmp != 0)
	{
		tmp /= 10;
		c++;
	}
	res = (char *)malloc((c + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[c] = '\0';
	return (printstr(n, res, c, neg));
}
/*
int	main(void)
{
	char	*res;

	res = ft_itoa(-2147483648);
	printf("%s|\n", res);
	res = ft_itoa(-83647);
	printf("%s|\n", res);
	res = ft_itoa(0);
	printf("%s|\n", res);
	res = ft_itoa(5);
	printf("%s|\n", res);
	res = ft_itoa(2147483647);
	printf("%s|\n", res);
	free(res);
	return (0);
}
*/
