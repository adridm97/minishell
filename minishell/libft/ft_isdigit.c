/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 10:00:13 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:15:09 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if ((c >= '0' && c <= '9'))
		return (1);
	else
		return (0);
}

/*
int main (void)
{<
	printf("%d \n", ft_isprint('0'));
	printf("%d \n", ft_isprint('9'));
	printf("%d \n", ft_isprint('z'));
	printf("%d \n", ft_isprint('!'));
	printf("%d \n", ft_isprint('\n'));
	return (0);
}
*/
