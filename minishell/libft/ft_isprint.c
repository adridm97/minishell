/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 10:02:29 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:14 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if ((c >= 32 && c <= 126))
		return (1);
	else
		return (0);
}

/*
int main (void)
{
	printf("%d \n", ft_isprint('0'));
	printf("%d \n", ft_isprint('9'));
	printf("%d \n", ft_isprint('z'));
	printf("%d \n", ft_isprint('!'));
	printf("%d \n", ft_isprint('\n'));
	return (0);
}
*/
