/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/03/31 17:33:50 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_valid_command(char *command)
{
	int		i;
	char	*res;
	char	*path;
	char	*command_path;
   
	path = getenv("PATH");
	i = 0;
	if (path == NULL)
	{
		fprintf(stderr, "No se pudo obtener el valor de PATH\n");
		exit(EXIT_FAILURE);
	}
	char **token = ft_split(path, ":");
	while (token[i] != NULL)
	{
		command_path = ft_strjoin(path, "/");
		command_path = ft_strjoin(command_path, command);
		printf("%s/%s\n", token, command);
		if (access(token, X_OK) == 0)
		{
			printf("El comando \"%s\" es válido en la ruta: %s\n", command, token);
			free(path_copy);
			return 1;
		}
	}
	free(path_copy);
	return 0;
}
