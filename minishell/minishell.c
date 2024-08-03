/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/03 14:19:44 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_stat_code;

char	**ft_matcpy(char **mat)
{
	int		size;
	char	**new_mat;
	int		i;
	char	**c_mat;

	i = -1;
	size = ft_matsize(mat);
	new_mat = (char **)malloc(sizeof(char **) * (size + 1));
	if (!new_mat)
		return (NULL);
	c_mat = mat;
	while (c_mat[++i])
	{
		new_mat[i] = ft_strdup(c_mat[i]);
	}
	new_mat[i] = NULL;
	return (new_mat);
}

int	file_exist(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	check_pwd(t_data *data)
{
	char	*key;

	key = ft_strdup("PWD");
	if (!key)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), g_stat_code);
	key = key_to_res(&key, data->env);
	if (!key)
		return (g_stat_code);
	chdir(key);
	free(key);
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
void	handle_env_file(t_data **data)
{
	if (*data && !file_exist("/tmp/env.env"))
	{
		if (save_env(*data))
			perror("Error saving environment\n");
	}
}
void	handle_input(char *env[], t_data **data, char ***mat, char *input)
{
	if (*mat)
		*data = lexer(input, data, *mat);
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
		print_data(*data);
		is_valid_command(*data, 0);
		if (g_stat_code == SC_HEREDOC)
			sc_error(1);
	}
}
int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		*data;
	char		**mat;

	(void)argc;
	(void)argv;
	data = NULL;
	mat = NULL;
	ft_set_shell(env, &mat);
	while (1)
	{
		wait_signal(1);
		ft_handle_env_file(&mat);
		input = get_input();
		if (input == NULL)
		{
			printf("\nexit\n");
			break ;
		}
		if (input && *input)
			add_history(input);
		handle_input(env, &data, &mat, input);
		if (data && data->comand && !strcmp(data->comand, "exit"))
		{
			if (g_stat_code != 1)
				break ;
		}
		handle_env_file(&data);
		ft_free_resources(&data, &input, &mat);
	}
	ft_free_resources(&data, &input, &mat);
	return (g_stat_code);
}
