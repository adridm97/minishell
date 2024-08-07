/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:55:56 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 17:27:39 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	b_unset(t_data *data)
{
	int	i;
	int	j;

	if (data->pipe)
		(sc_error(0), exit(g_stat_code));
	j = 0;
	while (data->args[++j])
	{
		i = index_env(data, data->args[j]);
		if (i != -1)
			data->env = ft_mat_rem_index(&data->env, i);
	}
	unlink("/tmp/env.env");
	if (save_env(data))
		exit(g_stat_code);
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

char	**ft_mat_rem_index(char ***mat, int index)
{
	char	**new_mat;
	int		i;
	int		j;
	char	**c_mat;

	i = -1;
	j = -1;
	new_mat = (char **)malloc(sizeof(char **) * (ft_matsize(*mat)));
	if (!new_mat)
		return (NULL);
	c_mat = *mat;
	while (c_mat[++i])
	{
		if (c_mat[i] && i != index)
			new_mat[++j] = ft_strdup(c_mat[i]);
	}
	new_mat[++j] = NULL;
	free_args(mat);
	return (new_mat);
}

void	is_space(t_token **token, t_data **data, char **str)
{
	if (*token)
		*token = (*token)->next;
	if (*str)
		add_last_data(data, str);
}
