/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:50:35 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 18:13:48 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	b_exit(t_data *data)
{
	int	arg_count;

	arg_count = count_args(data->args);
	if (data->args[1])
		check_numeric_argument(data->args[1]);
	if (arg_count > 2)
	{
		sc_error(235);
		if (!data->pipe)
			ft_putstr_fd("exit\n", 2);
		ft_putstr_fd("exit: too many arguments\n", 2);
		exit(g_stat_code);
	}
	if (data->args[1])
	{
		sc_error(ft_atoi(data->args[1]));
		g_stat_code = ft_atoi(data->args[1]);
		if (!data->pipe)
			printf("exit\n");
		exit(g_stat_code);
	}
	if (!data->pipe)
		printf("exit\n");
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

void	check_numeric_argument(char *arg)
{
	int	j;

	j = 0;
	if (ft_strlen(arg) >= ft_strlen("9223372036854775807"))
	{
		try_max_num(arg);
	}
	while (arg[j])
	{
		if (!ft_isdigit(arg[j]))
		{
			sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY);
			printf("exit\n");
			ft_putstr_fd("numeric argument required\n", 2);
			exit(g_stat_code);
		}
		j++;
	}
}

void	try_max_num(char *arg)
{
	if (ft_strlen(arg) > ft_strlen("9223372036854775807"))
	{
		sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY);
		printf("exit\n");
		ft_putstr_fd("numeric argument required\n", 2);
		exit(g_stat_code);
	}
	if (ft_strcmp(arg, "9223372036854775807") > 0)
	{
		sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY);
		printf("exit\n");
		ft_putstr_fd("numeric argument required\n", 2);
		exit(g_stat_code);
	}
}
