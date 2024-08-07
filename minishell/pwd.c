/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:08:03 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 11:42:07 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pwd(t_data *data)
{
	char	*key;

	key = ft_strdup("PWD");
	if (!key)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), g_stat_code);
	key = key_to_res(&key, data->env);
	if (!key)
		return (g_stat_code);
	chdir(key);
	free(key);
	return (0);
}

void	find_oldpwd(char **res, char **pwd, t_data *data)
{
	free(*pwd);
	*res = ft_strdup("OLDPWD");
	if (!*res)
	{
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code));
	}
	*res = key_to_res(res, data->env);
	if (!*res || *res == NULL)
	{
		(sc_error(EXIT_FAILURE), \
		ft_putstr_fd("OLDPWD no está definido\n", 2), exit(g_stat_code));
	}
	*pwd = ft_strdup(*res);
	free(*res);
	*res = NULL;
}

void	find_pwd(char **res, t_data **data)
{
	if (res)
		free(res);
	*res = ft_strjoin("PWD=", "PWD");
	if (!*res)
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code));
	(*data)->env = ft_matadd(&(*data)->env, *res);
	if (!(*data)->env)
		(free(*res), exit(g_stat_code));
	free(*res);
	*res = NULL;
}
