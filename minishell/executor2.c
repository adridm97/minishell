/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:17:14 by adrian            #+#    #+#             */
/*   Updated: 2024/08/03 16:17:46 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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