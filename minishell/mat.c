/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:25:11 by adrian            #+#    #+#             */
/*   Updated: 2024/08/11 22:40:22 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_matsize(char **mat)
{
	int	size;

	size = 0;
	while (mat[size])
		size++;
	return (size);
}

//TODO manage g_status_code
char	**ft_matadd(char ***mat, char *str, t_data **data)
{
	int		size;
	char	**new_mat;
	int		i;
	char	**c_mat;

	i = -1;
	size = ft_matsize(*mat);
	new_mat = (char **)malloc(sizeof(char **) * (size + 2));
	if (!new_mat)
		return (free_args(mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	c_mat = *mat;
	while (c_mat[++i])
	{
		new_mat[i] = ft_strdup(c_mat[i]);
		if (!new_mat[i])
			return (free_args(&new_mat), free_args(mat), \
			sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	}
	new_mat[i] = ft_strdup(str);
	if (!new_mat[i])
		return (free_args(&new_mat), free_args(mat), \
		sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	new_mat[++i] = NULL;
	return (free_args(mat), new_mat);
}

char	**ft_matcpy(char **mat)
{
	int		size;
	char	**new_mat;
	int		i;
	char	**c_mat;

	i = -1;
	size = ft_matsize(mat);
	new_mat = (char **)malloc(sizeof(char **) * (size + 1));
	if (!new_mat)
		return (NULL);
	c_mat = mat;
	while (c_mat[++i])
	{
		new_mat[i] = ft_strdup(c_mat[i]);
	}
	new_mat[i] = NULL;
	return (new_mat);
}
