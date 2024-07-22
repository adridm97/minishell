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
	if (!s1 || !s2)
		return (1);
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
	if (str == NULL || str[0] == '\0')
		return (ft_strdup("\n"));
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

// int	heredoc(t_data *data) 
// {
// 	int		fd;
// 	char	*line;
// 	char	*filename;
// 	char	*expanded_line;
// 	t_redir	*aux;

// 	aux = data->redir;
// 	filename = "/tmp/heredoc";
// 	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		perror("open");
// 		exit(EXIT_FAILURE);
// 	}
// 	while (1)
// 	{
// 		if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
// 		{
// 			perror("Error al configurar el manejador de SIGINT");
// 			exit(EXIT_FAILURE);
// 		}
// 		if (g_stat_code == -1)
// 		{
// 			printf("\n1\n");
// 			close(fd);
// 			close(0);
// 			sc_error(SC_OWNER_DIED), exit(g_stat_code);
// 		}
// 		line = readline("> ");
// 		if (line == NULL || ft_strcmp(line, aux->path) == 0)
// 		{
// 			free(line);
// 			break;
// 		}
// 		expanded_line = heredoc_tokenizer(line, data);
// 		ft_putstr_fd(expanded_line, fd);
// 		ft_putstr_fd("\n", fd);
// 		free(line);
// 		free(expanded_line);
// 	}
// 	close(fd);
// 	return open(filename, O_RDONLY);
// }

/*
 Abre el archivo, revisa permisos (hay que indicarlo en mayuscula) y cambia g_status si lo necesita, retorna 0 si está OK
 F = existe? retorna 1 cuando no.
 R = se puede leer? retorna 2 cuando no.
 W = se puede escribir? retorna 3 cuando no.
 X = se puede ejecutar? retorna 4 cuando no.
 si fd es -1 retorna 5.
*/
int	is_valid_file(char *filename, int fd, char *check)
{
	if (fd < 0)
		return(sc_error(EXIT_FAILURE), 5);
	if (ft_strchr(check, 'F') && access(filename, F_OK))
		return (perror("El archivo no existe"), sc_error(SC_KEY_HAS_EXPIRED), 1);
	if (ft_strchr(check, 'R') && access(filename, R_OK))
		return (perror("El archivo no tiene permisos de lectura"), sc_error(EXIT_FAILURE), 2);
	if (ft_strchr(check, 'W') && access(filename, W_OK))
		return (perror("El archivo no tiene permisos de escritura"), sc_error(EXIT_FAILURE), 3);
	if (ft_strchr(check, 'X') && access(filename, X_OK))
		return (perror("El archivo no tiene permisos de ejecución"), sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE), 4);
	return (0);
}

// TODO gestionar los exit con exit status
int	heredoc(t_data *data) 
{
	int		fd;
	char	*line;
	char	*filename;
	char	*expanded_line;
	t_redir	*aux;

	aux = data->redir;
	filename = "/tmp/heredoc";
	unlink(filename);
	// if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
	// {
	// 	perror("Error al configurar el manejador de SIGINT");
	// 	exit(EXIT_FAILURE);
	// }
	while (1)
	{
		line = readline("> ");
		// if (g_stat_code == 130)
		// {
		// 	free(line);
		// 	unlink(filename);
		// 	exit(130);
		// }
		if (line == NULL || ft_strcmp(line, aux->path) == 0)
		{
			free(line);
			break ;
		}
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (is_valid_file(filename, fd, "FRW"))
			exit(g_stat_code);
		expanded_line = heredoc_tokenizer(line, data);
		if (!expanded_line)
			close(fd), free(line), exit(g_stat_code);
		ft_putstr_fd(expanded_line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		free(expanded_line);
		close(fd);
	}
	printf("hola\n");
	return open(filename, O_RDONLY);
}

// TODO gestionar los exit con exit status
void	handle_redir(t_data *data)
{
	int 	fd;
	t_redir *redir;
	
	redir = data->redir;
	while (redir != NULL)
	{
		if (redir->type == MAJOR)
		{
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
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
		if ((redir->type == MAJOR || redir->type == D_MAJOR) && data->comand != NULL)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}
		else if (redir->type == MINOR && data->comand != NULL)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}

		close(fd);
		redir = redir->next;
	}
}

