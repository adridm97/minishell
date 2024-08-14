/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:17:14 by adrian            #+#    #+#             */
/*   Updated: 2024/08/15 00:13:09 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_command_path(char *dir, char *comand)
{
	char	*tmp;
	char	*comand_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	comand_path = ft_strjoin(tmp, comand);
	free(tmp);
	return (comand_path);
}

int	find_command_in_paths(t_data **data, char **token, int heredoc_processed)
{
	int		i;
	char	*comand_path;

	i = -1;
	while (token[++i] != NULL)
	{
		comand_path = build_command_path(token[i], (*data)->comand);
		if (comand_path == NULL)
			return (0);
		if (access(comand_path, F_OK) == 0)
		{
			if (access(comand_path, X_OK) == 0)
			{
				execute_command(data, comand_path, heredoc_processed);
				(free(comand_path), free_args(&token));
				return (1);
			}
			(sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE, data), exit(126));
		}
		free(comand_path);
	}
	if ((*data)->redir != NULL)
		execute_command(data, (*data)->path, heredoc_processed);
	(free_args(&token), printf("%s: command not found\n", (*data)->comand));
	return (sc_error(SC_KEY_HAS_EXPIRED, data), 0);
}

int	search_and_execute_command(t_data **data, char *path, int heredoc_processed)
{
	char	**token;

	token = ft_split(path, ':');
	free(path);
	if (access((*data)->comand, F_OK) == 0)
	{
		if (access((*data)->comand, X_OK) == 0)
		{
			execute_command(data, (*data)->comand, heredoc_processed);
			free_args(&token);
			return (1);
		}
		else
			sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE, data);
	}
	return (find_command_in_paths(data, token, heredoc_processed));
}
