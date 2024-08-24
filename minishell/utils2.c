/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:27:50 by adrian            #+#    #+#             */
/*   Updated: 2024/08/24 14:52:00 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*charstr(char c)
{
	char	*res;

	res = (char *)malloc(sizeof(char) * 2);
	if (!res)
		return (NULL);
	res[0] = c;
	res[1] = 0;
	return (res);
}

char	*new_str(char **str, char c)
{
	char	*res;
	char	*letter;

	if (!*str)
	{
		res = charstr(c);
		return (res);
	}
	else
	{
		letter = charstr(c);
		if (!letter)
			return (NULL);
		res = ft_strjoin(*str, letter);
		if (letter)
			free(letter);
		if (*str)
		{
			free(*str);
			*str = NULL;
		}
		return (res);
	}
}

int	is_special(char c, char *comp)
{
	char	*str;

	str = ft_strchr(comp, c);
	if (str)
		return (1);
	else
		return (0);
}

void	is_simple_string(t_token **token, char **env, char **str)
{
	char	*res;

	res = NULL;
	if (*str)
		res = ft_strdup(*str);
	*token = (*token)->next;
	while (*token && (*token)->value != '\'')
	{
		(void)env;
		res = new_str(&res, (*token)->value);
		if (*token)
			*token = (*token)->next;
	}
	if (*token)
		*token = (*token)->next;
	free(*str);
	*str = res;
}

/*if ((*token)->value == '$' && (*token)->next->value != '"')
{
	is_expandsor(token, &res, env, data);
	// if(*token && (*token)->next && (*token)->value != '"')
	// 	(*token) = (*token)->next;
}*/
void	is_double_string(t_token **token, char **env, char **str, t_data **data)
{
	char	*res;

	manage_early_is_double_string(&res, str);
	*token = (*token)->next;
	if (*token && (*token)->value == '"')
	{
		res = new_str(&res, 0);
		*token = (*token)->next;
		ft_free_char(str);
		*str = res;
		return ;
	}
	while (*token && (*token)->value != '"')
	{
		if ((*token)->value == '$' && (*token)->next->value != '"')
			is_expandsor(token, &res, env, data);
		if (*token && (*token)->value != '"')
			res = new_str(&res, (*token)->value);
		if (*token && (*token)->value != '"')
			*token = (*token)->next;
	}
	if (*token && (*token)->type == D_QUOTE)
		*token = (*token)->next;
	free(*str);
	*str = res;
}
