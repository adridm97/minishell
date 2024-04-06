/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/04/06 12:25:59 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void execute_command(char *command, char *command_path)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		char *args[] = {command, "-la", "libft", NULL};
		if (execve(command_path, args, NULL) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int	status;
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
	}
}

int is_valid_command(char *command)
{
	int		i;
	char	*path;
	char	*command_path;
   
	path = getenv("PATH");
	i = 0;
	if (path == NULL)
	{
		fprintf(stderr, "No se pudo obtener el valor de PATH\n");
		exit(EXIT_FAILURE);
	}
	char **token = ft_split(path, ':');
	while (token[i] != NULL)
	{
		command_path = ft_strjoin(token[i], "/");
		command_path = ft_strjoin(command_path, command);
		printf("%s/%s\n", token[i], command);
		if (access(command_path, X_OK) == 0)
		{
			execute_command(command, command_path);
			printf("El comando \"%s\" es válido en la ruta: %s\n", command, command_path);
			free(command_path);
			return 1;
		}
		i++;
	}
	free(command_path);
	return 0;
}
