/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 21:09:21 by kevin             #+#    #+#             */
/*   Updated: 2024/08/27 15:49:34 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_expandsor(t_token **token, char **str, char **env, t_data **data)
{
	*token = (*token)->next;
	if (!*token || is_special((*token)->value, "<> |\0"))
		*str = new_str(str, '$');
	else if ((*token)->value == '"')
		is_double_string(token, env, str, data);
	else if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else
		return (process_token(token, str, env, data));
	return (1);
}

int	switch_case(t_token **token, char **env, t_data **data, char **str)
{
	if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else if ((*token)->value == '"')
		is_double_string(token, env, str, data);
	else if ((*token)->value == '<')
		return (is_redir_input(token, data, str, env));
	else if ((*token)->value == '>')
		return (is_redir_output(token, data, str, env));
	else if ((*token)->value == '|')
		return (is_pipe(token, data, str, (*data)->stat_code));
	else if ((*token)->value == '$')
	{
		is_expandsor(token, str, env, data);
		(*data)->is_ex = 1;
	}
	else if ((*token)->value == ' ')
		is_space(token, data, str);
	return (1);
}

int	add_last_data(t_data **data, char **str)
{
	t_data	*n_data;
	char	**mat;
	int		i;

	i = 0;
	n_data = *data;
	while (n_data->next)
		n_data = n_data->next;
	if ((*data)->is_ex)
	{
		mat = ft_split(*str, ' ');
		if (!mat)
			return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
		while (mat[i])
		{
			if (!add_args(&n_data->args, &mat[i]))
				return (free_args(&mat), 0);
			i++;
		}
		free(mat);
		*str = NULL;
	}
	else if (!add_args(&n_data->args, str))
		return (0);
	return (1);
}

void	include_comand(t_data **data)
{
	t_data	*ldata;

	ldata = *data;
	while (ldata)
	{
		if (ldata->args)
			ldata->comand = ldata->args[0];
		ldata = ldata->next;
	}
}

int	split_token(t_token *token, char **env, t_data **data, int *sce)
{
	char	*str;

	if (!init_data(data, env, sce))
		return (free_data(data), 0);
	str = NULL;
	while (token)
	{
		if (is_special(token->value, " |\"'<>$"))
			switch_case(&token, (*data)->env, data, &str);
		else
		{
			str = new_str(&str, token->value);
			token = token->next;
		}
	}
	if (str)
	{
		if (!add_last_data(data, &str))
			return (0);
	}
	include_comand(data);
	return (1);
}
