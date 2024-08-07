/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:21:28 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 21:38:47 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_home(char **res, char **pwd, t_data *data)
{
	if (*pwd)
		ft_free_char(pwd);
	*res = ft_strdup("HOME");
	if (!*res)
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code));
	*res = key_to_res(res, data->env);
	if (!*res)
		(sc_error(EXIT_FAILURE), perror("HOME no está definido"), \
		exit(g_stat_code));
	*pwd = ft_strdup(*res);
	ft_free_char(res);
}

void	init_cd(char **last_pwd, char **pwd, int size, t_data *data)
{
	*last_pwd = NULL;
	*pwd = NULL;
	size = 1;
	if (data->pipe)
		(sc_error(0), exit(g_stat_code));
	if (data->args[1])
		*pwd = ft_strdup(data->args[1]);
	while (size < 10000 && !*last_pwd)
		*last_pwd = getcwd(*last_pwd, size++);
	if (size == 10000 || !last_pwd)
	{
		*last_pwd = ft_strdup("PWD");
		*last_pwd = key_to_res(last_pwd, data->env);
		if (!*last_pwd)
		{
			*last_pwd = ft_strdup("OLDPWD");
			*last_pwd = key_to_res(last_pwd, data->env);
		}
		if (!*last_pwd)
		{
			(ft_free_char(pwd), sc_error(SC_KEY_HAS_BEEN_REVOKED));
			*pwd = ft_strdup("/");
			*last_pwd = ft_strdup("/");
		}
	}
}

void	b_cd(t_data **data, char *home)
{
	char	*last_pwd;
	char	*pwd;
	char	*res;

	init_cd(&last_pwd, &pwd, 1, *data);
	if (!pwd)
		find_home(&res, &pwd, *data);
	else if (!ft_strcmp(pwd, "-"))
		find_oldpwd(&res, &pwd, *data);
	if (ft_strrchr(pwd, '~'))
	{
		res = ft_substr(pwd, 1, ft_strlen(pwd) - 1);
		if (!res)
			res = NULL;
		ft_free_char(&pwd);
		pwd = ft_strjoin(home, res);
	}
	if (!chdir(pwd))
		(ft_pwd(&pwd, &res, data, 0), ft_oldpwd(data, &last_pwd, &res));
	else
		(ft_putstr_fd("La ruta especificada no existe\n", 2), \
		ft_free_char(&pwd), sc_error(EXIT_FAILURE), exit(g_stat_code));
	(free_args(&(*data)->env), sc_error(SC_SUCCESS));
	exit(g_stat_code);
}
