/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/07/07 17:37:56 by kevin            ###   ########.fr       */
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
		return (0);
	fd = open("/tmp/env.env", O_WRONLY | O_CREAT | O_APPEND, 777);
	if (fd < 0)
		return (0);
	env = data->env;
	while (env[i])
	{
		ft_putstr_fd(env[i], fd);
		ft_putstr_fd("\n", fd);
		// printf("entorno: %s\n", env[i]);
		i++;
	}
	close(fd);
	return (1);
}

int	file_exist(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY, 777);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

/*TODO por algun motivo al poner adios el history falla*/
int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		*data;
	int			fd;
	char		**mat;
	char		*key;

	g_stat_code = 0;
	(void)argc;
	(void)argv;
	data = NULL;
	setup_signal_handlers();
	while (1)
	{
		if (data)
			data = NULL;
		if (input)
		{
			free (input);
			input = (char *) NULL;
		}
		fd = open("/tmp/env.env", O_RDONLY, 777);
		mat = NULL;
		if (fd >= 0)
		{
			mat = get_env_file(fd);
			unlink("/tmp/env.env");
		}
		input = readline(BLUE"Minishell: "BLACK);
		if (input == NULL) {
            printf("\n");
            break; // Salir del bucle si se presionó Ctrl + D (EOF)
        }
		if (!strcmp(input, "exit")) //TODO Exit ha de permitir 1 arg y solo 1 ademas solo permite 
									//numeros y hay que hacer modulo de 256 para que no se exceda.
									//si el 1º argumento es erróneo "ejemplo letras", ha de salir y 
									//obviar el resto de args, no ocurre igual si solo pasas 2 parametros validos
			break ;
		if (input && *input)
			add_history (input);
		if (mat)
			data = lexer(input, &data, mat);
		else
			data = lexer(input, &data, env);
		if (data)
		{
			key = ft_strdup("PWD");
			key = key_to_res(&key, data->env);
			chdir(key);
			free(key);
		}
		if (data && data->next)
			execute_pipeline(data);
		else if (data)
			is_valid_command(data);
		if (data && !file_exist("/tmp/env.env"))
		{
			if (!save_env(data))
				return (1);
			if (mat)
				clean_env(&mat, -1);
		}
		free_data(&data);
		data = NULL;
	}
	free_data(&data);
	free(input);
	return (0);
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
