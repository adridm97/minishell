


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

char	**create_env_first(char **cenv)
{
	int		i;
	char	**env;

	i = 0;
	if (!cenv || !cenv[0])
		return (NULL);
	while (cenv[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env)
		return(sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	i = -1;
	while (cenv[++i])
	{
		env[i] = ft_strdup(cenv[i]);
		if (!(env)[i])
			return (clean_env(&env, --i), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
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

void	ft_set_shell(char *env[], char ***mat)
{
	char	*key;
	int		fd;
	*mat = create_env_first(env);
	if (!*mat)
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
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