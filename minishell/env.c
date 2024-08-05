/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:35:01 by adrian            #+#    #+#             */
/*   Updated: 2024/08/05 20:41:15 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_env(char *key, char *val, char ***env)
{
	int		i;
	char	*res;
	char	*str;

	str = ft_strjoin(key, "=");
	if (!str)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	res = ft_strjoin(str, val);
	if (!res)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	i = index_env_env(*env, key);
	if (i >= 0)
	{
		free((*env)[i]);
		(*env)[i] = res;
	}
	else if (i == -1)
	{
		*env = ft_matadd(env, res);
		if (!*env)
			return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	}
	else
		return (sc_error(SC_RESOURCE_TEMPORARILY_UNAVAILABLE), 0);
	return (free(str), 1);
}

char	**set_env_i(char ***env)
{
    char 	*cwd;
	char	*pwd;
	int		size;

	size = 1;
	cwd = NULL;
	pwd = NULL;
	while (size < 10000 && !cwd)
		cwd = getcwd(cwd, size++);
	if (size == 10000)
		return (sc_error(EXIT_FAILURE), NULL);
	*env = (char **)malloc(sizeof(char *) * 1);
	if (!*env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	(*env)[0] = NULL;
	*env = ft_matadd(env, "SHLVL=0");
	*env = ft_matadd(env, "_=/usr/bin/env");
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	*env = ft_matadd(env, pwd);
	free(pwd);
	free(cwd);
	return (*env);
}

char	**create_env_first(char **cenv)
{
	int		i;
	char	**env;

	i = 0;
	env = NULL;
	//TODO Debe crear un mini env con: PWD(lo extrae con cwd()), SHLVL=1 y _???
	if (!cenv || !cenv[0])
		return (set_env_i(&env));
	while (cenv[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	i = -1;
	while (cenv[++i])
	{
		env[i] = ft_strdup(cenv[i]);
		if (!(env)[i])
			return (clean_env(&env, --i), sc_error(12), NULL);
	}
	env[i] = NULL;
	return (env);
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
		return (sc_error(EXIT_FAILURE), g_stat_code);
	unlink("/tmp/env.env");
	fd = open("/tmp/env.env", O_WRONLY | O_CREAT | O_TRUNC, 777);
	if (fd < 0)
		return (sc_error(SC_FILE_DESCRIPTOR_IN_BAD_STATE), g_stat_code);
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

void	ft_set_shell(char *env[], char ***mat, t_data **data)
{
	char	*key;
	int		fd;

	*data = NULL;
	*mat = create_env_first(env);
	if (!*mat)
	{
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
		return ;
	}
	key = ft_strdup("SHLVL");
	if (!key)
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	key = key_to_res(&key, *mat);
	if (!key)
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	fd = ft_atoi(key) + 1;
	free(key);
	key = ft_itoa(fd);
	if (!key)
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	if (!set_env("SHLVL", key, mat))
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	free (key);	
}
