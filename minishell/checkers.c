/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:39:20 by adrian            #+#    #+#             */
/*   Updated: 2024/08/19 07:56:38 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_error(t_token *token, char type, int flag)
{
	if (flag == -1)
		return (1);
	if (token->next && (type == token->next->type || (MINOR == type && \
			MAJOR == token->next->type)) && type != PIPE)
		token = token->next;
	while (token)
	{
		token = token->next;
		if (token && token->type != SPACES)
		{
			/*TODO, antes era esta, si falla el lexer es por este if if (token->type >= PIPE && token->type <= MINOR && (type == PIPE \
			&& !(token->type > PIPE && token->type <= MINOR)))*/
			if ((token->type >= MAJOR && token->type <= MINOR) && type == PIPE )
				return (0);
			if (token->type >= PIPE && token->type <= MINOR)
				return (1);
			else
				return (0);
		}
	}
	return (1);
}

int	check_gramathic(t_token *token, t_error *error, char flag, char type)
{
	while (token)
	{
		if (token->type >= PIPE && token->type <= MINOR)
		{
			if (flag != -1 || token->type != PIPE)
				flag = 1;
			type = token->type;
		}
		if (flag && token->type >= PIPE && token->type <= MINOR)
		{
			if (check_error(token, type, flag))
			{
				if (!error->error)
					error->error = ft_strdup("Syntax error");
				error->is_error = 1;
				return (ERROR);
			}
			flag = 0;
		}
		if (flag == -1 && !(token->type >= PIPE && token->type <= MINOR) && \
		token->type != SPACES)
			flag = 0;
		token = token->next;
	}
	return (1);
}

void	is_error(t_error *error)
{
	if (error->is_error)
	{
		printf(RED"Minishell: %s\n"BLACK, error->error);
		error->error = NULL;
		error->is_error = 0;
	}
}

int	check_closed(t_token *token, t_error *error)
{
	int		quote;
	char	open;

	quote = 0;
	open = 0;
	if (!token)
	{
		if (!error->error)
			error->is_error = 1;
		return (!open);
	}
	while (token)
	{
		if ((token->type == QUOTE || token->type == D_QUOTE) && !open)
		{
			open = 1;
			quote = token->type;
		}
		else if (token->type == quote && open)
			open = 0;
		token = token->next;
	}
	if (open && !error->error)
		error->is_error = 1;
	return (!open);
}

int	typeing(char c, char *base)
{
	int	i;

	i = -1;
	while (++i <= 5)
	{
		if (c == base[i])
			return (i);
	}
	return (6);
}
