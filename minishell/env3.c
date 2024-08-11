/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:36:08 by adrian            #+#    #+#             */
/*   Updated: 2024/08/11 23:12:05 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env_file(int fd, t_data **data)
{
	int		i;
	char	*env;
	char	*clean;
	char	**mat;

	i = count_lines("/tmp/env.env", data);
	fd = open("/tmp/env.env", O_RDONLY, 777);
	if (!i && is_valid_file("/tmp/env.env", fd, "R", data))
		return (sc_error(SC_PERMISSION_DENIED, data), NULL);
	mat = (char **)malloc(sizeof(char *) * (i + 1));
	if (!mat)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	i = -1;
	env = get_next_line(fd);
	while (env)
	{
		clean = ft_strtrim(env, "\n");
		if (!clean)
			return (sc_error(12, data), clean_env(&mat, --i), NULL);
		mat[++i] = clean;
		free(env);
		env = get_next_line(fd);
	}
	mat[++i] = NULL;
	return (close(fd), mat);
}

void	update_env(t_data *cdata, char *key, char *arg, int i)
{
	char	*new_entry;

	i = index_env(cdata, key);
	if (i >= 0)
	{
		new_entry = ft_strdup(cdata->env[i]);
		if (!new_entry)
			exit(*cdata->stat_code);
		free(cdata->env[i]);
		if (ft_strnstr(arg, "=", ft_strlen(arg)))
		{
			cdata->env[i] = ft_strdup(arg);
			if (!cdata->env[i])
				exit(*cdata->stat_code);
			free(new_entry);
		}
		else
			cdata->env[i] = new_entry;
	}
	else
	{
		cdata->env = ft_matadd(&(cdata)->env, arg, &cdata);
		if (!cdata->env)
			exit(*cdata->stat_code);

	}
}

int	index_env(t_data *data, char *str)
{
	int		i;
	char	**env;

	i = 0;
	env = data->env;
	if (!env)
		return (-2);
	while (env[i])
	{
		if (ft_strnstr(env[i], str, ft_strlen(str)) && \
				(env[i][ft_strlen(str)] \
				== '=' || env[i][ft_strlen(str)] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

void	b_env(t_data *data)
{
	int	i;

	i = -1;
	if (data->args[1])
	{
		ft_putstr_fd("Error, env not accept arguments\n", 2);
		sc_error(SC_KEY_HAS_EXPIRED, &data);
		exit(*data->stat_code);
	}
	if (data->env)
	{
		while (data->env[++i])
		{
			if (ft_strrchr(data->env[i], '='))
				printf("%s\n", data->env[i]);
		}
	}
	sc_error(SC_SUCCESS, &data);
	exit(*data->stat_code);
}

int	create_env(t_data **data, char **env)
{
	int	i;

	i = 0;
	if ((*data)->env)
		return (1);
	if (!env || !env[0])
		return (0);
	while (env[i])
		i++;
	(*data)->env = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	if (!(*data)->env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	while (env[++i])
	{
		(*data)->env[i] = ft_strdup(env[i]);
		if (!(*data)->env[i])
			return (clean_env(&(*data)->env, --i), 0);
	}
	(*data)->env[i] = NULL;
	return (1);
}
