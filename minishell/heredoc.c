/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:26:22 by adrian            #+#    #+#             */
/*   Updated: 2024/08/25 18:43:47 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	eof_check(char *line, t_redir *aux)
{
	if ((ft_strcmp(aux->path, "\"\"") == 0 && line[0] == '\0'))
		return (1);
	else if (aux->path[0] == '"' && aux->path[ft_strlen(aux->path) - 1] == '"')
	{
		if (ft_strncmp(line + 1, aux->path, ft_strlen(line) - 1) == 0)
			return (1);
	}
	else if ((aux->path[0] == '\''
			&& aux->path[ft_strlen(aux->path) - 1] == '\''))
	{
		if (ft_strncmp(line + 1, aux->path, ft_strlen(line) - 1) == 0)
			return (1);
	}
	else if (ft_strcmp(line, aux->path) == 0)
		return (1);
	return (0);
}

int	heredoc(t_redir	*aux, t_data *data)
{
	int		fd;
	char	*line;
	char	*expanded_line;

	fd = open("/tmp/heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (is_valid_file("/tmp/heredoc", fd, "FRW", &data))
		exit(*data->stat_code);
	if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
		(perror("Error al configurar el manejador de SIGINT"), exit(1));
	while (1)
	{
		if (g_sigint_received == 1)
			(close(fd), exit(SC_HEREDOC));
		line = readline("> ");
		if (line == NULL || eof_check(line, aux))
		{
			free(line);
			line = NULL;
			break ;
		}
		if (expand_line(&expanded_line, &line, fd, data))
			exit(*data->stat_code);
	}
	return (close(fd), open("/tmp/heredoc", O_RDONLY));
}

int	expand_line(char **expanded_line, char **line, int fd, t_data *data)
{
	if (ft_strchr(data->redir->path, '"' )
		|| ft_strchr(data->redir->path, '\''))
		*expanded_line = ft_strdup(*line);
	else
		*expanded_line = heredoc_tokenizer(*line, data);
	if (!*expanded_line)
	{
		close(fd);
		free(*line);
		return (1);
	}
	ft_putstr_fd(*expanded_line, fd);
	if (((*expanded_line)[0] != '\0'))
		ft_putstr_fd("\n", fd);
	free(*line);
	free(*expanded_line);
	return (0);
}

char	*heredoc_tokenizer(char *str, t_data *data)
{
	t_token	*token;
	int		i;
	char	*input;
	char	*res;

	if (str == NULL || str[0] == '\0')
		return (ft_strdup("\n"));
	res = NULL;
	input = ft_strdup(str);
	if (!input)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY, &data), NULL);
	i = -1;
	token = NULL;
	while (input[++i])
	{
		if (!manage_token_heredoc(&token, &input, &i, &data))
			return (NULL);
	}
	free(input);
	while (token)
	{
		if (!token_to_str(&token, &res, &data))
			return (free_token(&token), sc_error(12, &data), NULL);
	}
	return (free_token(&token), res);
}

int	manage_token_heredoc(t_token **token, char **input, int *i, t_data **data)
{
	if (!(*token))
	{
		if (!new_token((*input)[*i], typeing((*input)[*i], " |><\'\"") \
					, token, 0))
			return (is_error(& (t_error){"Memory error", 1}), free((*input)) \
					, free_token(token), \
					sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	}
	else
	{
		if (!add_token((*input)[*i], typeing((*input)[*i], " |><\'\"") \
					, token))
			return (is_error(&(t_error){"Memory error", 1}), \
			free((*input)) \
			, free_token(token), sc_error(SC_CANNOT_ALLOCATE_MEMORY, data), 0);
	}
	return (1);
}
