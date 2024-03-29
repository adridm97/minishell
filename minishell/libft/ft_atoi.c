/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 09:07:53 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:13:42 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

static int	isspace(int c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

static int	isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

static void	first(int *c, int *neg, const char *str)
{
	while ((isspace(str[*c]) && str[*c]))
		*c += 1;
	if (str[*c] == '-')
	{
		*c += 1;
		*neg = 1;
	}
	else if (str[*c] == '+')
		*c += 1;
}

int	ft_atoi(const char *str)
{
	int	c;
	int	neg;
	int	flag;
	int	res;

	c = 0;
	neg = 0;
	flag = 1;
	res = 0;
	first(&c, &neg, str);
	while (str[c] && flag)
	{
		if (isdigit(str[c]))
			res = ((res * 10) + (str[c++] - '0'));
		else
			flag = 0;
	}
	if (neg)
		res *= -1;
	return (res);
}
/*
int	main(void)
{
	printf("%d\n", atoi("  +123 55"));
	printf("%d\n", ft_atoi("  +123 55"));
	printf("%d\n", atoi("  \n2147483647"));
	printf("%d\n", ft_atoi("  \n2147483647"));
	printf("%d\n", atoi("  -2147483648"));
	printf("%d\n", ft_atoi("  -2147483648"));
	printf("%d\n", atoi(""));
	printf("%d\n", ft_atoi(""));
	printf("%d\n", atoi("  -12355"));
	printf("%d\n", ft_atoi("  -12355"));
	printf("%d\n", atoi("  - 123 55"));
	printf("%d\n", ft_atoi("  - 123 55"));
	return (0);
}
*/
