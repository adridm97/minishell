/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:33:06 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/07 17:17:44 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_resources(t_data **data, char **input, char ***mat)
{
	if (*data)
	{
		free_data(data);
		*data = NULL;
	}
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	if (*mat)
		clean_env(mat, -1);
}

int	is_special_string(char *c, char *comp)
{
	char	*str;

	while (*c)
	{
		str = ft_strchr(comp, *c);
		if (str)
			return (1);
		c++;
	}
	return (0);
}

char	*get_input(void)
{
	int		fd;
	char	*prompt;
	char	*input;

	if (!access("/tmp/echoafjnaifsnk", F_OK))
	{
		fd = open("/tmp/echoafjnaifsnk", O_RDONLY);
		prompt = get_next_line(fd);
		close(fd);
		unlink("/tmp/echoafjnaifsnk");
		input = readline(prompt);
	}
	else
		input = readline(BLUE"Minishell: "BLACK);
	return (input);
}

void	handle_input(char *env[], t_data **data, char ***mat, char *input)
{
	if (!*input)
		return ;
	if (*mat)
	{
		*data = lexer(input, data, *mat);
	}
	else
		*data = lexer(input, data, env);
	if (*data)
		check_pwd(*data);
	if (*data && (*data)->next)
	{
		execute_pipeline(data);
		if (g_stat_code == SC_HEREDOC)
			sc_error(1);
	}
	else if (*data)
	{
		is_valid_command(*data, 0);
		if (g_stat_code == SC_HEREDOC)
			sc_error(1);
	}
}

int	handle_status_code(char **str, char *key)
{
	char	*res;
	char	*status_code;

	status_code = ft_itoa(g_stat_code);
	if (!*str)
		res = ft_strjoin("", status_code);
	else
		res = ft_strjoin(*str, status_code);
	(free(*str), free(key));
	*str = res;
	return (1);
}
