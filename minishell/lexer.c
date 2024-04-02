/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/03/31 22:42:28 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_closed(t_token *token)
{
	int	q;
	int	dq;

	q = 0;
	dq = 0;
	if (token->type == QUOTE)
		q++;
	if (token->type == D_QUOTE)
		dq++;
	while (token->next)
	{
		token = token->next;
		if (token->type == QUOTE)
			q++;
		if (token->type == D_QUOTE)
			dq++;
	}
	if (q % 2 || dq % 2)
		return (ERROR);
	else
		return (1);
}

int	new_token(char c, int type, t_token **token)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (0);
	(*new).next = NULL;
	(*new).value = c;
	(*new).type = type;
	*token = new;
	return (1);
}

int	add_token(char c, int type, t_token **token)
{
	t_token	*lst;
	t_token	*new;

	new = NULL;
	lst = *token;
	while (lst->next)
	{
		lst = lst->next;
	}
	if (!new_token(c, type, &new))
		return (0);
	lst->next = new;
	return (1);
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

static void	print_token(t_token *token)
{
	while (token->next)
	{
		printf("type: %i, value: %c\n", token->type, token->value);
		token = token->next;
	}
	printf("type: %i, value: %c\n", token->type, token->value);
}


int	lexer(char *input, t_data *data)
{
	t_token	*token;
	int		i;
	int		open;

	(void)data;
	open = 0;
	i = -1;
	token = NULL;
	while (input[++i])
	{
		if (!token)
		{
			if (!new_token(input[i], typeing(input[i], " |><\'\""), &token))
				return (0);
		}
		else
		{
			if (!add_token(input[i], typeing(input[i], " |><\'\""), &token))
				return (0);
		}
	}
	if (token)
		print_token(token);
	if (lexer_error(data->error))
		return (0);
	//clean_list();
	return (1);
}
