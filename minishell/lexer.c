/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/07 00:58:06 by aduenas-         ###   ########.fr       */
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

int	check_error(t_token *token, char type, int flag)
{
	if(flag == -1)
		return (1);
	if (token->next && (type == token->next->type || (MINOR == type && \
			MAJOR == token->next->type)) && type != PIPE)
		token = token->next;
	while (token->next)
	{

		token = token->next;
		if (token->type != SPACES)
		{
			if (token->type >= PIPE && token->type <= MINOR && (type == PIPE 
				&& !(token->type > PIPE && token->type <= MINOR)))
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
		if(flag == -1 && !(token->type >= PIPE && token->type <= MINOR) && token->type != SPACES)
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

void	print_data(t_data *data)
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
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		print_redir(data->redir);
		data = data->next;
		j = -1;
		i++;
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

void	free_args(char ***args)
{
	int	i;

	i = -1;
	if (*args)
	{
		while ((*args)[++i])
		{
			if ((*args)[i])
				free((*args)[i]);
		}
		free(*args);
	}
	*args = NULL;
}

void	free_data(t_data **data)
{
	t_data	*del;

	if (!*data)
		return ;
	while ((*data)->next)
	{
		del = (*data)->next;
		free_args(&(*data)->args);
		free_redir(&(*data)->redir);
		clean_env(&(*data)->env, -1);
		free(*data);
		*data = del;
	}
	clean_env(&(*data)->env, -1);
	if ((*data)->args)
		free_args(&(*data)->args);
	if ((*data)->redir)
		free_redir(&(*data)->redir);
	free(*data);
	*data = NULL;
}

void	clean_env(char ***env, int i)
{
	if (!*env)
		return ;
	if (i == -1)
	{
		while (env[0][++i])
		{
			free(env[0][i]);
		}
		free(*env);
	}
	else
	{
		while (--i >= 0)
			free(env[0][i]);
		free(*env);
	}
	*env = NULL;
}

int	create_env(t_data **data, char **env)
{
	int	i;

	i = 0;
	if ((*data)->env)
		return (1);
	if (!env || !env[0])
		return (0);
	while (env[i])
		i++;
	(*data)->env = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	if (!(*data)->env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	while (env[++i])
	{
		(*data)->env[i] = ft_strdup(env[i]);
		if (!(*data)->env[i])
			return (clean_env(&(*data)->env, --i), 0);
	}
	(*data)->env[i] = NULL;
	return (1);
}

int	count_lines(char *file)
{
	int		fd;
	int		i;
	char	*line;

	fd = open(file, O_RDONLY, 777);
	if (is_valid_file(file, fd, "R"))
		return (sc_error(SC_PERMISSION_DENIED), 0);
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	close(fd);
	return (i);
}

char	**get_env_file(int fd)
{
	int		i;
	char	*env;
	char	*clean;
	char	**mat;

	i = count_lines("/tmp/env.env");
	fd = open("/tmp/env.env", O_RDONLY, 777);
	if (!i && is_valid_file("/tmp/env.env", fd, "R"))
		return (sc_error(SC_PERMISSION_DENIED), NULL);
	mat = (char **)malloc(sizeof(char *) * (i + 1));
	if (!mat)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	i = -1;
	env = get_next_line(fd);
	while (env)
	{
		clean = ft_strtrim(env, "\n");
		if (!clean)
			return (sc_error(12), clean_env(&mat, --i), NULL);
		mat[++i] = clean;
		free(env);
		env = get_next_line(fd);
	}
	mat[++i] = NULL;
	return (close(fd), mat);
}

// TODO extraer la funcion de contar la líneas de un archivo
int	get_file_env(int fd, t_data **data)
{
	int		i;
	char	*env;

	env = get_next_line(fd);
	i = 0;
	if (!env || (*data)->env)
		return (0);
	i = count_lines("/tmp/env.env");
	fd = open("/tmp/env.env", O_RDONLY, 777);
	(*data)->env = (char **)malloc(sizeof(char **) * ++i);
	if (!i && is_valid_file("/tmp/env.env", fd, "R"))
		return (sc_error(SC_PERMISSION_DENIED), 0);
	i = -1;
	if (!(*data)->env)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	env = get_next_line(fd);
	while (env)
	{
		(*data)->env[++i] = env;
		env = get_next_line(fd);
	}
	return (1);
}

int	init_data(t_data **data, char **env)
{
	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	(*data)->args = NULL;
	(*data)->comand = NULL;
	(*data)->next = NULL;
	(*data)->path = NULL;
	(*data)->redir = NULL;
	(*data)->env = NULL;
	(*data)->heredoc = 0;
	(*data)->is_ex = 0;
	(*data)->pipe = 0;
	if (!create_env(data, env))
		return (0);
	return (1);
}

int	manage_token(char *input, t_token **token, int i)
{
	if (!*token)
	{
		if (!new_token(input[i], typeing(input[i], " |><\'\"") \
					, token, 0))
			return (is_error(&(t_error){"Memory error", 1}) \
					, free_token(token), sc_error(12), 0);
	}
	else
	{
		if (!add_token(input[i], typeing(input[i], " |><\'\"") \
					, token))
			return (is_error(&(t_error){"Memory error", 1}) \
					, free_token(token), sc_error(12), 0);
	}
	return (1);
}

t_data	*lexer(char *input, t_data **data, char **env)
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
		if (!manage_token(input, &token, i))
			return (NULL);
	}
	check_closed(token, &error);
	check_gramathic(token, &error, -1, 0);
	if (error.is_error)
		return (is_error(&(t_error){"Syntax error", 1}), free(error.error), \
				free_data(data), free_token(&token), sc_error(1), NULL);
	else if (!split_token(token, env, data))
		return (is_error(&(t_error){"Memory error", 1}), \
				free_data(data), free_token(&token), sc_error(12), NULL);
	free_token(&token);
	return (*data);
}
