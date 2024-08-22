/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:33:06 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/22 20:12:36 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
	t_data	*tmp;

	tmp = *data;
	if (*data)
	{
		while(tmp->next)
			tmp = tmp->next;
		*sce = *tmp->stat_code;
		free_data(data);
		*data = NULL;
	}
*/
void	ft_free_resources(t_data **data, char **input, char ***mat,  int *sce)
{
	if (*data)
	{
		*sce = *(*data)->stat_code;
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

	g_sigint_received = 0;
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

void	handle_input(t_data **data, char ***mat, char *input, int *sce)
{
	if (!*input)
		return ;
	if (*mat)
		*data = lexer(input, data, *mat, sce);
	if (*data)
		check_pwd(*data);
	if (*data && (*data)->next)
	{
		execute_pipeline(data);
		if (*(*data)->stat_code == SC_HEREDOC)
			sc_error(1, data);
	}
	else if (*data)
	{
		is_valid_command(data, 0);
		if (*(*data)->stat_code == SC_HEREDOC)
			sc_error(1, data);
	}
}

int	handle_status_code(char **str, char *key, t_data **data)
{
	char	*res;
	char	*status_code;

	status_code = ft_itoa(*(*data)->stat_code);
	if (!*str)
		res = ft_strjoin("", status_code);
	else
		res = ft_strjoin(*str, status_code);
	(free(*str), free(key));
	*str = res;
	return (1);
}
