/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:35:01 by adrian            #+#    #+#             */
/*   Updated: 2024/08/24 14:52:08 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_env(char *key, char *val, char ***env, t_data **data)
{
	int		i;
	char	*res;
	char	*str;

	str = ft_strjoin(key, "=");
	if (!str)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	res = ft_strjoin(str, val);
	if (!res)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	i = index_env_env(*env, key);
	if (i >= 0)
	{
		free((*env)[i]);
		(*env)[i] = res;
	}
	else if (i == -1)
	{
		*env = ft_matadd(env, res, data);
		if (!*env)
			return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	}
	else
		return (sc_error(SC_RESOURCE_TEMPORARILY_UNAVAILABLE, data), 0);
	return (free(str), 1);
}

char	**set_env_i(char ***env, t_data **data)
{
	char	*cwd;
	char	*pwd;
	int		size;

	size = 1;
	cwd = NULL;
	pwd = NULL;
	while (size < 10000 && !cwd)
		cwd = getcwd(cwd, size++);
	if (size == 10000)
		return (sc_error(EXIT_FAILURE, data), NULL);
	*env = (char **)malloc(sizeof(char *) * 1);
	if (!*env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	(*env)[0] = NULL;
	*env = ft_matadd(env, "SHLVL=0", data);
	*env = ft_matadd(env, "_=/usr/bin/env", data);
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	*env = ft_matadd(env, pwd, data);
	free(pwd);
	free(cwd);
	return (*env);
}

int	index_env_env(char **env, char *str)
{
	int		i;

	i = -1;
	if (!env)
		return (-2);
	while (env[++i])
	{
		if (ft_strnstr(env[i], str, ft_strlen(str)) && \
				(env[i][ft_strlen(str)] \
				== '=' || env[i][ft_strlen(str)] == '\0'))
			return (i);
	}
	return (-1);
}

int	save_env(t_data *data)
{
	int		fd;
	int		i;
	char	**env;

	i = 0;
	if (!data->env || !data->env[0])
		return (sc_error(EXIT_FAILURE, &data), *data->stat_code);
	unlink("/tmp/env.env");
	fd = open("/tmp/env.env", O_WRONLY | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
		return (sc_error(SC_FILE_DESCRIPTOR_IN_BAD_STATE, &data),
			*data->stat_code);
	env = data->env;
	while (env[i])
	{
		ft_putstr_fd(env[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	close(fd);
	return (0);
}

int	save_env_mat(char **env, int *stat_code)
{
	int		fd;
	int		i;

	i = 0;
	if (!env || !env[0])
	{
		*stat_code = EXIT_FAILURE;
		return (*stat_code);
	}
	unlink("/tmp/env.env");
	fd = open("/tmp/env.env", O_WRONLY | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
	{
		*stat_code = SC_FILE_DESCRIPTOR_IN_BAD_STATE;
		return (*stat_code);
	}
	while (env[i])
	{
		ft_putstr_fd(env[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	close(fd);
	return (0);
}
