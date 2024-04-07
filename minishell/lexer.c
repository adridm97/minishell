/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/04/07 13:02:06 by kevin            ###   ########.fr       */
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

void	lexer_error(t_error *error)
{
	if(error->is_error)
	{
		printf(RED"Minishell: %s\n"BLACK, error->error);
		error->error = NULL;
		error->is_error = 0;
	}
}

int	check_closed(t_token *token, t_error *error)
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
	if (open)
	{
		error->error = ft_strdup("Syntax error");
		error->is_error = 1;
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

static void	print_data(t_data *data)
{
	int i;
	int j;

	i = 0;
	j = -1;
	while (data)
	{
		printf("data %i\nComand: %s\n",i, data->comand);
		while(data->args[++j])
			printf("arg[%i]: %s\n", j, data->args[j]);
		j = -1;
		while(data->redir)
		{
			printf("Redirs\n");
			printf("path: %s, type: %i\n", data->redir->path, data->redir->type);
			data->redir = data->redir->next;
		}
		data = data->next;
		j = -1;
		i++;
		printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	}
}

// static void	print_token(t_token *token, char **comands)
// {
// 	while (token->next)
// 	{
// 		printf("type: %i, value: %c\n", token->type, token->value);
// 		token = token->next;
// 	}
// 	printf("type: %i, value: %c\n", token->type, token->value);

// 	while (*comands)
// 	{
// 		printf("comand: %s\n", *comands++);
// 	}
// }

int	count_redir(char **splited)
{
	int i;

	i = 0;
	while (splited[i])
		i++;
	return i;
}
/*TODO Gestionar la redirección <>*/
int	search_special(char *comands, int j)
{
	int type;

	while (*comands)
	{
		if (j >= 0)
		{
			if (*comands == '<')
			{
				comands++;
				if (*comands == '<')
					type = D_MINOR;
				else
				{
					type = MINOR;
					comands--;
				}
				j--;
			}
			else if (*comands == '>')
			{
				comands++;
				if (*comands == '>')
					type = D_MAJOR;
				else
				{
					type = MAJOR;
					comands--;
				}
				j--;
			}
		}
		comands++;
	}
	return (type);
}

void	add_redir(t_redir **res, t_redir **list)
{
	t_redir *last;

	if (!*res)
	{
		(*res) = *list;
		(*res)->next = NULL;
		return ;	
	}
	last = *res;
	while (last->next)
		last = last->next;
	last->next = *list;
	(*list)->next = NULL;
}

void	free_redir(t_redir **redir)
{
	t_redir *i;
	if (!(*redir))
		return ;
	while ((*redir)->next)
	{
		i = *redir;
		free((*redir)->path);
		free(*redir);
		(*redir) = i;
	}
	free((*redir)->path);
	free(*redir);
}

void	free_args(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (args[i])
			free(args[i]);
	}
	if (args)
	free(args);
}

void	free_data(t_data **data)
{
	t_data *del;

	while ((*data)->next)
	{
		del = (*data)->next;
		free_args((*data)->args);
		free_redir(&(*data)->redir);
		free(*data);
		*data = del;
	}
	free_args((*data)->args);
	free_redir(&(*data)->redir);
	free(*data);
	*data = NULL;
}

void	add_data(t_data **data, t_data **data_lst)
{
	t_data *last;
	if (!*data)
	{
		*data = *data_lst;
		return ;
	}
	last = *data;
	while (last->next)
		last = last->next;
	last->next = *data_lst;
}

int	redir_init(t_data **data, char **splited, char *comands, int n_redir)
{
	t_redir *res;
	t_redir *list;
	int	i;

	i = 0;
	res = NULL;
	while (i < n_redir)
	{
		list = (t_redir*)malloc(sizeof(t_redir));
		if (!list)
			return (free_redir(&res), lexer_error(&(t_error){"Memory error",1}), ERROR);
		list->path = splited[i];
		list->type = search_special(comands, i);
		add_redir(&res, &list);
		i++;
	}
	(*data)->redir = res;
	return (1);
}

int	search_quotes(t_token *token)
{
	while (token)
	{
		if (token->type == QUOTE || token->type == D_QUOTE)
			return (1);
		token = token->next;
	}
	return (0);
}

char	**repair_quots(char **args, t_token *token)
{
	if (!search_quotes(token))
		return(args);
	return (args);
}
int	go_data(t_data **data, char **comands, t_token *token)
{
	char	**splited;
	char	**args;
	t_data	*data_lst;
	int		i;

	i = -1;
	while (comands[++i])
	{
		data_lst = (t_data *)malloc(sizeof(t_data));
		if (!data_lst)
			return (free_data(data), lexer_error(&(t_error){"Memory error", 1}), ERROR);
		splited = special_split(comands[i]);
		if (!splited)
			return (free_data(data), lexer_error(&(t_error){"Memory error", 1}), ERROR);
		args = ft_split(splited[0], ' ');
		data_lst->comand = args[0];
		data_lst->args = repair_quots(args, token);
		(free(splited[0]), splited++);
		if (!redir_init(&data_lst, splited, comands[i], count_redir(splited)))
			return (ERROR);
		data_lst->next = NULL;
		add_data(data, &data_lst);
	}
	print_data(*data);
	return (1);
}

int	parser(t_data **data, t_token **token, char *input)
{
	char **comands;

	(void)token;
	comands = ft_split(input, '|');
	if (go_data(data, comands, *token))
		return (ERROR);
	return (1);
}
/*TODO Este caso rompe con segfault: ahola>adios|adios>ee||*/
t_data	*lexer(char *input, t_data *data)
{
	t_token	*token;
	t_error	error;
	int		i;

	i = -1;
	token = NULL;
	error.is_error = 0;
	while (input[++i])
	{
		if (!token)
		{
			if (!new_token(input[i], typeing(input[i], " |><\'\""), &token))
				return (lexer_error(&(t_error){"Memory error",1}), NULL);
		}
		else
		{
			if (!add_token(input[i], typeing(input[i], " |><\'\""), &token))
				return (lexer_error(&(t_error){"Memory error",1}), NULL);
		}
	}
	(check_closed(token, &error), check_gramathic(token, &error));
	if (error.is_error)
		lexer_error(&error);
	else	
	if (parser(&data, &token, input))
		data = NULL;
	//clean_list();
	return (data);
}
