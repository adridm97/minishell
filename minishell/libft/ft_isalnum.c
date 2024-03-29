/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 10:01:11 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:14:47 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

/*
int main (void)
{
	printf("%d \n", ft_isalnum('a'));
	printf("%d \n", ft_isalnum('G'));
	printf("%d \n", ft_isalnum('z'));
	printf("%d \n", ft_isalnum('4'));
	printf("%d \n", ft_isalnum('!'));
	printf("%d \n", ft_isalnum('\n'));
	return (0);
}
*/
