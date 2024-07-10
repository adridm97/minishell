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

void	handle_redir(t_data *data);

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

// size_t	calculate_expanded_length(const char *line, char **env)
// {
// 	size_t	i;
// 	size_t	len;
// 	size_t	j;
// 	size_t	var_len;
// 	char	*var_name;
// 	char	*var_value;

// 	i = 0;
// 	len = 0;
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '$')
// 		{
// 			j = i + 1;
// 			while (line[j] != '\0' && (ft_isalnum(line[j]) || line[j] == '_'))
// 			j++;
// 			var_len = j - (i + 1);
// 			var_name = ft_strndup(line + i + 1, var_len);
// 			var_value = key_to_res(&var_name, env);
// 			if (var_value)
// 			{
// 				len += ft_strlen(var_value);
// 				free(var_value);
// 			}
// 			else
// 				len += j - i;
// 			i = j;
// 		}
// 		else
// 		{
// 			len++;
// 			i++;
// 		}
// 	}
// 	return (len);
// }

// char	*expand_variables(const char *line, char **env)
// {
// 	size_t	result_len;
// 	char	*result;
// 	size_t	i;
// 	size_t	k;
// 	size_t	j;
// 	size_t	var_len;
// 	char	*var_name;
// 	char	*var_value;

// 	i = 0;
// 	j = 0;
// 	k = 0;
//  	result_len = calculate_expanded_length(line, env);
// 	result = malloc(result_len + 1);
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '$')
// 		{
// 			j = i + 1;
// 			while (line[j] != '\0' && (ft_isalnum(line[j]) || line[j] == '_'))
// 				j++;
// 			var_len = j - (i + 1);
// 			var_name = ft_strndup(line + i + 1, var_len);
// 			var_value = key_to_res(&var_name, env);
// 			if (var_value)
// 			{
// 				ft_strcpy(result + k, var_value);
// 				k += ft_strlen(var_value);
// 				free(var_value);
// 			}
// 			else
// 			{
// 				result[k++] = '$';
// 				ft_strncpy(result + k, line + i + 1, var_len);
// 				k += var_len;
// 			}
// 			i = j;
// 		}
// 		else
// 		{
// 			result[k++] = line[i++];
// 		}
// 	}
// 	result[k] = '\0';
// 	return result;
// }

// char	*ft_get_key(char *line)
// {
// 	int		i;
// 	int		start;
// 	char	*res;

// 	i = 0;
// 	while (line[i] && line[i] != '$')
// 		i++;
// 	if (!line[i] || line[i] != '$')
// 		return (NULL);
// 	i++;
// 	start = i;
// 	while (line[i] && ft_strchr(" <>|'\".,-+*!¡?¿%%=·@#ªº¬€$", line[i]) == NULL)
// 		i++;
// 	res = malloc((i - start + 1) * sizeof(char));
// 	if (!res)
// 		return (NULL);
// 	ft_strncpy(res, line + start, i - start);
// 	res[i - start] = '\0';
// 	return (res);
// }

// char	*ft_expand_line(char *str, char start, char *value)
// {
// 	int		i;
// 	int		j;
// 	int		k;
// 	int		val_len;
// 	int		str_len;
// 	char	*res;

// 	i = 0;
// 	j = 0;
// 	k = 0;
// 	val_len = ft_strlen(value);
// 	str_len = ft_strlen(str);
// 	res = malloc(str_len + val_len + 1);
// 	if (!res)
// 		return (NULL);
// 	while (str[i])
// 	{
// 		if (str[i] == start)
// 		{
// 			i++;
// 			while (str[i] && ft_strchr(" <>|'\".,-+*!¡?¿%%=·@#ªº¬€", str[i]) == NULL)
// 				i++;
// 			while (value[k])
// 				res[j++] = value[k++];
// 		}
// 		else
// 			res[j++] = str[i++];
// 	}
// 	res[j] = '\0';
// 	return (res);
// }

/*int	switch_case_heredoc(t_token **token, char **env, t_data **data, char **str)
{
	if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else if ((*token)->value == '"')
		is_double_string(token, env, str);
	else if ((*token)->value == '$')
		is_expandsor(token, str, env);
	return (1);
}*/

void sc_error(int sce)
{
	g_stat_code = sce;
}

