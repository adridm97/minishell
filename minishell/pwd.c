/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:08:03 by adrian            #+#    #+#             */
/*   Updated: 2024/08/11 23:13:44 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pwd(t_data *data)
{
	char	*key;

	key = ft_strdup("PWD");
	if (!key)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, &data), *data->stat_code);
	key = key_to_res(&key, data->env);
	if (!key)
		return (*data->stat_code);
	chdir(key);
	ft_free_char(&key);
	return (0);
}

void	find_oldpwd(char **res, char **pwd, t_data *data)
{
	ft_free_char(pwd);
	*res = ft_strdup("OLDPWD");
	if (!*res)
	{
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY, &data), exit(*data->stat_code));
	}
	*res = key_to_res(res, data->env);
	if (!*res || *res == NULL)
	{
		(sc_error(EXIT_FAILURE, &data), \
		ft_putstr_fd("OLDPWD no está definido\n", 2), exit(*data->stat_code));
	}
	*pwd = ft_strdup(*res);
	ft_free_char(res);
	*res = NULL;
}

void	find_pwd(char **res, t_data **data)
{
	*res = ft_strjoin("PWD=", "PWD");
	if (!*res)
		(sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), exit(*(*data)->stat_code));
	(*data)->env = ft_matadd(&(*data)->env, *res, data);
	if (!(*data)->env)
		(ft_free_char(res), exit(*(*data)->stat_code));
	ft_free_char(res);
	*res = NULL;
}
