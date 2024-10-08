/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:26:22 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 11:36:16 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(t_redir	*aux, t_data *data)
{
	int		fd;
	char	*line;
	char	*expanded_line;

	fd = open("/tmp/heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (is_valid_file("/tmp/heredoc", fd, "FRW"))
		exit(g_stat_code);
	if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
		(perror("Error al configurar el manejador de SIGINT"), \
		exit(EXIT_FAILURE));
	while (1)
	{
		if (g_stat_code == SC_HEREDOC)
			(close(fd), exit(SC_HEREDOC));
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, aux->path) == 0)
		{
			free(line);
			break ;
		}
		if (expand_line(&expanded_line, &line, fd, data))
			exit(g_stat_code);
	}
	return (close(fd), open("/tmp/heredoc", O_RDONLY));
}

int	expand_line(char **expanded_line, char **line, int fd, t_data *data)
{
	*expanded_line = heredoc_tokenizer(*line, data);
	if (!*expanded_line)
	{
		close(fd);
		free(*line);
		return (1);
	}
	ft_putstr_fd(*expanded_line, fd);
	if (!(ft_strlen(*expanded_line) == 1 && (*expanded_line)[0] != '\0'))
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
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	i = -1;
	token = NULL;
	while (input[++i])
	{
		if (!manage_token_heredoc(&token, &input, &i))
			return (NULL);
	}
	free(input);
	while (token)
	{
		if (!token_to_str(&token, &res, &data))
			return (free_token(&token), sc_error(12), NULL);
	}
	return (free_token(&token), res);
}

int	manage_token_heredoc(t_token **token, char **input, int *i)
{
	if (!(*token))
	{
		if (!new_token((*input)[*i], typeing((*input)[*i], " |><\'\"") \
					, token, 0))
			return (is_error(& (t_error){"Memory error", 1}), free((*input)) \
					, free_token(token), \
					sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	}
	else
	{
		if (!add_token((*input)[*i], typeing((*input)[*i], " |><\'\"") \
					, token))
			return (is_error(&(t_error){"Memory error", 1}), \
			free((*input)) \
			, free_token(token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	}
	return (1);
}

int	token_to_str(t_token **token, char **res, t_data **data)
{
	if (is_special((*token)->value, "$") && \
	(!is_special((*token)->next->value, "\"'")))
	{
		if (!is_expandsor(token, res, (*data)->env))
			return (free_token(token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	}
	else
	{
		if ((*token))
		{
			*res = new_str(res, (*token)->value);
			(*token) = (*token)->next;
		}
	}
	return (1);
}