char	*heredoc_tokenizer(char *str, t_data *data)
{
	t_token	*token;
	//t_token	*c_token;
	int		i;
	char	*input;
	char	*res;

	//c_token = token;
	res = NULL;
	input = ft_strdup(str);
	if (!input)
		return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	i = -1;
	token = NULL;
	while (input[++i])
	{
		if (!token)
		{
			if (!new_token(input[i], typeing(input[i], " |><\'\"") \
						, &token, 0))
				return (is_error(& (t_error){"Memory error",1}) \
						, free_token(&token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
		else
		{
			if (!add_token(input[i], typeing(input[i], " |><\'\"") \
						, &token))
				return (is_error(& (t_error){"Memory error",1}) \
						, free_token(&token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
	}
	while (token)
	{
		if (is_special(token->value, "$") && (!is_special(token->next->value, "\"'")))
		{
			if (!is_expandsor(&token, &res, data->env))
				return(sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
		else
		{
			if (token)
			{
				res = new_str(&res, token->value);
				token = token->next;
			}
		}	
	}
	return (res);
}

int	heredoc(t_data *data) 
{
	int		fd;
	char	*line;
	char	*filename;
	char	*expanded_line;
	t_redir	*aux;

	aux = data->redir;
	filename = "/tmp/heredoc";
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, aux->path) == 0)
		{
			free(line);
			break;
		}
		expanded_line = heredoc_tokenizer(line, data);
		ft_putstr_fd(expanded_line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	return open(filename, O_RDONLY);
}


void	handle_redir(t_data *data)
{
	int 	fd;
	t_redir *redir;
	
	redir = data->redir;

	while (redir != NULL)
	{
		if (redir->type == MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == D_MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == MINOR)
			fd = open(redir->path, O_RDONLY);
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

void	b_cd(t_data *data)
{
	int	i;
	char	last_pwd[1024];
	char	pwd[1024];
	char	*res;

	if (getcwd(last_pwd,sizeof(last_pwd)))
	{
		i = index_env(data, "OLDPWD");
		// Se debe guardar al confirmar el cd
		if(i >= 0)
			data->env[i] = ft_strjoin("OLDPWD=",last_pwd);
		else
		{
			res = ft_strjoin("OLDPWD=", last_pwd);
			if (!res)
				sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
			(data)->env = ft_matadd(&(data)->env, res);
			if (!(data)->env)
				exit (g_stat_code);
			// creo que no he de liberarlo
			// free(last_pwd);
			free(res);
		}
	}
	else
		exit(EXIT_FAILURE);
	if (data->args[1] && !chdir(data->args[1]))
	{
		// printf("PETO, 1\n");	
		i = index_env(data, "PWD");
		if(i < 0)
		{
			res = ft_strjoin("PWD=", "PWD");
			if (!res)
				sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
			(data)->env = ft_matadd(&(data)->env, res);
			if (!(data)->env)
				free(res), exit(g_stat_code);
			// creo que no he de liberarlo
			// free(last_pwd);
			free(res);
			i = index_env(data, "PWD");
		}
		// printf("PETO, 2\n");	
		// printf("PETO, 3\n");
		//es posible que aquí pierda memoria memory leak
		if (getcwd(pwd,sizeof(pwd)))
		{
		data->env[i] = ft_strjoin("PWD=",pwd);
		}
		else
			exit(EXIT_FAILURE);
		if(!data->env[i])
		{
			// printf("PETO, no guardo en data env\n");	
			exit(EXIT_FAILURE);
		}
		if (!save_env(data))
		{
			// printf("PETO, no guardo archivo en save env\n");	
			exit(EXIT_FAILURE);
		}
		// printf("data env: %s\n", data->env[i]);
	}
	else
	{
		// printf("PETO Y NO CAMBIO\n");	
    	exit(EXIT_SUCCESS);
	}
		free(data->env[i]);
    exit(EXIT_SUCCESS);
}

void	b_echo(t_data *data)
{
	int	i;

	i = 1;
	if (data->args[1] && ft_strcmp(data->args[1], "-n"))
	{
		while (data->args[i])
		{
			data->args[i] = heredoc_tokenizer(data->args[i],data);
			if (!data->args[i])
				exit(g_stat_code);
			printf("%s", data->args[i]);
			if (data->args[++i])
			printf(" ");
			}
			printf("\n");
    }
    else if (data->args[1])
    {
        i++;
        while (data->args[i])
        {
			data->args[i] = heredoc_tokenizer(data->args[i],data);
			if (!data->args[i])
				exit(g_stat_code);
            printf("%s", data->args[i]);
            if (data->args[++i])
                printf(" ");
        }
		printf("\n");
    }
	else
		printf("\n");
	g_stat_code = SC_SUCCESS;
    exit(g_stat_code);
}

void	b_pwd(void)
{
	size_t	size;
	char	*buff;

	size = 1;
	buff = NULL;
	while (!buff)
		buff = getcwd(buff, size++);
	printf("%s\n", buff);
	free(buff);
	exit(EXIT_SUCCESS);
}

void	print_env(t_data *data, char *str)
{
	int	i;

	i = -1;
	if (data->env)
	{
		while (data->env[++i])
			printf("%s%s\n", str, data->env[i]);
	}
}

int	ft_matsize(char **mat)
{
	int	size;

	size = 0;
	while (mat[size])
		size++;
	return (size);
}

char	**ft_matadd(char ***mat, char *str)
{
	int		size;
	char	**new_mat;
	int		i;
	char	**c_mat;

	i = -1;
	size = ft_matsize(*mat);
	new_mat = (char **)malloc(sizeof(char **) * (size + 2));
	if (!new_mat)
		return (free_args(*mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	c_mat = *mat;
	while (c_mat[++i])
	{
		new_mat[i] = ft_strdup(c_mat[i]);
		if (!new_mat[i])
		{
			new_mat[i] = NULL;
			return (free_args(new_mat), free_args(*mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
	}
	new_mat[i] = ft_strdup(str);
	if (!new_mat[i])
		{
			new_mat[i] = NULL;
			return (free_args(new_mat), free_args(*mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
	new_mat[++i] = NULL;
	free_args(*mat);
	return (new_mat);
}

// hay que ponerle, no cambia las variables, no llega al data.env correctamente tras pasarlo a una funcion.
void	b_export(t_data **data)
{
	int		i;
	char	*key;
	char	*need;
	t_data	*cdata;
	
	if (!(*data)->args[1])
		return(print_env(*data, "declare -x "));
	cdata = *data;
	need = ft_strnstr((cdata)->args[1],"=", ft_strlen((cdata)->args[1]));
	if(need)
	{
		key = (char*)malloc((need - (cdata)->args[1]) +1);
		ft_strlcpy(key, (cdata)->args[1], ((need - (cdata)->args[1]) + 1));
	}
	else
		key = ft_strdup((cdata)->args[1]);
	// printf("la i es %s\n", key);
	if (index_env(cdata, key) >= 0)
	{
		i = index_env(cdata, key);
		// printf("la i es %i\n", i);
		(cdata)->env[i] = (cdata)->args[1];
	}
	else
	{
		// printf("NO TENGO QUE ENTRAR AQUI\n");
		(cdata)->env = ft_matadd(&(cdata)->env, (cdata)->args[1]);
		if (!(cdata)->env)
			exit (g_stat_code);
	}
	// printf("ENTRO AQUI y es: %s\n", (*data)->env[i]);
	unlink("/tmp/env.env");
	if (!save_env(cdata))
		exit(g_stat_code);
	exit(EXIT_SUCCESS);
}

char	**ft_mat_rem_index(char ***mat, int index)
{
	char	**new_mat;
	int		i;
	int		j;
	char	**c_mat;

	i = -1;
	j = -1;
	new_mat = (char **)malloc(sizeof(char **) * (ft_matsize(*mat)));
	if (!new_mat)
		return (NULL);
	c_mat = *mat;
	while (c_mat[++i])
	{
		if (c_mat[i] && i != index)
			new_mat[++j] = ft_strdup(c_mat[i]);
	}
	new_mat[++j] = NULL;
	free_args(*mat);
	return (new_mat);
}

int	index_env(t_data *data, char *str)
{
	int		i;
	char	**env;

	i = -1;
	env = data->env;
	while (env[++i])
	{
		if (ft_strnstr(env[i], str, ft_strlen(str)) && \
				(env[i][ft_strlen(str)] \
				== '=' || env[i][ft_strlen(str)] == '\0'))
			return (i);
	}
	return (-1);
}

void	b_unset(t_data *data)
{
	int	i;

	i = index_env(data, data->args[1]);
	// printf("%i\n", i);
	if (i != -1)
		data->env = ft_mat_rem_index(&data->env, i);
	unlink("/tmp/env.env");
	if (!save_env(data))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

void	b_env(t_data *data)
{
	int	i;

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

void	switch_builtin(t_data **ddata)
{
	t_data	*data;

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
	return ;
}

/*int execute_command(t_data **ddata, char *command_path)
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
}*/
int	is_builtin(char *comand)
{
	if (!comand)
		return (0);
	if (!ft_strcmp(comand, "echo") || !ft_strcmp(comand, "cd") \
			|| !ft_strcmp(comand, "pwd") \
			|| !ft_strcmp(comand, "export") || !ft_strcmp(comand, "unset") \
			|| !ft_strcmp(comand, "env") \
			|| !ft_strcmp(comand, "exit"))
		return (1);
	return (0);
}

void	execute_command(t_data **ddata, char *command_path)
{
	pid_t	pid;
	t_data	*data;
	int		heredoc_fd;
	int		exit_code;

	pid = fork();
	data = *ddata;
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		if ((data->redir != NULL && data->redir->type == D_MINOR) \
				&& data->next == NULL)
		{
			heredoc_fd = heredoc(data);
			if (dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			close(heredoc_fd);
		}
		if (data->redir != NULL)
			handle_redir(data);
		if (ft_strcmp(command_path, "is_builtinOMG") == 0)
		{
			switch_builtin(ddata);
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (execve(command_path, data->args, data->env) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		if (waitpid(pid, &exit_code, 0) == -1)
		{
			perror("waitpid");
			return ;
		}
		if (exit_code != 0)
			printf("Proceso hijo terminó con error: %d\n", exit_code);
	}
}

void	execute_pipeline(t_data *data)
{
	t_data	*current;
	int		input_fd;
	int		fd[2];
	pid_t	pid;
	int		heredoc_fd;

	heredoc_fd = -1;
	input_fd = STDIN_FILENO;
	current = data;
	while (current != NULL)
	{
		if (current->redir != NULL && current->redir->type == D_MINOR)
			heredoc_fd = heredoc(current);
		if (current->next != NULL)
		{
			if (pipe(fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (heredoc_fd != -1)
			{
				if (dup2(heredoc_fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				close(heredoc_fd);
			}
			else if (input_fd != STDIN_FILENO)
			{
				if (dup2(input_fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				close(input_fd);
			}
			if (current->next != NULL)
			{
				close(fd[0]);
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				close(fd[1]);
			}
			if (current->redir != NULL)
				handle_redir(current);
			if (!is_valid_command(current))
			{
				printf("Comando no encontrado: %s\n", current->comand);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (input_fd != STDIN_FILENO)
				close(input_fd);
			if (current->next != NULL)
			{
				close(fd[1]);
				input_fd = fd[0];
			}
			if (heredoc_fd != -1)
			{
				close(heredoc_fd);
				heredoc_fd = -1;
			}
			current = current->next;
		}
	}
	while (wait(NULL) > 0)
		;
}

int	is_valid_command(t_data *data)
{
	char	*path;
	int		i;
	char	*comand_path;
	char	**token;
	char	*tmp;

	i = 0;
	path = ft_strdup("PATH");
	path = key_to_res(&path, data->env);
	if (!path || !data->comand)
	{
		if (data->redir != NULL && data->redir->type == D_MINOR)
		{
			heredoc(data);
			return (0);
		}
		printf("No se pudo obtener el valor de PATH\n");
		free(path);
		return (0);
	}
	if (is_builtin(data->comand))
	{
		execute_command(&data, "is_builtinOMG");
		free(path);
		return (1);
	}
	token = ft_split(path, ':');
	free(path);
	if (access(data->comand, X_OK) == 0)
	{
		execute_command(&data, data->comand);
		free_args(token);
		return (1);
	}
	while (token[i] != NULL)
	{
		tmp = ft_strjoin(token[i], "/");
		comand_path = ft_strjoin(tmp, data->comand);
		free(tmp);
		if (access(comand_path, X_OK) == 0)
		{
			execute_command(&data, comand_path);
			free(comand_path);
			free_args(token);
			return (1);
		}
		free(comand_path);
		i++;
	}
	free_args(token);
	return (0);
}
