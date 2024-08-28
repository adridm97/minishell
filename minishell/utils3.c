/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 14:51:41 by kevin             #+#    #+#             */
/*   Updated: 2024/08/29 00:51:07 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_env_first(char **cenv, t_data **data)
{
	int		i;
	char	**env;

	i = 0;
	env = NULL;
	if (!cenv || !cenv[0])
		return (set_env_i(&env, data));
	while (cenv[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), NULL);
	i = -1;
	while (cenv[++i])
	{
		env[i] = ft_strdup(cenv[i]);
		if (!(env)[i])
			return (clean_env(&env, --i), sc_error(12, data), NULL);
	}
	env[i] = NULL;
	return (env);
}

void	manage_early_is_double_string(char **res, char **str, t_data **data)
{
	(*data)->is_ex = 0;
	*res = NULL;
	if (*str)
		*res = ft_strdup(*str);
}

int	token_to_str(t_token **token, char **res, t_data **data)
{
	if (is_special((*token)->value, "$") && \
	(!is_special((*token)->next->value, "\"'")))
	{
		if (!is_expandsor(token, res, (*data)->env, data))
			return (free_token(token),
				sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	}
	else
	{
		if ((*token))
		{
			*res = new_str(res, (*token)->value);
			(*token) = (*token)->next;
		}
	}
	return (1);
}

int	check_puti(t_data *d)
{
	if (d->args[1] && !d->args[2] && ft_atoi(d->args[1]) == 235)
		return (1);
	return (0);
}

int	transform_token_to_string(t_token **dtoken, char **res, t_data **data)
{
	t_token	*token;

	token = *dtoken;
	while (token)
	{
		if (!token_to_str(&token, res, data))
			return (free_token(&token), sc_error(12, data), 1);
	}
	return (0);
}
