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

void sc_error(int sce)
{
	g_stat_code = sce;
}

char	*heredoc_tokenizer(char *str, t_data *data)
{
	t_token	*token;
	int		i;
	char	*input;
	char	*res;

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
				return (is_error(& (t_error){"Memory error",1}), free(input) \
						, free_token(&token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
		else
		{
			if (!add_token(input[i], typeing(input[i], " |><\'\"") \
						, &token))
				return (is_error(& (t_error){"Memory error",1}), free(input) \
						, free_token(&token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
	}
	free(input);
	while (token)
	{
		if (is_special(token->value, "$") && (!is_special(token->next->value, "\"'")))
		{
			if (!is_expandsor(&token, &res, data->env))
				return(free_token(&token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
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
	free_token(&token);
	return (res);
}

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
		return (close(fd), perror("El archivo no existe"), sc_error(SC_KEY_HAS_EXPIRED), 1);
	if (ft_strchr(check, 'R') && access(filename, R_OK))
		return (close(fd), perror("El archivo no tiene permisos de lectura"), sc_error(EXIT_FAILURE), 2);
	if (ft_strchr(check, 'W') && access(filename, W_OK))
		return (close(fd), perror("El archivo no tiene permisos de escritura"), sc_error(EXIT_FAILURE), 3);
	if (ft_strchr(check, 'X') && access(filename, X_OK))
		return (close(fd), perror("El archivo no tiene permisos de ejecución"), sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE), 4);
	return (0);
}

// TODO gestionar los exit con exit status
int	heredoc(t_redir	*aux, t_data *data) 
{
	int		fd;
	char	*line;
	char	*filename;
	char	*expanded_line;

	filename = "/tmp/heredoc";
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (is_valid_file(filename, fd, "FRW"))
		exit(g_stat_code);
	if (signal(SIGINT, handle_sigint_heredoc) == SIG_ERR)
	{
		perror("Error al configurar el manejador de SIGINT");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		if (g_stat_code == SC_HEREDOC)
		{
			close(fd);
			exit(SC_HEREDOC);
		}
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, aux->path) == 0)
		{
			free(line);
			break ;
		}
		expanded_line = heredoc_tokenizer(line, data);
		if (!expanded_line)
			close(fd), free(line), exit(g_stat_code);
		ft_putstr_fd(expanded_line, fd);
		if (!(ft_strlen(expanded_line) == 1 && expanded_line[0] != '\0'))
			ft_putstr_fd("\n", fd);
		free(line);
		free(expanded_line);
	}
		close(fd);
	return open(filename, O_RDONLY);
}

void	handle_redir(t_data *data, int heredoc_processed)
{
	int 	fd;
	t_redir *redir;

	redir = data->redir;
	while (redir != NULL)
	{
		if (data && (data->redir != NULL && data->redir->type == D_MINOR) \
				&& heredoc_processed == 0)
		{
			fd = heredoc(redir, data);
		}
		else if (redir->type == MAJOR)
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
		else if (!heredoc_processed && !redir->next && redir->type == D_MINOR && data->comand != NULL)
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
// cat << a cat << b cat << c
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

int	ft_is_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] != '-')
		return (1);
	while(str[++i])
	{
		if (str[i] != 'n')
		return (1);
	}
	return (0);
}

//TODO mirar el error del open que no hay permisos al usar is_valid_file salta error
void	b_echo(t_data *data)
{
	int	i;
	int	fd;

	i = 1;
	if (data->args[i] && !ft_is_n(data->args[i]))
	{
		i++;
		fd = open("/tmp/echoafjnaifsnk", O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (is_valid_file("/tmp/echoafjnaifsnk", fd, "FW"))
			close(fd), unlink("/tmp/echoafjnaifsnk"), exit(g_stat_code);
		while (data->args[i])
		{
			// data->args[i] = heredoc_tokenizer(data->args[i], data);
			// if (!data->args[i])
			// 	exit(g_stat_code);
			ft_putstr_fd(data->args[i], fd);
			if (data->args[++i])
				ft_putstr_fd(" ", fd);
		}
		ft_putstr_fd(BLUE"Minishell: "BLACK, fd);
		close(fd);
    }
    else if (data->args[i])
    {
        while (data->args[i])
        {
			// data->args[i] = heredoc_tokenizer(data->args[i],data);
			// if (!data->args[i])
			// 	exit(g_stat_code);
            printf("%s", data->args[i]);
            if (data->args[++i])
                printf(" ");
        }
		printf("\n");
    }
	else
		printf("\n");
	g_stat_code = SC_SUCCESS;
	free_args(&data->args);
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

void	print_export(t_data *data, char *str)
{
	int		i;
	char	*key;
	i = -1;
	if (data->env)
	{
		while (data->env[++i])
		{
			if(ft_strchr(data->env[i], '='))
			{
				key = (char *)malloc(sizeof(char) * (ft_strchr(data->env[i], '=') - data->env[i]) + 2);
				if (key)
				{
					ft_strncpy(key, data->env[i], ft_strchr(data->env[i], '=') - data->env[i] + 1);
					printf("%s%s\"%s\"\n", str, key, data->env[i] + ft_strlen(key));
					free(key);
				}
			}
			else
				printf("%s%s\n", str, data->env[i]);
		}
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

//TODO manage g_status_code
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
		return (free_args(mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
	c_mat = *mat;
	while (c_mat[++i])
	{
		new_mat[i] = ft_strdup(c_mat[i]);
		if (!new_mat[i])
		{
			new_mat[i] = NULL;
			return (free_args(&new_mat), free_args(mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
	}
	new_mat[i] = ft_strdup(str);
	if (!new_mat[i])
		{
			new_mat[i] = NULL;
			return (free_args(&new_mat), free_args(mat), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
		}
	new_mat[++i] = NULL;

	free_args(mat);
	return (new_mat);
}

int	is_special_string(char *c, char *comp)
{
	char	*str;

	while (*c)
	{
		str = ft_strchr(comp, *c);
		if (str)
			return (1);
		c++;
	}
		return (0);
}

// TODO liberar key???
void	b_export(t_data **data)
{
	int		i;
	char	*key;
	char	*need;
	t_data	*cdata;
	int		j;

	j = 0;
	if (!(*data)->args[1])
		return(print_export(*data, "declare -x "));
	cdata = *data;
	while ((*data)->args[++j])
	{
		need = ft_strnstr((cdata)->args[j],"=", ft_strlen((cdata)->args[j]));
		if(need)
		{
			key = (char*)malloc((need - (cdata)->args[j]) +1);
			ft_strlcpy(key, (cdata)->args[j], ((need - (cdata)->args[j]) + 1));
			if(!*key || is_special_string(key, " <>|'\".,-+*!¡?¿%%=·@$#ªº¬€"))
			{
				free(key);
				return (sc_error(SC_INVALID_ARGUMENT), printf("export: `%s': not a valid identifier\n", (cdata)->args[j]), exit(g_stat_code));
			}
		}
		else
		{
			key = ft_strdup((cdata)->args[j]);
			if(!*key || is_special_string(key, " <>|'\".,-+*!¡?¿%%=·@$#ªº¬€"))
			{
				free(key);
				return (sc_error(SC_INVALID_ARGUMENT), printf("export: `%s': not a valid identifier\n", (cdata)->args[j]), exit(g_stat_code));
			}
		}
		// printf("la i es %s\n", key);
		if (index_env(cdata, key) >= 0)
		{
			i = index_env(cdata, key);
			// printf("la i es %i\n", i);
			if(need && need[0] == '=')
				(cdata)->env[i] = (cdata)->args[j];
		}
		else
		{
			(cdata)->env = ft_matadd(&(cdata)->env, (cdata)->args[j]);
			if (!(cdata)->env)
				exit(g_stat_code);
		}
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
	free_args(mat);
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
	int	j;

	j = 0; 
	while(data->args[++j])
	{
		i = index_env(data, data->args[j]);
		// printf("%i\n", i);
		if (i != -1)
			data->env = ft_mat_rem_index(&data->env, i);
	}
	unlink("/tmp/env.env");
	if (save_env(data))
		exit(g_stat_code);
	sc_error(SC_SUCCESS), exit(g_stat_code);
}
//TODO si es mayor que 2, too many arguments
//si el primer valor es correcto y el segundo es incorrecto, no cierra minishell
//dos parametros validos cierra correctamente
// void	b_exit(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (data->args[i])
// 		i++;
// 	if (i > 1)
// 	{
// 		sc_error(EXIT_FAILURE), perror("demasiados argumentos\n"), printf("exit\n");
// 		return ;
// 	}
// 	if (data->args[1])
// 	{
// 		i = 0;
// 		while(data->args[1][i++])
// 		{
// 			if (!ft_isdigit(data->args[1][i]))
// 				sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY), ft_putstr_fd("se requiere un argumento numérico\n", 2), exit(g_stat_code);
// 		}
// 		sc_error(ft_atoi(data->args[1]) % 256);
// 		if (ft_atoi(data->args[1]) % 256)
// 			ft_putstr_fd("exit\n", 2);
// 		else
// 			printf("exit\n");
// 		exit(g_stat_code);
// 	}
// 	printf("exit\n"), sc_error(SC_SUCCESS), exit(g_stat_code);
// }

void	b_exit(t_data *data)
{
	int	i;
	int j;

	j = 0;
	i = 0;
	while (data->args[i])
		i++;
	if (data->args[1])
	{
		j = 0;
		while(data->args[1][j])
		{
			if (!ft_isdigit(data->args[1][j]))
				sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY), printf("exit\n"), ft_putstr_fd("numeric argument required\n", 2), exit(g_stat_code);
			j++;
		}
	}
	if (i > 2)
	{
		sc_error(EXIT_FAILURE), printf("exit\n"), ft_putstr_fd("exit: too many arguments\n", 2);
		return ;
	}
	if (data->args[1])
	{
		sc_error(ft_atoi(data->args[1]) % 256);
		g_stat_code = ft_atoi(data->args[1]) % 256;
		printf("exit\n");
		exit(g_stat_code);
	}
	printf("exit\n"), sc_error(SC_SUCCESS), exit(g_stat_code);
}

void	b_env(t_data *data)
{
	int	i;

	i = -1;
	if(data->args[1])
		ft_putstr_fd("Error, env not accept arguments\n", 2), sc_error(SC_KEY_HAS_EXPIRED), exit(g_stat_code);
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

	int		status;
	wait_signal(0);
	pid = fork();
	data = *ddata;
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		if (data->redir != NULL)
			handle_redir(data, heredoc_processed);
		if (ft_strcmp(command_path, "is_builtinOMG") == 0)
		{
			switch_builtin(ddata);
			exit(g_stat_code);
		}
		else
		{
			if (!command_path)
				exit(SC_KEY_HAS_EXPIRED);
			if (execve(command_path, data->args, data->env) == -1)
			{
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

	pid = 0;
	heredoc_processed = 0;
	heredoc_fd = -1;
	input_fd = STDIN_FILENO;
	current = *data;
	last_pid = 0;
	while (current != NULL)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (current->redir != NULL && current->redir->type == D_MINOR && !heredoc_processed)
			{
				heredoc_fd = heredoc(current->redir, current);
				if (heredoc_fd == -1)
					heredoc_processed = 1;
				if (dup2(heredoc_fd, STDIN_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
				if (g_stat_code == SC_HEREDOC)
					sc_error(1), exit(1);
				close(heredoc_fd);
				heredoc_processed = 1;
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
				if (dup2(fd[1], STDOUT_FILENO) == -1)
				{
					perror("dup2");
					exit(EXIT_FAILURE);
				}
			}
			close(fd[0]);
			close(fd[1]);
			if (current->redir != NULL)
				handle_redir(current, heredoc_processed);
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
			else
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (heredoc_fd != -1)
			{
				close(heredoc_fd);
				heredoc_fd = -1;
			}
			current = current->next;
			last_pid = 0;
			if ((*data)->heredoc == 1)
			{
				while ((pid = wait(&status)) > 0)
				{
					if (pid > last_pid && WIFEXITED(status))
						g_stat_code = WEXITSTATUS(status);
					else if (pid > last_pid && WIFSIGNALED(status))
						g_stat_code = WTERMSIG(status);
					last_pid = pid;
				}
				heredoc_processed = 0;
			}
		}
	}
	while ((pid = wait(&status)) > 0)
	{
		if (pid > last_pid && WIFEXITED(status))
			g_stat_code = WEXITSTATUS(status);
		else if (pid > last_pid && WIFSIGNALED(status))
			g_stat_code = WTERMSIG(status);
		last_pid = pid;
	}
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
	if ((!path || !data->comand) && !is_builtin(data->comand))
	{
		if (data->redir != NULL && data->redir->type == D_MINOR)
		{
			execute_command(&data, data->comand, heredoc_processed);
			return (1);
		}
		if (data->redir != NULL)
		{
			handle_redir(data, heredoc_processed);
		}
		//free(path);
		ft_putstr_fd("Comand not found\n", 2);
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
			free_args(&token);
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
				free_args(&token);
				return (1);
			}
			sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE), exit(126);
		}
		free(comand_path);
		i++;
	}
	if (data->redir != NULL)
		execute_command(&data, data->path, heredoc_processed);
	free_args(&token);
	printf("%s: command not found\n", data->comand);
	return (sc_error(SC_KEY_HAS_EXPIRED), 0);
}
