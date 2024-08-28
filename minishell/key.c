/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:21:05 by adrian            #+#    #+#             */
/*   Updated: 2024/08/27 16:23:11 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	take_key(t_token **token, char **key, char *str)
{
	t_token	*ctoken;

	ctoken = *token;
	if (!is_special(ctoken->value, str) && ctoken->value == '?')
	{
		*key = new_str(key, ctoken->value);
		ctoken = ctoken->next;
		(*token) = ctoken;
		if (*key)
			return (1);
		else
			return (0);
	}
	while (ctoken && !is_special(ctoken->value, str))
	{
		*key = new_str(key, ctoken->value);
		ctoken = ctoken->next;
		(*token) = ctoken;
	}
	if (*key)
		return (1);
	else
		return (0);
}

char	*key_to_res(char **key, char **env)
{
	int		i;
	char	*res;

	i = -1;
	res = NULL;
	while (env[++i])
	{
		if (ft_strnstr(env[i], *key, ft_strlen(*key)) \
				&& env[i][ft_strlen(*key)] == '=')
		{
			res = ft_strdup(&env[i][ft_strlen(*key) + 1]);
			free(*key);
			return (res);
		}
	}
	free (*key);
	return (NULL);
}

int	handle_key(char **str, char **env, char **key, t_data **data)
{
	char	*res;

	*key = key_to_res(key, env);
	if (*key)
	{
		if (!*str)
			res = ft_strjoin("", *key);
		else
			res = ft_strjoin(*str, *key);
		(free(*str), free(*key));
		*str = res;
	}
	else
	{
		sc_error(SC_CANNOT_ALLOCATE_MEMORY, data);
		return (0);
	}
	return (1);
}

int	handle_string(char **str)
{
	*str = new_str(str, '$');
	return (1);
}

int	process_token(t_token **token, char **str, char **env, t_data **data)
{
	char	*key;

	key = NULL;
	if (take_key(token, &key, " <>|'\".,-+*!¡?¿%%=·@#ªº¬€$"))
		return (handle_key(str, env, &key, data));
	else if (take_key(token, &key, " <>|'\".,-+*!¡¿%%=·@#ªº¬€$"))
		return (handle_status_code(str, key, data));
	else
		return (handle_string(str));
}
