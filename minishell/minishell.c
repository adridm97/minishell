/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/05/04 12:40:28 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*TODO por algun motivo al poner adios el history falla*/
int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		*data;

	(void)env;
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
		input = readline(BLUE"Minishell: "BLACK);
		if (!strcmp(input, "exit"))
			break ;
		if (input && *input)
			add_history (input);
		//TODO si hay un error de sintaxis data es null y se acaba el programa, hay que gestionarlo.
		data = lexer(input, data, env);
		if (!data)
			exit(EXIT_FAILURE);
		// is_valid_command(data);
		// printf("%s\n", data.token->value);
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
