/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:08:03 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 11:51:36 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	managing_env(char **res, int i, char **last_pwd, t_data **data)
{
	if (i == -2)
		(sc_error(SC_RESOURCE_TEMPORARILY_UNAVAILABLE), exit(g_stat_code));
	*res = ft_strjoin("OLDPWD=", *last_pwd);
	if (!*res)
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code));
	(*data)->env = ft_matadd(&(*data)->env, *res);
	if (!(*data)->env)
		exit(g_stat_code);
	free(*res);
	*res = NULL;
}

void	ft_oldpwd(t_data **data, char **last_pwd, char **res)
{
	int	i;

	i = index_env(*data, "OLDPWD");
	if (i >= 0)
	{
		free((*data)->env[i]);
		(*data)->env[i] = ft_strjoin("OLDPWD=",*last_pwd);
	}
	else
		managing_env(res, i, last_pwd, data);
	if (!(*data)->env)
		(free((*data)->env[index_env((*data), "PWD")]), \
		sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code));
	unlink("/tmp/env.env");
	if (save_env(*data))
		(clean_env(&(*data)->env, -1), free(*last_pwd), \
		free(*res), exit(g_stat_code));
}
/*
// (free((*data)->env[index_env((*data), "PWD")]), \
// free((*data)->env[index_env((*data), "OLDPWD")]), free(*last_pwd), \
// free(*res), exit(g_stat_code));
*/

void	ft_pwd(char **pwd, char **res, t_data **data, int size)
{
	int	i;

	free(*pwd);
	*pwd = NULL;
	i = index_env((*data), "PWD");
	if (i < 0)
		find_pwd(res, data);
	while (size < 10000 && !*pwd)
		*pwd = getcwd(*pwd, size++);
	if (size == 10000)
	{
		if (size == 10000)
		{
			*pwd = ft_strdup("PWD");
			*pwd = key_to_res(pwd, (*data)->env);
		}
		if (!*pwd)
			(sc_error(EXIT_FAILURE), exit(g_stat_code));
	}
	i = index_env((*data), "PWD");
	free((*data)->env[i]);
	(*data)->env[i] = ft_strjoin("PWD=", *pwd);
	free(*pwd);
	if (!(*data)->env[i])
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code));
}

void	b_pwd(t_data *data)
{
	size_t	size;
	char	*buff;

	size = 1;
	buff = NULL;
	while (size < 10000 && !buff)
	{
		buff = getcwd(buff, size++);
	}
	if (size == 10000)
	{
		buff = ft_strdup("PWD");
		buff = key_to_res(&buff, data->env);
		if (!buff)
			(sc_error(EXIT_FAILURE), exit(g_stat_code));
	}
	printf("%s\n", buff);
	free(buff);
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}
