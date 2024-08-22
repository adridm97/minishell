/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:08:03 by adrian            #+#    #+#             */
/*   Updated: 2024/08/22 22:06:56 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_char(char **f)
{
	if (*f)
		free(*f);
	*f = NULL;
}

void	managing_env(char **res, int i, char **last_pwd, t_data **data)
{
	if (i == -2)
	{
		sc_error(SC_RESOURCE_TEMPORARILY_UNAVAILABLE, data);
		exit(*(*data)->stat_code);
	}
	*res = ft_strjoin("OLDPWD=", *last_pwd);
	if (!*res)
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), exit(*(*data)->stat_code));
	(*data)->env = ft_matadd(&(*data)->env, *res, data);
	if (!(*data)->env)
		exit(*(*data)->stat_code);
	ft_free_char(res);
	*res = NULL;
}

void	ft_oldpwd(t_data **data, char **last_pwd, char **res)
{
	int	i;

	i = index_env(*data, "OLDPWD");
	if (i >= 0)
	{
		ft_free_char(&(*data)->env[i]);
		(*data)->env[i] = ft_strjoin("OLDPWD=",*last_pwd);
	}
	else
		managing_env(res, i, last_pwd, data);
	if (!(*data)->env)
		(ft_free_char(&(*data)->env[index_env((*data), "PWD")]), \
		sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), exit(*(*data)->stat_code));
	unlink("/tmp/env.env");
	if (save_env(*data))
		(clean_env(&(*data)->env, -1), ft_free_char(last_pwd), \
		ft_free_char(res), exit(*(*data)->stat_code));
}

void	ft_pwd(char **pwd, char **res, t_data **data, int size)
{
	int	i;

	ft_free_char(pwd);
	i = index_env((*data), "PWD");
	if (i < 0)
		find_pwd(res, data);
	while (size < 10000 && !*pwd)
		*pwd = getcwd(*pwd, size++);
	if (size == 10000 || !*pwd)
	{
		if (size == 10000)
		{
			*pwd = ft_strdup("PWD");
			*pwd = key_to_res(pwd, (*data)->env);
		}
		if (!*pwd)
			(sc_error(EXIT_FAILURE, data), exit(*(*data)->stat_code));
	}
	i = index_env((*data), "PWD");
	ft_free_char(&(*data)->env[i]);
	(*data)->env[i] = ft_strjoin("PWD=", *pwd);
	ft_free_char(pwd);
	if (!(*data)->env[i])
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), exit(*(*data)->stat_code));
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
			(sc_error(EXIT_FAILURE, &data), exit(*data->stat_code));
	}
	printf("%s\n", buff);
	ft_free_char(&buff);
	sc_error(SC_SUCCESS, &data);
	exit(*data->stat_code);
}
