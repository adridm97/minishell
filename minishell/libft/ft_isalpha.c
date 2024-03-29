/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 09:59:52 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/10/01 20:14:54 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/*
int main (void)
{
	printf("%d \n", ft_isalpha('a'));
	printf("%d \n", ft_isalpha('G'));
	printf("%d \n", ft_isalpha('z'));
	printf("%d \n", ft_isalpha('!'));
	printf("%d \n", ft_isalpha('4'));
	printf("%d \n", ft_isalpha('\n'));
	return (0);
}
*/
