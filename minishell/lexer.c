/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/05/26 17:18:24 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token **token)
{
	t_token	*c_token;

	if (*token)
	{
		c_token = (*token)->next;
		while (*token)
		{
			free (*token);
			*token = c_token;
			if (*token)
				c_token = (*token)->next;
		}
	}
}

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

int	check_gramathic(t_token *token, t_error *error)
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
				if (!error->error)
					error->error = ft_strdup("Syntax error");
				error->is_error = 1;
				return (ERROR);
			}
			flag = 0;
		}
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
	if (open && !error->error)
		error->is_error = 1;
	return (!open);
}

int	new_token(char c, int type, t_token **token, int index)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (0);
	(*new).next = NULL;
	(*new).value = c;
	(*new).type = type;
	(*new).index = index;
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
	if (!new_token(c, type, &new, lst->index + 1))
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

void	print_redir(t_redir *redir)
{
	if (!redir)
		return ;
	while (redir)
	{
		printf("Redirs\n");
		printf("path: %s, type: %i\n", redir->path, redir->type);
		redir = redir->next;
	}
}

static void	print_data(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (data)
	{
		printf("data %i\nComand: %s|\n", i, data->comand);
		while (data->args && data->args[++j])
			printf("arg[%i]: %s|\nnext = %p\n", j, data->args[j], data->next);
		j = -1;
		print_redir(data->redir);
		data = data->next;
		j = -1;
		i++;
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	}
}

void	free_redir(t_redir **redir)
{
	t_redir	*i;

	if (!(*redir))
		return ;
	while ((*redir))
	{
		i = (*redir)->next;
		free((*redir)->path);
		free(*redir);
		(*redir) = i;
	}
	redir = NULL;
}

void	free_args(char **args)
{
	int	i;

	i = -1;
	if (*args)
	{
		while (args[++i])
		{
			if (args[i])
				free(args[i]);
		}
		free(args);
	}
	args = NULL;
}

void	free_data(t_data **data)
{
	t_data	*del;

	if (!*data)
		return ;
	while ((*data)->next)
	{
		del = (*data)->next;
		free_args((*data)->args);
		free_redir(&(*data)->redir);
		free(*data);
		*data = del;
	}
	if ((*data)->args)
		free_args((*data)->args);
	if ((*data)->redir)
		free_redir(&(*data)->redir);
	free(*data);
	*data = NULL;
}

int	init_data(t_data **data)
{
	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (0);
	(*data)->args = NULL;
	(*data)->comand = NULL;
	(*data)->next = NULL;
	(*data)->path = NULL;
	(*data)->redir = NULL;
	return (1);
}

t_data	*lexer(char *input, t_data *data, char **env)
{
	t_token	*token;
	t_error	error;
	int		i;

	i = -1;
	token = NULL;
	error.is_error = 0;
	error.error = NULL;
	while (input[++i])
	{
		if (!token)
		{
			if (!new_token(input[i], typeing(input[i], " |><\'\""), &token, 0))
				return (is_error(&(t_error){"Memory error",1}), free_token(&token), NULL);
		}
		else
		{
			if (!add_token(input[i], typeing(input[i], " |><\'\""), &token))
				return (is_error(&(t_error){"Memory error",1}), free_token(&token), NULL);
		}
	}
	check_closed(token, &error);
	check_gramathic(token, &error);
	if (error.is_error)
		return(is_error(&(t_error){"Syntax error",1}), free(error.error), free_data(&data), free_token(&token), NULL);
	else if (!split_token(token, env, &data))
		return (is_error(&(t_error){"Memory error",1}), free_data(&data), free_token(&token), NULL);
	print_data(data);
	free_token(&token);
	return (data);
}
