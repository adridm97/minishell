/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+    */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/06/25 23:28:35 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_command(t_data **data, int heredoc_processed)
{
	char	*path;

	path = ft_strdup("PATH");
	path = key_to_res(&path, (*data)->env);
	if ((!path || !(*data)->comand) && !is_builtin((*data)->comand))
	{
		handle_missing_command(*data, heredoc_processed);
		free(path);
		return (0);
	}
	if (is_builtin((*data)->comand))
	{
		execute_command(data, "is_builtinOMG", heredoc_processed);
		free(path);
		return (1);
	}
	return (search_and_execute_command(data, path, heredoc_processed));
}

/*
 Abre el archivo, revisa permisos (hay que indicarlo en mayuscula) 
 y cambia g_status si lo necesita, retorna 0 si está OK
 F = existe? retorna 1 cuando no.
 R = se puede leer? retorna 2 cuando no.
 W = se puede escribir? retorna 3 cuando no.
 X = se puede ejecutar? retorna 4 cuando no.
 si fd es -1 retorna 5.
*/
int	is_valid_file(char *filename, int fd, char *check, t_data **data)
{
	if (fd < 0)
		return (sc_error(EXIT_FAILURE, data), 5);
	if (ft_strchr(check, 'F') && access(filename, F_OK))
		return (close(fd), perror("El archivo no existe"), \
		sc_error(SC_KEY_HAS_EXPIRED, data), 1);
	if (ft_strchr(check, 'R') && access(filename, R_OK))
		return (close(fd), perror("El archivo no tiene permisos de lectura"), \
		sc_error(EXIT_FAILURE, data), 2);
	if (ft_strchr(check, 'W') && access(filename, W_OK))
		return (close(fd), perror("El archivo no tiene permisos de escritura"), \
		sc_error(EXIT_FAILURE, data), 3);
	if (ft_strchr(check, 'X') && access(filename, X_OK))
		return (close(fd), perror("El archivo no tiene permisos de ejecución"), \
		sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE, data), 4);
	return (0);
}

void	execute_command(t_data **ddata, char *command_path, int processed)
{
	pid_t	pid;
	int		status;

	wait_signal(0);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
		handle_child_process(ddata, command_path, processed);
	else
	{
		pid = wait(&status);
		while (pid > 0)
		{
			if (WIFEXITED(status))
			{
				*(*ddata)->stat_code = WEXITSTATUS(status);
				printf("3Terminado por señal %d\n", *(*ddata)->stat_code);
			}
			else if (WIFSIGNALED(status))
			{
				*(*ddata)->stat_code = WTERMSIG(status) + 128;
				printf("4Terminado por señal %d\n", *(*ddata)->stat_code);
			}
			pid = wait(&status);
		}
	}
}
