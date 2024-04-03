/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/04/03 21:16:02 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_error(t_token *token, char type)
{
	if (token->next && (type == token->next->type || (MINOR == type && \
			MAJOR == token->next->type)) && type != PIPE)
		token = token->next;
	while (token->next)
	{
		token = token->next;
		if (token->type != SPACES)
		{
			if (token->type >= PIPE && token->type <= MINOR)
				return (1);
			else
			return (0);
		}
	}
	return (1);
}

int	check_gramathic(t_token *token, t_data **data)
{
	char	flag;
	char	type;

	flag = 0;
	while (token)
	{
		if (token->type >= PIPE && token->type <= MINOR && !flag)
		{
			flag = 1; 
			type = token->type;
		}
		if (flag)
		{
			if (check_error(token, type))
			{
				(*data)->error.error = ft_strdup("Syntax error");
				(*data)->error.is_error = 1;
				return (ERROR);
			}
			flag = 0;
		}
		token = token->next;
	}
		return (1);
}

void	lexer_error(t_error *error)
{
	if(error->is_error)
	{
		printf(RED"Minishell: %s\n"BLACK, error->error);
		free(error->error);
		error->error = NULL;
		error->is_error = 0;
	}
}

int	check_closed(t_token *token, t_data **data)
{
	int quote;
	char open;

	open = 0;
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
		printf("%i\n", open);
	if (open)
	{
		(*data)->error.error = ft_strdup("Syntax error");
		(*data)->error.is_error = 1;
	}
	return (!open);
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

static void	print_token(t_token *token, char **comands)
{
	while (token->next)
	{
		printf("type: %i, value: %c\n", token->type, token->value);
		token = token->next;
	}
	printf("type: %i, value: %c\n", token->type, token->value);

	while (*comands)
	{
		printf("comand: %s\n", *comands++);
	}
}

void	go_data(t_data **data, char **comands)
{
	char **splited;
	(void)data;
	splited = special_split(*comands);
	while (*splited)
	{
		printf("splited: %s\n", *splited++);
	}
}

void	parser(t_data **data, t_token **token, char *input)
{
	char **comands;
	(void)data;
	comands = ft_split(input, '|');
	go_data(data, comands);
	print_token(*token, comands);
}

int	lexer(char *input, t_data *data)
{
	t_token	*token;
	int		i;

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
	{
		(check_closed(token, &data), check_gramathic(token, &data));
		if (data->error.is_error)
			lexer_error(&data->error);
		else
			parser(&data, &token, input);
	}
	//clean_list();
	return (1);
}


