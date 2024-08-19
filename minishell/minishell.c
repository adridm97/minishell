/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/19 07:57:07 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if (*d->stat_code != 235)
			return (1);
		else if (!(count_args(d->args) > 2))
			(ft_putstr_fd("exit\n", 2), sc_error(1, &d));
	}
	return (0);
}

int	ft_isspace(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (*str != ' ')
			return (0);
		str++;
	}
	return (1);
}

//ls -l > a < a < a > b > b < a | ls -la < a << eof > c >> c < | ls
int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		*d;
	char		**mat;
	int 		sce;

	sce = 0;
	mat = NULL;
	if (argc != 1)
		return (ft_putstr_fd("No arguments accepted\n", 2), 1);
	ft_set_shell(env, &mat, &d);
	while (1)
	{
		(wait_signal(1), ft_handle_env_file(&mat, &d), (void)argv);
		input = get_input();
		if (input == NULL)
		{
			printf("\nexit\n");
			break ;
		}
		if ((input && *input) && !ft_isspace(input))
			add_history(input);
		else if (input)
		{
			free(input);
			continue ;
		}
		handle_input(&d, &mat, input, &sce);
		if (ft_is_exit(d) == 1)
			break ;
		(handle_env_file(&d), ft_free_resources(&d, &input, &mat, &sce));
	}
	return (ft_free_resources(&d, &input, &mat, &sce), sce);
}
