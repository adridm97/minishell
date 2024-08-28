/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 14:12:39 by adrian            #+#    #+#             */
/*   Updated: 2024/08/11 23:12:47 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	b_export(t_data **data)
{
	int	j;

	j = 0;
	if ((*data)->pipe)
	{
		sc_error(SC_SUCCESS, data);
		exit(*(*data)->stat_code);
	}
	if (!(*data)->args[1])
		return (print_export(*data, "declare -x "));
	while ((*data)->args[++j])
	{
		process_argument(*data, (*data)->args[j]);
	}
	finalize_export(*data);
}

void	process_argument(t_data *cdata, char *arg)
{
	char	*key;

	key = extract_key(arg, &cdata);
	if (!is_valid_key(key))
	{
		free(key);
		return (sc_error(SC_INVALID_ARGUMENT, &cdata), \
		printf("export: `%s': not a valid identifier\n", arg), \
		exit(*cdata->stat_code));
	}
	update_env(cdata, key, arg, 0);
	free(key);
}

void	finalize_export(t_data *cdata)
{
	unlink("/tmp/env.env");
	if (save_env(cdata))
		exit(*cdata->stat_code);
	sc_error(SC_SUCCESS, &cdata);
	exit(*cdata->stat_code);
}

char	*extract_key(char *arg, t_data **data)
{
	char	*need;
	char	*key;

	need = ft_strnstr(arg, "=", ft_strlen(arg));
	if (need)
	{
		key = (char *)malloc((need - arg) + 1);
		if (!key)
			exit(*(*data)->stat_code);
		ft_strlcpy(key, arg, (need - arg) + 1);
	}
	else
	{
		key = ft_strdup(arg);
		if (!key)
			exit(*(*data)->stat_code);
	}
	return (key);
}

int	is_valid_key(char *key)
{
	if(key && (key[0] != '_' && !ft_isalpha(key[0])))
		return (0);
	return (*key && !is_special_string(key, " <>|'\".,-+*!¡?¿%%=·@$#ªº¬€"));
}
