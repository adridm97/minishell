/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:27:50 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 16:47:56 by adrian           ###   ########.fr       */
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

void	is_double_string(t_token **token, char **env, char **str)
{
	char	*res;

	res = NULL;
	if (*str)
		res = ft_strdup(*str);
	*token = (*token)->next;
	while (*token && (*token)->value != '"')
	{
		if ((*token)->value == '$')
		{
			is_expandsor(token, &res, env);
		}
		res = new_str(&res, (*token)->value);
		if (*token)
			*token = (*token)->next;
	}
	if (*token)
		*token = (*token)->next;
	free(*str);
	*str = res;
}
