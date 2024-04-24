/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/04/21 03:15:49 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_redir(t_data *data);

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	*str1;

	i = 0;
	str = (unsigned char *)s1;
	str1 = (unsigned char *)s2;
	while ((str[i] != '\0' || str1[i] != '\0'))
	{
		if (str[i] != str1[i])
			return (str[i] - str1[i]);
		i++;
	}
	return (0);
}


static void	sig_handler_def(int signum)
{
	if (signum == CTRL_C)
	{
		printf("\n");
		//rl_replace_line("", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	sig_handler_childs(int signum)
{
	if (signum == CTRL_C)
		printf("\n");
}

int	init_signals(int mode)
{
	if (mode == DEFAULT)
		signal(CTRL_C, sig_handler_def);
	else if (mode == CHILDS)
		signal(CTRL_C, sig_handler_childs);
	signal(CTRL_SLASH, sig_handler_def);
	return (1);
}

void	create_heredoc(char *delimiter, char *path)
{
	char	*str;
	int		fd;

	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	str = readline("> ");
	while (str && ft_strcmp(delimiter, str) != 0)
	{
		str = ft_strjoin(str, "\n");
		ft_putstr_fd(str, fd);
		free(str);
		str = readline("> ");
	}
	close(fd);
	free(str);
	init_signals(DEFAULT);
}

void	do_heredoc(int i, t_redir *aux)
{
	char	*num;
	char	*path;

	num = ft_itoa(i);
	path = ft_strjoin("/tmp/minishell", num);
	free(num);
	create_heredoc(aux->path, path);
	free(aux->path);
	aux->path = ft_strdup(path);
	free(path);
}

void	heredoc(t_data *data)
{
	t_redir *aux;
	int		i;

	i = 0;
	while (data)
	{
		aux = data->redir;
		while (aux)
		{
			if (aux->type == D_MINOR)
			{
				do_heredoc(i, aux);
				i++;
			}
			aux = aux->next;
		}
		data = data->next;
	}
	//return (0);
}

void	handle_redir(t_data *data)
{
	int fd;
	t_redir *redir = data->redir;

	while (redir != NULL)
	{
		if (redir->type == MAJOR && redir->next == NULL)
		{
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
		}
		else if (redir->type == D_MAJOR)
		{
			fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
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
		}
		else if (redir->type == MINOR)
		{
			fd = open(redir->path, O_RDONLY);
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
		}
		else if (redir->type == D_MINOR)
		{
			heredoc(data);
		}
		redir = redir->next;
		close(fd);
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
