/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:52:55 by adrian            #+#    #+#             */
/*   Updated: 2024/08/24 12:39:35 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	b_echo(t_data *data)
{
	int	i;
	int	fd;

	i = 1;
	if (data->args[i] && !ft_is_n(data->args, &i))
	{
		fd = open("/tmp/echoafjnaifsnk", O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (is_valid_file("/tmp/echoafjnaifsnk", fd, "FW", &data))
			(close(fd), unlink("/tmp/echoafjnaifsnk"), exit(*data->stat_code));
		print_args_echo(data, &i, fd);
		(ft_putstr_fd(BLUE"Minishell: "BLACK, fd), close(fd));
	}
	else if (data->args[i])
	{
		print_args_echo(data, &i, 1);
		printf("\n");
	}
	else
		printf("\n");
	sc_error(SC_SUCCESS, &data);
	free_args(&data->args);
	exit(*data->stat_code);
}

int	ft_is_n(char **str, int *j)
{
	int	i;
	int	res;

	i = 0;
	res = -1;
	while (str[*j])
	{
		if (res == -1 && str[*j][i] != '-')
			return (1);
		else if (res == 0 && str[*j][i] != '-')
			return (res);
		while (str[*j][++i])
		{
			if (res == -1 && str[*j][i] != 'n')
				return (1);
			else if (res == 0 && str[*j][i] != 'n')
				return (1);
			res = 0;
		}
		*j = *j + 1;
		i = 0;
	}
	return (res);
}

void	print_args_echo(t_data *data, int *i, int fd)
{
	while (data->args[*i])
	{
		ft_putstr_fd(data->args[*i], fd);
		if (data->args[++*i])
			ft_putstr_fd(" ", fd);
	}
}
