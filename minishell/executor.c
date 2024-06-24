/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/06/24 21:49:19 by kevin            ###   ########.fr       */
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

void    b_cd(t_data *data)
{
    (void)data;
    printf("hago cosas\n");
}

//TODO falta que espanda
void    b_echo(t_data *data)
{
    int i;

    i = 1;
    if (ft_strcmp(data->args[1], "-n"))
    {
        while (data->args[i])
        {

            printf("%s", data->args[i]);
            if (data->args[++i])
                printf(" ");
        }
        printf("\n");
    }
    else
    {
        i++;
        while (data->args[i])
        {

            printf("%s", data->args[i]);
            if (data->args[++i])
                printf(" ");
        }
    }
    exit(EXIT_SUCCESS);
}

void    b_pwd()
{
    size_t  size;
    char    *buff;

    size = 1;
    buff = NULL;
    while(!buff)
        buff = getcwd(buff, size++);
    printf("%s\n", buff);
    free(buff);
    exit(EXIT_SUCCESS);
}

void    print_env(t_data *data, char *str)
{
    int i;

    i = -1;
    if (data->env)
    {
        while (data->env[++i])
            printf("%s%s\n", str, data->env[i]);
    }
}

int ft_matsize(char **mat)
{
    int size;

    size = 0;
    while (mat[size])
        size++;
    return (size);
}

char **ft_matadd(char ***mat, char *str)
{
    int size;
    char **new_mat;
    int i;
    char **c_mat;

    i = -1;
    size = ft_matsize(*mat);
    new_mat = (char**)malloc(sizeof(char**) * (size + 2));
    if (!new_mat)
        return (NULL);
    c_mat = *mat;
    while (c_mat[++i])
    {
        new_mat[i] = ft_strdup(c_mat[i]);
    }
    new_mat[i] = ft_strdup(str);
    new_mat[++i] = NULL;
    free_args(*mat);
    return (new_mat);
}

// cada nuevo comando env se reinicializa
void    b_export(t_data **data)
{
    if (!(*data)->args[1])
        print_env(*data, "declare -x ");
    else
        (*data)->env = ft_matadd(&(*data)->env, (*data)->args[1]);
    unlink("/tmp/env.env");
    if (!save_env(*data))
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}


char **ft_mat_rem_index(char ***mat, int index)
{
    char **new_mat;
    int i;
    int j;
    char **c_mat;

    i = -1;
    j = -1;
    new_mat = (char**)malloc(sizeof(char**) * (ft_matsize(*mat)));
    if (!new_mat)
        return (NULL);
    c_mat = *mat;
    while (c_mat[++i])
    {
        if (i == index)
        {
            // printf("ENCUENTRO EL MALO = %s\n", c_mat[i]);
        }
        else if (c_mat[i])
        {
            // printf("PETO AQUI\n");
            // printf("He entrado aqui y es %s\n", c_mat[i]);
            new_mat[++j] = ft_strdup(c_mat[i]);
        }
    }
    new_mat[++j] = NULL;
    free_args(*mat);
    return (new_mat);
}

int    index_env(t_data *data, char *str)
{
    int     i;
    char    **env;

    i = -1;
    env = data->env;
    while (env[++i])
    {
        if (ft_strnstr(env[i], str, ft_strlen(str)) && (env[i][ft_strlen(str)] == '=' || env[i][ft_strlen(str)] == '\0'))
            return (i);
    }
    return (-1);
}

void    b_unset(t_data *data)
{
    int i;

    i = index_env(data, data->args[1]);
    printf("%i\n", i);
    if (i != -1)
        data->env = ft_mat_rem_index(&data->env, i);
    unlink("/tmp/env.env");
    if (!save_env(data))
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
    
}

void    b_env(t_data *data)
{
    int i;

    i = -1;
    if (data->env)
    {
        while (data->env[++i])
        {
            if (ft_strrchr(data->env[i], '='))
                printf("%s\n", data->env[i]);
        }
    }
    exit(EXIT_SUCCESS);
}

void    switch_builtin(t_data **ddata)
{
    t_data *data;

    data = *ddata;
    if (!ft_strcmp(data->comand, "echo"))
        b_echo(data);
    else if (!ft_strcmp(data->comand, "cd"))
        b_cd(data);
    else if (!ft_strcmp(data->comand, "pwd"))
        b_pwd();
    else if (!ft_strcmp(data->comand, "export"))
        b_export(ddata);
    else if (!ft_strcmp(data->comand, "unset"))
        b_unset(data);
    else if (!ft_strcmp(data->comand, "env"))
        b_env(data);
    else if (!ft_strcmp(data->comand, "exit"))
        b_cd(data);
    printf("llego???\n");
    // exit(EXIT_SUCCESS);
    return ;
}

int execute_command(t_data **ddata, char *command_path)
{
	pid_t pid = fork();
    t_data *data;

    data = *ddata;
	if (pid == -1)
	{
		perror("fork");
        return(0);
	}
	else if (pid == 0)
	{
        if (data->redir != NULL && data->redir->type == D_MINOR)
			heredoc(data);
	    if (data->redir != NULL)
		    handle_redir(data);
        if (!ft_strcmp(command_path,"is_builtinOMG"))
            switch_builtin(ddata);
		else if (execve(command_path, data->args, NULL) == -1)
		{
			perror("execve");
			return(0);
		}
	}
	else
	{
		int	status;
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			return(0);
		}
	}
    return (1);
}
int is_builtin(char *comand)
{
    if(!comand)
        return (0);
    if (!ft_strcmp(comand, "echo") || !ft_strcmp(comand, "cd") || !ft_strcmp(comand, "pwd") \
    || !ft_strcmp(comand, "export") || !ft_strcmp(comand, "unset") || !ft_strcmp(comand, "env") \
    || !ft_strcmp(comand, "exit"))
        return (1);
    return (0);
}
int is_valid_command(t_data *data, t_data **ddata)
{
	int		i;
	char	*path;
	char	*comand_path;
	//char	**token;

	// path = getenv("PATH");
    path = ft_strdup("PATH");
    path = key_to_res(&path, data->env);
	i = 0;
	if (path == NULL || !data->comand)
	{
		fprintf(stderr, "No se pudo obtener el valor de PATH\n");
        return (0);
		//exit(EXIT_FAILURE);
	}
        if (is_builtin(data->comand))
            return(execute_command(ddata, "is_builtinOMG"));
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
			execute_command(&data, comand_path);
			printf("El comando \"%s\" es válido en la ruta: %s\n", data->comand, comand_path);
			free(comand_path);
			return 1;
		}
		i++;
	}
	free(comand_path);
	return 0;
}
