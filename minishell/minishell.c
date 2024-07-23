/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/07/24 00:39:03 by kevin            ###   ########.fr       */
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

int	save_env(t_data *data)
{
	int		fd;
	int		i;
	char	**env;

	i = 0;
	if (!data->env && !data->env[0])
		return (sc_error(EXIT_FAILURE), g_stat_code);
	unlink("/tmp/env.env");
	fd = open("/tmp/env.env", O_WRONLY | O_CREAT | O_APPEND, 777);
	// printf("en save env el fd es: %i\n", fd);
	if (fd < 0)
		return (sc_error(SC_FILE_DESCRIPTOR_IN_BAD_STATE), g_stat_code);
	env = data->env;
	while (env[i])
	{
		ft_putstr_fd(env[i], fd);
		ft_putstr_fd("\n", fd);
		// printf("entorno: %s\n", env[i]);
		i++;
	}
	close(fd);
	return (0);
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

int	index_env_env(char **env, char *str)
{
	int		i;

	i = -1;
	if (!env)
		return (-2);
	while (env[++i])
	{
		if (ft_strnstr(env[i], str, ft_strlen(str)) && \
				(env[i][ft_strlen(str)] \
				== '=' || env[i][ft_strlen(str)] == '\0'))
			return (i);
	}
	return (-1);
}

int	set_env(char *key, char *val, char ***env)
{
	int		i;
	char	*res;
	char	*str;

	str = ft_strjoin(key, "=");
	if (!str)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	res = ft_strjoin(str, val);
	if (!res)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	i = index_env_env(*env, key);
	if (i >= 0)
	{
		free((*env)[i]);
		(*env)[i] = res;
	}
	else if (i == -1)
	{
		*env = ft_matadd(env, res);
		if (*env)
			return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	}
	else
		return (sc_error(SC_RESOURCE_TEMPORARILY_UNAVAILABLE), 0);
	return (free(str), 1);
}

char	**create_env_first(char **cenv)
{
	int		i;
	char	**env;

	i = 0;
	if (!cenv || !cenv[0])
		return (NULL);
	while (cenv[i])
		i++;
	env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!env)
		return(sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	i = -1;
	while (cenv[++i])
	{
		env[i] = ft_strdup(cenv[i]);
		if (!(env)[i])
			return (clean_env(&env, --i), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	}
	env[i] = NULL;
	return (env);
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

int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		*data;
	int			fd;
	char		**mat;
	char		*key;

	(void)argc;
	(void)argv;
	data = NULL;
	mat = NULL;
	setup_signal_handlers();
	mat = create_env_first(env);
	// TODO si da error que hay que hacer desde aqui?
	if (!mat)
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
	else
	{
		key = ft_strdup("SHLVL");
		if (!key)
			sc_error(SC_CANNOT_ALLOCATE_MEMORY);
		key = key_to_res(&key, mat);
		if (!key)
			sc_error(SC_CANNOT_ALLOCATE_MEMORY);
		fd = ft_atoi(key) + 1;
		free(key);
		key = ft_itoa(fd);
		if (!key)
			sc_error(SC_CANNOT_ALLOCATE_MEMORY);
		if (!set_env("SHLVL", key, &mat))
			sc_error(SC_CANNOT_ALLOCATE_MEMORY);
		free (key);
	}
	while (1)
	{
		if (data)
		{
			free_data(&data);
			data = NULL;
		}
		if (input)
		{
			free (input);
			input = NULL;
		}
		fd = open("/tmp/env.env", O_RDONLY);
		if (is_valid_file("/tmp/env.env", fd, "R"))
			sc_error(SC_PERMISSION_DENIED);
		if (!mat && fd >= 0)
		{
			mat = get_env_file(fd);
			close(fd);
			// printf("env.env ha sido eliminado\n");
			unlink("/tmp/env.env");
		}
		input = readline(BLUE"Minishell: "BLACK);
		if (input == NULL) {
            printf("\nexit\n");
            break; // Salir del bucle si se presionó Ctrl + D (EOF)
        }
		// printf("6\n");
		if (input && *input)
			add_history (input);
		// printf("7\n");
		if (mat)
		{
			// printf("Entro a lexer con MAT\n");
			data = lexer(input, &data, mat);
		}
		else
		{
			// printf("Entro a lexer con ENV\n");
			data = lexer(input, &data, env);
		}
			// printf("El p de data es: %p\n", data);
		// printf("7\n");
		if (data)
			check_pwd(data);
		// printf("8\n");
		if (data && data->next)
		{
			execute_pipeline(&data);
			if (g_stat_code == SC_HEREDOC)
				sc_error(1);
		}
		else if (data)
		{
			is_valid_command(data, 0);
			if (g_stat_code == SC_HEREDOC)
				sc_error(1);
		}
		if (data && data->comand && !strcmp(data->comand, "exit"))
		{
			break;
		}
		printf("10\n");
		if (data && !file_exist("/tmp/env.env"))
		{
			// printf("en main: guardando env\n");
			if (save_env(data))
				perror("Error saving envoirment\n");
			if (mat)
				clean_env(&mat, -1);
		}
		printf("11\n");
		if (mat)
			clean_env(&mat, -1);
		printf("12\n");
		if (data)
			free_data(&data);
		data = NULL;
	}
	if (mat)
		clean_env(&mat, -1);
	free_data(&data);
	free(input);
	return (g_stat_code);
}

/*int es_comando_valido(char *comando) {
    // Aquí podrías implementar una lógica para verificar si el comando es válido
    // Por ejemplo, verificar si el archivo ejecutable existe en el sistema
    if (access(comando, X_OK) != -1) {
        return 1;
    }
    return 0;
}
int main(int argc, char *argv[], char *env[])
{
    static char *input;

    while(1)
    {
        input = readline("\x1b[34m""Minishell:""\x1b[0m");
        if(!strcmp(input, "exit"))
            break ;
        //printf("%s\n",input);
 		if (!es_comando_valido(input)) {
            printf("Comando inválido: %s\n", input);
  //          return 1;
        }
//		if (execve(input, (char **){"touch", "test.txt", NULL}, env) == -1) {
  //      perror("Error al ejecutar el comando");
    //    return 1;
//    }		
    }
}*/
