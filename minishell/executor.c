/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+    */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/06/02 18:57:00 by kevin            ###   ########.fr       */
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

int heredoc(t_data *data) {
    int fd;
    char *line;
    char *filename = "/tmp/heredoc";
    t_redir *aux = data->redir;

    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {
        line = readline("> ");
        if (line == NULL || ft_strcmp(line, aux->path) == 0) {
            free(line);
            break;
        }
        ft_putstr_fd(line, fd);
        ft_putstr_fd("\n", fd);
        free(line);
    }
    close(fd);
    return open(filename, O_RDONLY);
}


void handle_redir(t_data *data)
{
    int 	fd;
    t_redir *redir = data->redir;

    while (redir != NULL)
    {
        if (redir->type == MAJOR)
        {
            fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        else if (redir->type == D_MAJOR)
        {
            fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else if (redir->type == MINOR)
        {
            fd = open(redir->path, O_RDONLY);
        }
        else
        {
            redir = redir->next;
            continue;
        }

        if (fd == -1)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if ((redir->type == MAJOR || redir->type == D_MAJOR) && dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        else if (redir->type == MINOR && dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);
        redir = redir->next;
    }
}

void b_cd(t_data *data)
{
    if (data->args[1] == NULL || chdir(data->args[1]) != 0)
    {
        perror("cd");
    }
}

void b_echo(t_data *data)
{
    int i = 1;
    int newline = 1;

    if (data->args[1] && ft_strcmp(data->args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    while (data->args[i])
    {
        printf("%s", data->args[i]);
        if (data->args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}

void b_pwd()
{
    char *buff = getcwd(NULL, 0);
    if (buff)
    {
        printf("%s\n", buff);
        free(buff);
    }
    else
    {
        perror("getcwd");
    }
}

void print_env(t_data *data)
{
    for (int i = 0; data->env[i]; i++)
    {
        printf("declare -x %s\n", data->env[i]);
    }
}

int ft_matsize(char **mat)
{
    int size = 0;
    while (mat[size])
        size++;
    return size;
}

char **ft_matadd(char ***mat, char *str)
{
    int size = ft_matsize(*mat);
    char **new_mat = malloc(sizeof(char*) * (size + 2));
    if (!new_mat)
        return NULL;

    for (int i = 0; i < size; i++)
    {
        new_mat[i] = ft_strdup((*mat)[i]);
    }
    new_mat[size] = ft_strdup(str);
    new_mat[size + 1] = NULL;
    free_args(*mat);
    return new_mat;
}

void b_export(t_data *data)
{
    if (!data->args[1])
    {
        print_env(data);
    }
    else
    {
        data->env = ft_matadd(&data->env, data->args[1]);
        print_env(data);
    }
}

void switch_builtin(t_data *data)
{
    if (ft_strcmp(data->comand, "echo") == 0)
        b_echo(data);
    else if (ft_strcmp(data->comand, "cd") == 0)
        b_cd(data);
    else if (ft_strcmp(data->comand, "pwd") == 0)
        b_pwd();
    else if (ft_strcmp(data->comand, "export") == 0)
        b_export(data);
    else if (ft_strcmp(data->comand, "unset") == 0)
        b_cd(data); // cambiar a la implementación de unset
    else if (ft_strcmp(data->comand, "env") == 0)
        print_env(data);
    else if (ft_strcmp(data->comand, "exit") == 0)
        exit(EXIT_SUCCESS);
}

int is_builtin(char *comand)
{
    if (!comand)
        return 0;
    if (ft_strcmp(comand, "echo") == 0 || ft_strcmp(comand, "cd") == 0 || ft_strcmp(comand, "pwd") == 0 ||
        ft_strcmp(comand, "export") == 0 || ft_strcmp(comand, "unset") == 0 || ft_strcmp(comand, "env") == 0 ||
        ft_strcmp(comand, "exit") == 0)
        return 1;
    return 0;
}

void execute_command(t_data *data, char *command_path) {
    pid_t pid;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Proceso hijo
        if ((data->redir != NULL && data->redir->type == D_MINOR) && data->next == NULL) {
            int heredoc_fd = heredoc(data);
            if (dup2(heredoc_fd, STDIN_FILENO) == -1) {
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(heredoc_fd);
        }

        if (data->redir != NULL) {
            handle_redir(data);
        }

        if (ft_strcmp(command_path, "is_builtinOMG") == 0) {
            switch_builtin(data);
            exit(EXIT_SUCCESS);
        } else {
            if (execve(command_path, data->args, data->env) == -1) {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        // Proceso padre
        int exit_code;
        if (waitpid(pid, &exit_code, 0) == -1) {
            perror("waitpid");
            return;
        }

        if (exit_code != 0) {
            fprintf(stderr, "Proceso hijo terminó con error: %d\n", exit_code);
        }
    }
}

void execute_pipeline(t_data *data) {
    t_data *current = data;
    int input_fd = STDIN_FILENO;
    int fd[2];
    pid_t pid;
    int heredoc_fd = -1;

    while (current != NULL) {
        // Manejo del heredoc antes del fork
        if (current->redir != NULL && current->redir->type == D_MINOR) {
            heredoc_fd = heredoc(current);
        }
        // Creación del pipe si no es el último comando
        if (current->next != NULL) {
            if (pipe(fd) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
        pid = fork();
        //execve("/bin/cat", (char *[]){"cat", "/tmp/heredoc", NULL}, NULL);
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
            if (heredoc_fd != -1) {
                if (dup2(heredoc_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(heredoc_fd);
            } else if (input_fd != STDIN_FILENO) {
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(input_fd);
            }
            if (current->next != NULL) {
                close(fd[0]);
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(fd[1]);
            }
            if (current->redir != NULL) {
                handle_redir(current);
            }

            if (!is_valid_command(current)) {
                fprintf(stderr, "Comando no encontrado: %s\n", current->comand);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        } else {
            // Proceso padre
            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }
            if (current->next != NULL) {
                close(fd[1]);
                input_fd = fd[0];
            }
            if (heredoc_fd != -1) {
                close(heredoc_fd);
                heredoc_fd = -1;
            }
            current = current->next;
        }
    }

    // Esperar a que todos los procesos hijos terminen
    while (wait(NULL) > 0);
}

int is_valid_command(t_data *data)
{
    char *path;
    int  i;
    char *comand_path;
    char **token;

    i = 0;
    path = getenv("PATH");
    if (!path || !data->comand)
    {
        fprintf(stderr, "No se pudo obtener el valor de PATH\n");
        return 0;
    }

    if (is_builtin(data->comand))
    {
        execute_command(data, "is_builtinOMG");
        return 1;
    }
    
    token = ft_split(path, ':');
    while(token[i] != NULL)
    {
        comand_path = ft_strjoin(token[i], "/");
        comand_path = ft_strjoin(comand_path, data->comand);

        if (access(comand_path, X_OK) == 0)
        {
            execute_command(data, comand_path);
            //printf("El comando \"%s\" es válido en la ruta: %s\n", data->comand, comand_path);
            free(comand_path);
            free_args(token);
            return 1;
        }
        free(comand_path);
        i++;
    }
    free_args(token);
    return 0;
}


