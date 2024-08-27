/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:32:19 by adrian            #+#    #+#             */
/*   Updated: 2024/08/26 16:42:55 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	switch_builtin(t_data **ddata)
{
	t_data	*data;

	data = *ddata;
	if (!ft_strcmp(data->comand, "echo"))
		b_echo(data);
	else if (!ft_strcmp(data->comand, "cd"))
		(b_cd(ddata, getenv("HOME")));
	else if (!ft_strcmp(data->comand, "pwd"))
		b_pwd(*ddata);
	else if (!ft_strcmp(data->comand, "export"))
		b_export(ddata);
	else if (!ft_strcmp(data->comand, "unset"))
		b_unset(data);
	else if (!ft_strcmp(data->comand, "env"))
		b_env(data);
	else if (!ft_strcmp(data->comand, "exit"))
		b_exit(ddata);
	return ;
}

int	is_builtin(char *comand)
{
	if (!comand)
		return (0);
	if (!ft_strcmp(comand, "echo") || !ft_strcmp(comand, "cd") \
			|| !ft_strcmp(comand, "pwd") \
			|| !ft_strcmp(comand, "export") || !ft_strcmp(comand, "unset") \
			|| !ft_strcmp(comand, "env") \
			|| !ft_strcmp(comand, "exit"))
		return (1);
	return (0);
}
