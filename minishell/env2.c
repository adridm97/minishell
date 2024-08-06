/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 21:27:52 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/06 22:55:45 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_shlvl(int *n)
{
	if (*n < 0)
		*n = 0;
	else if (*n >= 1000)
	{
		*n = 1;
		printf("warning: shell level (1000) too high, resetting to 1\n");
	}
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
	check_shlvl(&fd);
	key = ft_itoa(fd);
	if (!key)
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	if (!set_env("SHLVL", key, mat))
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	free (key);
}

void	print_env_env(char **env, char *str)
{
	int	i;

	i = -1;
	if (env)
	{
		while (env[++i])
			printf("%s%s\n", str, env[i]);
	}
}

void	ft_handle_env_file(char ***mat)
{
	int	fd;

	fd = open("/tmp/env.env", O_RDONLY);
	if (is_valid_file("/tmp/env.env", fd, "R"))
		sc_error(SC_PERMISSION_DENIED);
	if (mat && fd >= 0)
	{
		*mat = get_env_file(fd);
		close(fd);
		unlink("/tmp/env.env");
	}
}
