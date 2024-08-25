/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:46:45 by adrian            #+#    #+#             */
/*   Updated: 2024/08/25 09:04:30 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(t_data **ddata, char *command_path, int processed)
{
	t_data	*data;

	data = *ddata;
	if ((*ddata)->pipe && data->redir != NULL && data->comand && *data->comand)
		handle_redir(data, processed);
	else if (data->redir != NULL && data->comand && *data->comand)
	{
		if (data->redir != NULL)
			handle_heredoc(data, &(t_exec_vars){STDIN_FILENO, -1, 0, 0});
		handle_redir_simple(data);
	}
	if (ft_strcmp(command_path, "is_builtinOMG") == 0)
		exit((switch_builtin(ddata), *data->stat_code));
	else
	{
		if (!data->comand || !command_path || !*data->comand)
			exit(handle_missing_command(data, processed));
		if (execve(command_path, data->args, data->env) == -1)
			exit((perror("execve"), EXIT_FAILURE));
	}
}

void	handle_heredoc(t_data *current, t_exec_vars *vars)
{
	t_redir	*redir;

	redir = current->redir;
	while (redir != NULL)
	{
		if (redir->type == D_MINOR)
		{
			vars->heredoc_fd = heredoc(redir, current);
			if (vars->heredoc_fd == -1)
			{
				vars->heredoc_processed = 1;
			}
			if (g_sigint_received == 1)
				exit((sc_error(2 + 128, &current), *current->stat_code));
		}
		redir = redir->next;
	}
	if (vars->heredoc_fd > 2)
	{
		if (dup2(vars->heredoc_fd, STDIN_FILENO) == -1)
			exit((perror("dup2"), EXIT_FAILURE));
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

void	handle_output_redirection(t_data *current, int fd[2], int file_fd)
{
	t_redir	*redir;

	if (current->redir != NULL)
	{
		redir = current->redir;
		while (redir != NULL)
		{
			if (redir->type == MAJOR)
			{
				file_fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (file_fd == -1)
					exit((perror("open"), EXIT_FAILURE));
				if (dup2(file_fd, STDOUT_FILENO) == -1)
					exit((perror("dup2"), (EXIT_FAILURE)));
				close(file_fd);
			}
			redir = redir->next;
		}
	}
	if (current->next != NULL)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			exit((perror("dup2 pipe"), EXIT_FAILURE));
	}
	(close(fd[0]), close(fd[1]));
}
