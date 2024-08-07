/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/07 18:13:53 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_stat_code;

int	file_exist(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	ft_is_exit(t_data *d)
{
	if (d && d->comand && !ft_strcmp(d->comand, "exit") && !d->pipe)
	{
		if (g_stat_code != 235)
			return (1);
		else
			(ft_putstr_fd("exit\n", 2), sc_error(1));
	}
	return (0);
}



int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		*d;
	char		**mat;

	mat = NULL;
	if (argc != 1)
		return (ft_putstr_fd("No arguments accepted\n", 2), sc_error(1), 1);
	ft_set_shell(env, &mat, &d);
	while (1)
	{
		(wait_signal(1), ft_handle_env_file(&mat), (void)argv);
		input = get_input();
		if (input == NULL)
		{
			printf("\nexit\n");
			break ;
		}
		if (input && *input)
			add_history(input);
		handle_input(env, &d, &mat, input);
		if (ft_is_exit(d) == 1)
			break;
		(handle_env_file(&d), ft_free_resources(&d, &input, &mat));
	}
	return (ft_free_resources(&d, &input, &mat), g_stat_code);
}
