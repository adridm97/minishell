/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/06/24 21:58:20 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **ft_matcpy(char **mat)
{
    int size;
    char **new_mat;
    int i;
    char **c_mat;

    i = -1;
    size = ft_matsize(mat);
    new_mat = (char**)malloc(sizeof(char**) * (size + 1));
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

int save_env(t_data *data)
{
	int	fd;
	int i;
	char **env;

	i = 0;
	if (!data->env && !data->env[0])
		return (0);
	fd = open("/tmp/env.env", O_WRONLY | O_CREAT | O_APPEND, 777);
	// printf("EL PUTO FD ES:%i\n",fd);
	if (fd < 0)
		return (0);
	// printf("en save env, env[%i] = %s\n", i, data->env[i]);
	env = data->env;
	// printf("----------env TIENE:---------\n");
	// 	 i = -1;
	// 		while(env[++i])
	// 			printf("mat %i = %s\n", i, env[i]);
	// i = 0;
	while(env[i])
	{
		ft_putstr_fd(env[i],fd);
		ft_putstr_fd("\n",fd);
		i++;
		// printf("peto aqui\n");
		// printf("en save env, env[%i] = %s\n", i, env[i]);
		// if (data->env[i])
		// 	ft_putstr_fd("\n",fd);
	}
	// printf("he escrito en el fichero\n");
	close(fd);
	return (1);
}

int file_exist(char *file)
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

	(void)argc;
	(void)argv;
	data = NULL;
	while (1)
	{
		if (data)
			data = NULL;
		if (input)
		{
			free (input);
			input = (char *) NULL;
		}
		fd = open("/tmp/env.env",O_RDONLY,777);
		mat = NULL;
		if (fd >= 0)
		{
			mat = get_env_file(fd);
			unlink("/tmp/env.env");
			// printf("----------MAT TIENE:---------\n");
			// int i = -1;
			// while(mat[++i])
			// 	printf("mat %i = %s\n", i, mat[i]);
		}
		input = readline(BLUE"Minishell: "BLACK);
		if (!strcmp(input, "exit"))
			break ;
		if (input && *input)
			add_history (input);
		if (mat)
		{
			data = lexer(input, data, mat);
			// printf("PETO AQUI POR LERDO:\n");
			// print_env(data, "Esto es una kk:");

		}
		else
			data = lexer(input, data, env);
		//if (!data)
		// 	exit(EXIT_FAILURE);	
		is_valid_command(data, &data);
		// printf("%s\n", data.token->value);
		if (!file_exist("/tmp/env.env"))
		{
			if (!save_env(data))
				return (1);
		}
		free_data(&data);
		data = NULL;
	}
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
