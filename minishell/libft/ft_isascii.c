/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 10:02:13 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:00 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if ((c >= 0 && c <= 127))
		return (1);
	else
		return (0);
}

/*
int main (void)
{
	printf("%d \n", ft_isascii('0'));
	printf("%d \n", ft_isascii('9'));
	printf("%d \n", ft_isascii('z'));
	printf("%d \n", ft_isascii('!'));
	printf("%d \n", ft_isascii('\n'));
	return (0);
}
*/
