/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/04/06 12:47:06 by aduenas-         ###   ########.fr       */
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

void	redir_init(t_data **data, char **splited, char *comands, int n_redir)
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
			return ;
		list->path = splited[i];
		list->type = search_special(comands, i);
		add_redir(&res, &list);
		i++;
	}
	(*data)->redir = res;
}

void	go_data(t_data **data, char **comands)
{
	char	**splited;
	char	**args;
	t_data	*data_lst;
	int		i;

	i = -1;
	while (comands[++i])
	{
		data_lst = (t_data *)malloc(sizeof(t_data));
		splited = special_split(comands[i]);
		args = ft_split(splited[0], ' ');
		data_lst->comand = args[0];
		data_lst->args = args;
		splited++;
		redir_init(&data_lst, splited, comands[i], count_redir(splited));
		data_lst->next = NULL;
		add_data(data, &data_lst);
	}
	print_data(*data);

}

void	parser(t_data **data, t_token **token, char *input)
{
	char **comands;
	(void)token;
	comands = ft_split(input, '|');
	go_data(data, comands);
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
//	if (token)
//		print_token(token);
//	if (lexer_error(data->error))
//		return (0);
	{
		(check_closed(token, &data), check_gramathic(token, &data));
		// if ((*data)->error.is_error)
		// 	lexer_error(&(*data)->error);
		// else
			parser(&data, &token, input);
	}
	//clean_list();
	return (1);
}


