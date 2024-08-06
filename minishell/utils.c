/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:33:06 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/06 21:37:54 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_resources(t_data **data, char **input, char ***mat)
{
	if (*data)
	{
		free_data(data);
		*data = NULL;
	}
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	if (*mat)
		clean_env(mat, -1);
}

int	is_special_string(char *c, char *comp)
{
	char	*str;

	while (*c)
	{
		str = ft_strchr(comp, *c);
		if (str)
			return (1);
		c++;
	}
	return (0);
}
