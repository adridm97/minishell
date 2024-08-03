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

int token_to_str(t_token **token, char **res, t_data **data)
{
	if (is_special((*token)->value, "$") && (!is_special((*token)->next->value, "\"'")))
		{
			if (!is_expandsor(token, res, (*data)->env))
				return(free_token(token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
		}
		else
		{
			if ((*token))
			{
				*res = new_str(res, (*token)->value);
				(*token) = (*token)->next;
			}
		}
		return (1);
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
		if (!token_to_str(&token, &res, &data))
			return(free_token(&token), sc_error(SC_CANNOT_ALLOCATE_MEMORY), NULL);
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
