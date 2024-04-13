/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/04/13 16:40:45 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redir(t_data *data);

void execute_command(t_data *data, char *command_path)
{
	pid_t pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		//char *args[] = {command,  "-la", "libft", NULL};
		if (data->redir != NULL)
			handle_redir(data);
		if (execve(command_path, data->args, NULL) == -1)
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

void	handle_redir(t_data *data)
{
	int fd;
	char	buf[1024];
	ssize_t	bytes_read;

	if (data->redir == NULL)
		printf("redir es null\n");
	if (data->redir != NULL)
	{
		if (data->redir->type == MAJOR)
		{
			fd = open(data->redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else if (data->redir->type == D_MAJOR)
		{
			fd = open(data->redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else if (data->redir->type == MINOR)
		{
			fd = open(data->redir->path, O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				exit(EXIT_FAILURE);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(fd);
		}
		else if (data->redir->type == D_MINOR)
		{
			while ((bytes_read = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
			{
				if (write(STDIN_FILENO, buf, bytes_read) == -1)
				{
					perror("write");
					exit(EXIT_FAILURE);
				}
				if (!ft_strncmp(buf, data->redir->path, ft_strlen(data->redir->path)))
					break ;
			}
			if (bytes_read == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
		}
	}
}

int is_valid_command(t_data *data)
{
	int		i;
	char	*path;
	char	*comand_path;
	//char	**token;

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
		comand_path = ft_strjoin(token[i], "/");
		comand_path = ft_strjoin(comand_path, data->comand);
		printf("%s/%s\n", token[i], data->comand);
		if (access(comand_path, X_OK) == 0)
		{
			execute_command(data, comand_path);
			printf("El comando \"%s\" es válido en la ruta: %s\n", data->comand, comand_path);
			free(comand_path);
			return 1;
		}
		i++;
	}
	free(comand_path);
	return 0;
}
