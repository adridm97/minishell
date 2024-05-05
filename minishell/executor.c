/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/05/05 20:41:14 by kevin            ###   ########.fr       */
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

void heredoc(t_data *data)
{
    int     fd;
    int     i;
    char    *line;
    char    *filename; 
    t_redir *aux;
    t_data  *iterator;

    i = 0;
    iterator = data;
    while (iterator)
    {
        aux = iterator->redir;
        if (aux && aux->path)
        {
            if (aux->type == D_MINOR)
            {
                filename = "/tmp/heredoc";
                fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                line = readline("> ");
                while (line)
                {
                    
                    if (ft_strcmp(line, aux->path) == 0)
                    {
                        free(line);
                        break;
                    }
                    ft_putstr_fd(line, fd);
                    ft_putstr_fd("\n", fd);
                    free(line);
                    line = readline("> ");
                }
                close(fd);
                int arg_count = 0;
                while (data->args[arg_count] != NULL)
                    arg_count++;
                if(arg_count < 2)
                {
                    // Crear un nuevo arreglo de punteros con espacio adicional
                    char **new_args = malloc((i + 2) * sizeof(char *));
                    if (new_args == NULL)
                    {
                        perror("malloc");
                        exit(EXIT_FAILURE);
                    }
                    // Copiar los elementos del arreglo original al nuevo arreglo
                    i = 0;
                    while (iterator->args[i] != NULL)
                    {
                        new_args[i] = iterator->args[i];
                        i++;
                    }
                    // Agregar el nombre del archivo al final del nuevo arreglo
                    new_args[i] = filename;
                    new_args[i + 1] = NULL;  // Asegurarse de que el nuevo arreglo esté terminado con NULL
                    // Liberar el arreglo original
                    free(iterator->args);
                    // Actualizar el puntero args para apuntar al nuevo arreglo
                    iterator->args = new_args;
                }
                // Salir si no hay más delimitadores
                if (aux->next == NULL)
                {
                    break;
                }
            }
            if(aux->next != NULL)
            {
                iterator->redir = aux->next;
            }
        }
        if(iterator->next != NULL)
        {
            iterator = iterator->next;
        }
        if (line == NULL)
            break;
    }
}

/*void	child_process(t_data *data, int *fd, char *comand_path)
{
	int	filein;
    t_redir *aux;

    aux = data->redir;
	filein = open(aux->path, O_RDONLY, 0644);
    if (filein == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    dup2(fd[1], STDOUT_FILENO);
    dup2(filein, STDIN_FILENO);
    close(fd[0]);
    execute_command(data,comand_path);
}


void	pipex(t_data *data, char *comand_path)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("pid");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		child_process(data, fd, comand_path);
    waitpid(pid, NULL, 0);
    parent_process(data, fd, comand_path);
}*/

void handle_redir(t_data *data)
{
    int 	fd;
    t_redir *redir;

    redir = data->redir;

    while (redir != NULL)
    {
        if (redir->type == MAJOR)
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
			close(fd);
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
			close(fd);
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
			close(fd);
        }
        redir = redir->next;
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
        if (data->redir != NULL && data->redir->type == D_MINOR)
			heredoc(data);
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
			/*if (data->redir != NULL && data->redir->type == PIPE)
			    pipex(data, comand_path);*/
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
