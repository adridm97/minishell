/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:17:14 by adrian            #+#    #+#             */
/*   Updated: 2024/08/03 21:04:01 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(t_data *cdata, char *key, char *arg)
{
	int	i;

	i = index_env(cdata, key);
	if (i >= 0)
	{
		if (ft_strnstr(arg, "=", ft_strlen(arg)))
			cdata->env[i] = arg;
	}
	else
	{
		cdata->env = ft_matadd(&(cdata)->env, arg);
		if (!cdata->env)
			exit(g_stat_code);
	}
}

char	*extract_key(char *arg)
{
	char	*need;
	char	*key;

	need = ft_strnstr(arg, "=", ft_strlen(arg));
	if (need)
	{
		key = (char *)malloc((need - arg) + 1);
		if (!key)
			exit(g_stat_code);
		ft_strlcpy(key, arg, (need - arg) + 1);
	}
	else
	{
		key = ft_strdup(arg);
		if (!key)
			exit(g_stat_code);
	}
	return (key);
}

int	is_valid_key(char *key)
{
	return (*key && !is_special_string(key, " <>|'\".,-+*!¡?¿%%=·@$#ªº¬€"));
}

void	process_argument(t_data *cdata, char *arg)
{
	char	*key;

	key = extract_key(arg);
	if (!is_valid_key(key))
	{
		free(key);
		return (sc_error(SC_INVALID_ARGUMENT), \
		printf("export: `%s': not a valid identifier\n", arg), \
		exit(g_stat_code));
	}
	update_env(cdata, key, arg);
	free(key);
}

void	finalize_export(t_data *cdata)
{
	unlink("/tmp/env.env");
	if (save_env(cdata))
		exit(g_stat_code);
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

void	b_export(t_data **data)
{
	int	j;

	j = 0;
	if (!(*data)->args[1])
		return (print_export(*data, "declare -x "));
	while ((*data)->args[++j])
	{
		process_argument(*data, (*data)->args[j]);
	}
	finalize_export(*data);
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
	while (data->args[++j])
	{
		i = index_env(data, data->args[j]);
		if (i != -1)
			data->env = ft_mat_rem_index(&data->env, i);
	}
	unlink("/tmp/env.env");
	if (save_env(data))
		exit(g_stat_code);
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

void	check_numeric_argument(char *arg)
{
	int	j;

	j = 0;
	while (arg[j])
	{
		if (!ft_isdigit(arg[j]))
		{
			sc_error(SC_NO_SUCH_FILE_OR_DIRECTORY);
			printf("exit\n");
			ft_putstr_fd("numeric argument required\n", 2);
			exit(g_stat_code);
		}
		j++;
	}
}

//TODO si es mayor que 2, too many arguments
void	b_exit(t_data *data)
{
	int	arg_count;

	arg_count = count_args(data->args);
	if (data->args[1])
		check_numeric_argument(data->args[1]);
	if (arg_count > 2)
	{
		sc_error(EXIT_FAILURE);
		printf("exit\n");
		ft_putstr_fd("exit: too many arguments\n", 2);
		return ;
	}
	if (data->args[1])
	{
		sc_error(ft_atoi(data->args[1]) % 256);
		g_stat_code = ft_atoi(data->args[1]) % 256;
		printf("exit\n");
		exit(g_stat_code);
	}
	printf("exit\n");
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

void	b_env(t_data *data)
{
	int	i;

	i = -1;
	if (data->args[1])
	{
		ft_putstr_fd("Error, env not accept arguments\n", 2);
		sc_error(SC_KEY_HAS_EXPIRED);
		exit(g_stat_code);
	}
	if (data->env)
	{
		while (data->env[++i])
		{
			if (ft_strrchr(data->env[i], '='))
				printf("%s\n", data->env[i]);
		}
	}
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

void	switch_builtin(t_data **ddata)
{
	t_data	*data;

	data = *ddata;
	if (!ft_strcmp(data->comand, "echo"))
		b_echo(data);
	else if (!ft_strcmp(data->comand, "cd"))
		b_cd(data, getenv("HOME"), 0);
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

void	handle_child_process(t_data **ddata, char *command_path, int processed)
{
	t_data	*data;

	data = *ddata;
	if (data->redir != NULL)
		handle_redir(data, processed);
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

void	execute_command(t_data **ddata, char *command_path, int processed)
{
	pid_t	pid;
	int		status;

	wait_signal(0);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
		handle_child_process(ddata, command_path, processed);
	else
	{
		pid = wait(&status);
		while (pid > 0)
		{
			if (WIFEXITED(status))
				g_stat_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_stat_code = WTERMSIG(status);
			pid = wait(&status);
		}
	}
}

void	initialize_pipe_vars(t_exec_vars *vars)
{
	vars->input_fd = STDIN_FILENO;
	vars->heredoc_fd = -1;
	vars->heredoc_processed = 0;
	vars->last_pid = 0;
}

void	handle_heredoc(t_data *current, t_exec_vars *vars)
{
	vars->heredoc_fd = heredoc(current->redir, current);
	if (vars->heredoc_fd == -1)
		vars->heredoc_processed = 1;
	if (dup2(vars->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	if (g_stat_code == SC_HEREDOC)
	{
		sc_error(1);
		exit(1);
	}
	close(vars->heredoc_fd);
	vars->heredoc_processed = 1;
}

void	handle_input_redirection(int *input_fd)
{
	if (*input_fd != STDIN_FILENO)
	{
		if (dup2(*input_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(*input_fd);
	}
}

void	handle_output_redirection(t_data *current, int fd[2])
{
	if (current->next != NULL)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

void	handle_child_pipes(t_data *current, t_exec_vars *vars, int fd[2])
{
	if (current->redir != NULL && current->redir->type == D_MINOR && \
		!(vars->heredoc_processed))
		handle_heredoc(current, vars);
	else
		handle_input_redirection(&(vars->input_fd));
	handle_output_redirection(current, fd);
	close(fd[0]);
	close(fd[1]);
	if (current->redir != NULL)
		handle_redir(current, vars->heredoc_processed);
	if (!is_valid_command(current, vars->heredoc_processed))
	{
		sc_error(SC_KEY_HAS_EXPIRED);
		exit(g_stat_code);
	}
	sc_error(SC_SUCCESS);
	exit(g_stat_code);
}

void	update_input_fd(int *input_fd, int fd[2], t_data *current)
{
	if (current->next != NULL)
	{
		close(fd[1]);
		*input_fd = fd[0];
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	close_heredoc_fd(int *heredoc_fd)
{
	if (*heredoc_fd != -1)
	{
		close(*heredoc_fd);
		*heredoc_fd = -1;
	}
}

void	update_status(pid_t pid, int *last_pid, int status)
{
	if (pid > *last_pid && WIFEXITED(status))
		g_stat_code = WEXITSTATUS(status);
	else if (pid > *last_pid && WIFSIGNALED(status))
		g_stat_code = WTERMSIG(status);
	*last_pid = pid;
}

void	wait_for_remaining_processes(int last_pid)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		update_status(pid, &last_pid, status);
		pid = wait(&status);
	}
}

void	update_heredoc_status(t_data **data, pid_t pid, int *processed)
{
	int	status;

	if ((*data)->heredoc == 1)
	{
		pid = wait(&status);
		while (pid > 0)
		{
			update_status(pid, &pid, status);
			pid = wait(&status);
		}
		*processed = 0;
	}
}

void	close_input_fd(int *input_fd)
{
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
}

void	handle_parent_process(t_exec_vars *vars, int fd[2], \
pid_t pid, t_data **data)
{
	close_input_fd(&(vars->input_fd));
	update_input_fd(&(vars->input_fd), fd, *data);
	close_heredoc_fd(&(vars->heredoc_fd));
	update_heredoc_status(data, pid, &(vars->heredoc_processed));
}

void	execute_pipeline(t_data **data)
{
	int			fd[2];
	pid_t		pid;
	t_exec_vars	vars;
	t_data		*current;

	current = *data;
	initialize_pipe_vars(&vars);
	pid = 0;
	while (current != NULL)
	{
		if (pipe(fd) == -1)
			(perror("pipe"), exit(EXIT_FAILURE));
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			handle_child_pipes(current, &vars, fd);
		else
			handle_parent_process(&vars, fd, pid, data);
		current = current->next;
	}
	wait_for_remaining_processes(vars.last_pid);
}

// void	execute_pipeline(t_data **data)
// {
// 	int			fd[2];
// 	pid_t		pid;
// 	t_exec_vars	vars;
// 	t_data *current;
// 	current = *data;
// 	initialize_pipe_vars(&vars);
// 	pid = 0;
// 	while (current != NULL)
// 	{
// 		if (pipe(fd) == -1)
// 		{
// 			perror("pipe");
// 			exit(EXIT_FAILURE);
// 		}
// 		pid = fork();
// 		if (pid == -1)
// 		{
// 			perror("fork");
// 			exit(EXIT_FAILURE);
// 		}
// 		else if (pid == 0)
// 			handle_child_pipes(current, &vars, fd);
// 		//{
// 		// 	if (current->redir != NULL && current->redir->type ==D_MINOR 
//	&& !heredoc_processed)
// 		// 	{
// 		// 		heredoc_fd = heredoc(current->redir, current);
// 		// 		if (heredoc_fd == -1)
// 		// 			heredoc_processed = 1;
// 		// 		if (dup2(heredoc_fd, STDIN_FILENO) == -1)
// 		// 		{
// 		// 			perror("dup2");
// 		// 			exit(EXIT_FAILURE);
// 		// 		}
// 		// 		if (g_stat_code == SC_HEREDOC)
// 		// 			sc_error(1), exit(1);
// 		// 		close(heredoc_fd);
// 		// 		heredoc_processed = 1;
// 		// 	}
// 		// 	else if (input_fd != STDIN_FILENO)
// 		// 	{
// 		// 		if (dup2(input_fd, STDIN_FILENO) == -1)
// 		// 		{
// 		// 			perror("dup2");
// 		// 			exit(EXIT_FAILURE);
// 		// 		}
// 		// 		close(input_fd);
// 		// 	}
// 		// 	if (current->next != NULL)
// 		// 	{
// 		// 		if (dup2(fd[1], STDOUT_FILENO) == -1)
// 		// 		{
// 		// 			perror("dup2");
// 		// 			exit(EXIT_FAILURE);
// 		// 		}
// 		// 	}
// 		// 	close(fd[0]);
// 		// 	close(fd[1]);
// 		// 	if (current->redir != NULL)
// 		// 		handle_redir(current, heredoc_processed);
// 		// 	if (!is_valid_command(current, heredoc_processed))
// 		// 		sc_error(SC_KEY_HAS_EXPIRED), exit(g_stat_code);
// 		// 	sc_error(SC_SUCCESS), exit(g_stat_code);
// 		// }
// 		else
// 			handle_parent_process(&vars, fd, pid, data);
//         current = current->next;
// 		// {
// 		// 	if (input_fd != STDIN_FILENO)
// 		// 		close(input_fd);
// 		// 	if (current->next != NULL)
// 		// 	{
// 		// 		close(fd[1]);
// 		// 		input_fd = fd[0];
// 		// 	}
// 		// 	else
// 		// 	{
// 		// 		close(fd[0]);
// 		// 		close(fd[1]);
// 		// 	}
// 		// 	if (heredoc_fd != -1)
// 		// 	{
// 		// 		close(heredoc_fd);
// 		// 		heredoc_fd = -1;
// 		// 	}
// 		// 	current = current->next;
// 		// 	last_pid = 0;
// 		// 	if ((*data)->heredoc == 1)
// 		// 	{
// 		// 		while ((pid = wait(&status)) > 0)
// 		// 		{
// 		// 			if (pid > last_pid && WIFEXITED(status))
// 		// 				g_stat_code = WEXITSTATUS(status);
// 		// 			else if (pid > last_pid && WIFSIGNALED(status))
// 		// 				g_stat_code = WTERMSIG(status);
// 		// 			last_pid = pid;
// 		// 		}
// 		// 		heredoc_processed = 0;
// 		// 	}
// 		// }
// 	}
// 	wait_for_remaining_processes(vars.last_pid);
// 	// while ((pid = wait(&status)) > 0)
// 	// {
// 	// 	if (pid > last_pid && WIFEXITED(status))
// 	// 		g_stat_code = WEXITSTATUS(status);
// 	// 	else if (pid > last_pid && WIFSIGNALED(status))
// 	// 		g_stat_code = WTERMSIG(status);
// 	// 	last_pid = pid;
// 	// }
// }

char	*build_command_path(char *dir, char *comand)
{
	char	*tmp;
	char	*comand_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	comand_path = ft_strjoin(tmp, comand);
	free(tmp);
	return (comand_path);
}

int	find_command_in_paths(t_data *data, char **token, int heredoc_processed)
{
	int		i;
	char	*comand_path;

	i = -1;
	while (token[++i] != NULL)
	{
		comand_path = build_command_path(token[i], data->comand);
		if (comand_path == NULL)
			return (0);
		if (access(comand_path, F_OK) == 0)
		{
			if (access(comand_path, X_OK) == 0)
			{
				execute_command(&data, comand_path, heredoc_processed);
				(free(comand_path), free_args(&token));
				return (1);
			}
			(sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE), exit(126));
		}
		free(comand_path);
	}
	if (data->redir != NULL)
		execute_command(&data, data->path, heredoc_processed);
	(free_args(&token), printf("%s: command not found\n", data->comand));
	return (sc_error(SC_KEY_HAS_EXPIRED), 0);
}

int	search_and_execute_command(t_data *data, char *path, int heredoc_processed)
{
	char	**token;

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
	return (find_command_in_paths(data, token, heredoc_processed));
}

void	handle_missing_command(t_data *data, int heredoc_processed)
{
	if (data->redir != NULL && data->redir->type == D_MINOR)
		execute_command(&data, data->comand, heredoc_processed);
	else if (data->redir != NULL)
		handle_redir(data, heredoc_processed);
	ft_putstr_fd("Command not found\n", 2);
}

int	is_valid_command(t_data *data, int heredoc_processed)
{
	char	*path;

	path = ft_strdup("PATH");
	path = key_to_res(&path, data->env);
	if (!path || !data->comand)
	{
		handle_missing_command(data, heredoc_processed);
		free(path);
		return (0);
	}
	if (is_builtin(data->comand))
	{
		execute_command(&data, "is_builtinOMG", heredoc_processed);
		free(path);
		return (1);
	}
	return (search_and_execute_command(data, path, heredoc_processed));
}

// int	is_valid_command(t_data *data, int heredoc_processed)
// {
// 	char	*path;
// 	int		i;
// 	char	*comand_path;
// 	char	**token;
// 	char	*tmp;

// 	i = 0;
// 	path = ft_strdup("PATH");
// 	path = key_to_res(&path, data->env);
// 	if (!path || !data->comand)
// 	{
// 		handle_missing_command(data, heredoc_processed);
//         free(path);
//         return (0);
// 		// if (data->redir != NULL && data->redir->type == D_MINOR)
// 		// {
// 		// 	execute_command(&data, data->comand, heredoc_processed);
// 		// 	return (1);
// 		// }
// 		// if (data->redir != NULL)
// 		// 	handle_redir(data, heredoc_processed);
// 		// ft_putstr_fd("Comand not found\n", 2);
// 		// return (0);
// 	}
// 	if (is_builtin(data->comand))
// 	{
// 		execute_command(&data, "is_builtinOMG", heredoc_processed);
// 		free(path);
// 		return (1);
// 	}
// 	return search_and_execute_command(data, path, heredoc_processed);

// 	// token = ft_split(path, ':');
// 	// free(path);	
// 	// if (access(data->comand, F_OK) == 0)
// 	// {
// 	// 	if (access(data->comand, X_OK) == 0)
// 	// 	{
// 	// 		execute_command(&data, data->comand, heredoc_processed);
// 	// 		free_args(&token);
// 	// 		return (1);
// 	// 	}
// 	// 	else
// 	// 		sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE);
// 	// }

// 	// while (token[i] != NULL)
// 	// {
// 	// 	tmp = ft_strjoin(token[i], "/");
// 	// 	comand_path = ft_strjoin(tmp, data->comand);
// 	// 	free(tmp);
// 	// 	if(access(comand_path, F_OK) == 0)
// 	// 	{
// 	// 		if (access(comand_path, X_OK) == 0)
// 	// 		{
// 	// 			execute_command(&data, comand_path, heredoc_processed);
// 	// 			free(comand_path);
// 	// 			free_args(&token);
// 	// 			return (1);
// 	// 		}
// 	// 		sc_error(SC_REQUIRED_KEY_NOT_AVAILABLE), exit(126);
// 	// 	}
// 	// 	free(comand_path);
// 	// 	i++;
// 	// }
// 	// if (data->redir != NULL)
// 	// 	execute_command(&data, data->path, heredoc_processed);
// 	// free_args(&token);
// 	// printf("%s: command not found\n", data->comand);
// 	// return (sc_error(SC_KEY_HAS_EXPIRED), 0);
// }