void	b_cd(t_data *data, char *home)
{
	int		i;
	size_t	size;
	char	*last_pwd;
	char	*pwd;
	char	*res;

	last_pwd = NULL;
	pwd = NULL;
	size = 1;
	if (data->args[1])
		pwd = ft_strdup(data->args[1]);
	while (!last_pwd)
		last_pwd = getcwd(last_pwd, size++);
	if (!pwd)
	{
		res = ft_strdup("HOME");
		if(!res)
			sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
		res = key_to_res(&res, data->env);
		if (!res)
			sc_error(EXIT_FAILURE), perror("HOME no está definido"), exit(g_stat_code);
		pwd = ft_strdup(res);
		free(res);
	}
	else
	{
		if(!ft_strcmp(pwd, "-"))
			{
				res = ft_strdup("OLDPWD");
				if(!res)
					sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
				res = key_to_res(&res, data->env);
				if (!res)
					sc_error(EXIT_FAILURE), perror("OLDPWD no está definido"), exit(g_stat_code);
				pwd = ft_strdup(res);
				free(res);
			}
	}
	if (ft_strrchr(pwd, '~'))
	{
		res = ft_substr(pwd, 1, ft_strlen(pwd) - 1);
		pwd = ft_strjoin(home, res);
	}
	if (!chdir(pwd))
	{
		free(pwd);
		pwd = NULL;
		i = index_env(data, "PWD");
		if(i < 0)
		{
			res = ft_strjoin("PWD=", "PWD");
			if (!res)
				sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
			(data)->env = ft_matadd(&(data)->env, res);
			if (!(data)->env)
				free(res), exit(g_stat_code);
			free(res);
		}
		size = 1;
		while (!pwd)
			pwd = getcwd(pwd, size++);
		i = index_env(data, "PWD");
		data->env[i] = ft_strjoin("PWD=",pwd);
		if(!data->env[i])
		{
			sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
		}
		i = index_env(data, "OLDPWD");
		if(i >= 0)
			data->env[i] = ft_strjoin("OLDPWD=",last_pwd);
		else
		{
			if(i == -2)
				sc_error(SC_RESOURCE_TEMPORARILY_UNAVAILABLE), exit(g_stat_code);
			res = ft_strjoin("OLDPWD=", last_pwd);
			if (!res)
				sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
			(data)->env = ft_matadd(&(data)->env, res);
			if (!(data)->env)
				exit(g_stat_code);
			free(res);
		}
		if(!data->env[i])
		{
			free(data->env[index_env(data, "PWD")]), sc_error(SC_CANNOT_ALLOCATE_MEMORY), exit(g_stat_code);
		}
		unlink("/tmp/env.env");
		if (save_env(data))
		{
			free(data->env[index_env(data, "PWD")]);
			free(data->env[index_env(data, "OLDPWD")]);
			free(last_pwd);
			exit(g_stat_code);
		}
	}
	else
	{
		free(pwd);
		pwd = NULL;
		printf("La ruta especificada no existe\n");	
    	sc_error(EXIT_FAILURE), exit(g_stat_code);
	}
	free(data->env[i]);
    sc_error(SC_SUCCESS), exit(g_stat_code);
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
	// g_stat_code = SC_ADDRESS_ALREADY_IN_USE;
	// printf("el status antes es: %i\n", g_stat_code);
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
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
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

void	print_env_env(char **env, char *str)
{
	int	i;

	i = -1;
	if (env)
	{
		while (env[++i])
			printf("%s%s\n", str, env[i]);
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

//manage g_status_code
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
		(cdata)->env = ft_matadd(&(cdata)->env, (cdata)->args[1]);
		if (!(cdata)->env)
			exit(g_stat_code);
	}
	unlink("/tmp/env.env");
	if (save_env(cdata))
		exit(g_stat_code);
	sc_error(SC_SUCCESS), exit(g_stat_code);
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
// retorna el index de env o -1 si index no existe o -2 si env no existe
int	index_env(t_data *data, char *str)
{
	int		i;
	char	**env;

	i = -1;
	env = data->env;
	if (!env)
		return (-2);
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
	if (save_env(data))
		exit(g_stat_code);
	sc_error(SC_SUCCESS), exit(g_stat_code);
}

void	b_exit(t_data *data)
{
	int	i;

	i = -1;
	while (data->args[i])
		i++;
	if (i > 2)
		sc_error(EXIT_FAILURE), perror("demasiados argumentos\n"), exit(g_stat_code);
	if (data->args[1])
	{
		while(data->args[1][++i])
		{
			if (!ft_isdigit(data->args[1][i]))
				sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY), perror("se requiere un argumento numérico\n"), exit(g_stat_code);
		}
		sc_error(ft_atoi(data->args[1]) % 256);
		if (ft_atoi(data->args[1]) % 256)
			perror("exit\n");
		else
			printf("exit\n");
		exit(g_stat_code);
	}
	printf("exit\n"), exit(g_stat_code);
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
	sc_error(SC_SUCCESS), exit(g_stat_code);
}

void	switch_builtin(t_data **ddata)
{
	t_data	*data;

	data = *ddata;
	if (!ft_strcmp(data->comand, "echo"))
		b_echo(data);
	else if (!ft_strcmp(data->comand, "cd"))
		b_cd(data, getenv("HOME"));
	else if (!ft_strcmp(data->comand, "pwd"))
		b_pwd();
	else if (!ft_strcmp(data->comand, "export"))
		b_export(ddata);
	else if (!ft_strcmp(data->comand, "unset"))
		b_unset(data);
	else if (!ft_strcmp(data->comand, "env"))
		b_env(data);
	else if (!ft_strcmp(data->comand, "exit"))
		b_exit(data);
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

// TODO gestionar los exit con exit status
void	execute_command(t_data **ddata, char *command_path, int heredoc_processed)
{
	pid_t	pid;
	t_data	*data;
	int		heredoc_fd;
	int		status;
	pid = fork();
	data = *ddata;
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		if (data && (data->redir != NULL && data->redir->type == D_MINOR) \
				&& heredoc_processed == 0)
		{
			heredoc_fd = heredoc(data);
			printf("1\n");
			if (heredoc_fd != -1 && dup2(heredoc_fd, STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}

			close(heredoc_fd);
			printf("2\n");
		}
		if (data->redir != NULL)
			handle_redir(data);
		if (ft_strcmp(command_path, "is_builtinOMG") == 0)
		{
			switch_builtin(ddata);
			exit(g_stat_code);
		}
		else
		{
			if (!command_path)
				exit(SC_KEY_HAS_EXPIRED);
			printf("command path: %s\n", command_path);
			print_data(data);
			if (execve(command_path, data->args, data->env) == -1)
			{
			printf("3\n");
				perror("execve");
				exit(EXIT_FAILURE);
			}
		}
	}
	else
	{
		while ((pid = wait(&status)) > 0)
		{
			if (WIFEXITED(status))
				g_stat_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_stat_code = WTERMSIG(status);
		}
	}
}

void	execute_pipeline(t_data **data)
{
	t_data	*current;
	int		input_fd;
	int		fd[2];
	pid_t	pid;
	int		heredoc_fd;
	int		heredoc_processed;
	int		status;
	int		last_pid;

	heredoc_processed = 0;
	heredoc_fd = -1;
	input_fd = STDIN_FILENO;
	current = *data;
	while (current != NULL)
	{
		if (current->redir != NULL && current->redir->type == D_MINOR)
		{
			if (heredoc_fd == -1)
			{
				heredoc_fd = heredoc(current);
				heredoc_processed = 1;
			}
		}
			//heredoc_fd = heredoc(current);
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
			if (!is_valid_command(current, heredoc_processed))
				sc_error(SC_KEY_HAS_EXPIRED), exit(g_stat_code);
			sc_error(SC_SUCCESS), exit(g_stat_code);
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
	last_pid = 0;
	while ((pid = wait(&status)) > 0)
    {
        if (pid > last_pid && WIFEXITED(status))
            g_stat_code = WEXITSTATUS(status);
        else if (pid > last_pid && WIFSIGNALED(status))
            g_stat_code = WTERMSIG(status);
		last_pid = pid;
    }
	signal(SIGINT, SIG_DFL);
	if(g_stat_code == 130)
		write(STDOUT_FILENO, "\n", 1);
}

int	is_valid_command(t_data *data, int heredoc_processed)
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
			execute_command(&data, data->comand, heredoc_processed);
			return (1);
		}
		if (data->redir != NULL)
		{
			handle_redir(data);
		}
		//free(path);
		return (0);
	}
	if (is_builtin(data->comand))
	{
		execute_command(&data, "is_builtinOMG", heredoc_processed);
		free(path);
		return (1);
	}
	token = ft_split(path, ':');
	free(path);	
	if (access(data->comand, F_OK) == 0)
	{
		if (access(data->comand, X_OK) == 0)
		{
			execute_command(&data, data->comand, heredoc_processed);
			free_args(token);
			return (1);
		}
		else
			sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE);
	}

	while (token[i] != NULL)
	{
		tmp = ft_strjoin(token[i], "/");
		comand_path = ft_strjoin(tmp, data->comand);
		free(tmp);
		if(access(comand_path, F_OK) == 0)
		{
			if (access(comand_path, X_OK) == 0)
			{
				execute_command(&data, comand_path, heredoc_processed);
				free(comand_path);
				free_args(token);
				return (1);
			}
			sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE), exit(126);
		}
		free(comand_path);
		i++;
	}
	if (data->redir != NULL)
		execute_command(&data, data->path, heredoc_processed);
	free_args(token);
	printf("%s: command not found\n", data->comand);
	return (sc_error(SC_KEY_HAS_EXPIRED), 0);
}
