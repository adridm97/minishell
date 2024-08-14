/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/15 00:06:36 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_lines(char *file, t_data **data)
{
	int		fd;
	int		i;
	char	*line;

	fd = open(file, O_RDONLY, 777);
	if (is_valid_file(file, fd, "R", data))
		return (sc_error(SC_PERMISSION_DENIED, data), 0);
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

int	init_data(t_data **data, char **env, int *sce)
{
	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	(*data)->args = NULL;
	(*data)->comand = NULL;
	(*data)->next = NULL;
	(*data)->path = NULL;
	(*data)->redir = NULL;
	(*data)->env = NULL;
	(*data)->heredoc = 0;
	(*data)->is_ex = 0;
	(*data)->pipe = 0;
	(*data)->stat_code = sce;
	if (!create_env(data, env))
		return (0);
	return (1);
}

static t_token	*manage_token(char *input, t_token **token, int i, int *sce)
{
	if (!*token)
	{
		if (!new_token(input[i], typeing(input[i], " |><\'\"") \
					, token, 0))
		{
			*sce = 12;
			return (is_error(&(t_error){"Memory error", 1}) \
					, free_token(token), NULL);
		}
	}
	else
	{
		if (!add_token(input[i], typeing(input[i], " |><\'\"") \
					, token))
		{
			*sce = 12;
			return (is_error(&(t_error){"Memory error", 1}) \
					, free_token(token), NULL);
		}
	}
	return (*token);
}

t_data	*lexer(char *input, t_data **data, char **env, int  *sce)
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
		if (!manage_token(input, &token, i, sce))
			return (NULL);
	}
	check_closed(token, &error);
	check_gramathic(token, &error, -1, 0);
	if (error.is_error)
		return (is_error(&(t_error){"Syntax error", 1}), free(error.error), \
				save_env_mat(env, sce), free_data(data), free_token(&token), sc_error_int(12, sce), NULL);
	else if (!split_token(token, env, data, sce))
		return (is_error(&(t_error){"Memory error", 1}), \
				save_env_mat(env, sce), free_data(data), free_token(&token), sc_error_int(12, sce), NULL);
	free_token(&token);
	return (*data);
}
