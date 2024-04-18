/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/04/16 21:38:10 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redir(t_data *data);

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		i++;
	return (str1[i] - str2[i]);
}

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, size);
		free (ptr);
	}
	return (new_ptr);
}

void	check_buffer_size(char **input_buffer, char *line)
{
	size_t	current_len;
	size_t	new_len;

	current_len = 0;
	new_len = 0;
	if (*input_buffer == NULL)
		*input_buffer = ft_strdup(line);
	else
	{
		current_len = ft_strlen(*input_buffer);
		new_len = current_len + ft_strlen(line) + 2;
		*input_buffer = ft_realloc(*input_buffer, new_len);
		ft_strlcat(*input_buffer, "\n", new_len);
		ft_strlcat(*input_buffer, line, new_len);
	}
}

static void	handle_siginthc(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		exit(EXIT_SUCCESS);
	}
	else if (sig == SIGQUIT)
	{
		printf("\b\b  \b\b");
		fflush(stdout);
	}
}

void	here_doc(t_data *data)
{
	char	*line;
	char	*input_buffer;

	signal(SIGINT, handle_siginthc);
	signal(SIGQUIT, handle_siginthc);
	printf("> ");
	if (!data->redir->path)
		return (printf("Error: no delimiter\n"), (void)(0));
	line = "";
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("");
		if (!line)
			exit(1);
		if (ft_strcmp(line, data->redir->path) == 0)
		{
			free(line);
			break ;
		}
		check_buffer_size(&input_buffer, line);
		free(line);
		printf("> ");
	}
}

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
			here_doc(data);
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
