/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:46:45 by adrian            #+#    #+#             */
/*   Updated: 2024/08/23 16:15:47 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(t_data **ddata, char *command_path, int processed)
{
	t_data	*data;

	data = *ddata;
	if ((*ddata)->pipe && data->redir != NULL && data->comand && *data->comand)
	{
		handle_redir(data, processed);
	}
	else if (data->redir != NULL && data->comand && *data->comand)
	{
		handle_redir_simple(data, processed);
	}
	if (ft_strcmp(command_path, "is_builtinOMG") == 0)
	{
		switch_builtin(ddata);
		exit(*data->stat_code);
	}
	else
	{
		if (!data->comand || !command_path || !*data->comand)
		{
			exit(handle_missing_command(data, processed));
		}
		if (execve(command_path, data->args, data->env) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
}

void	handle_heredoc(t_data *current, t_exec_vars *vars)
{
	t_redir	*redir;

	redir = current->redir;
	while(redir != NULL)
	{
		if (redir->type == D_MINOR)
		{
			vars->heredoc_fd = heredoc(redir, current);
			if (vars->heredoc_fd == -1)
			{
				vars->heredoc_processed = 1;
				return ;
			}
			if (g_sigint_received == 1)
				exit((sc_error(2 + 128, &current), *current->stat_code));
		}
		redir = redir->next;
	}
	if (dup2(vars->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
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
	int file_fd;
	
	file_fd = -1;
    if (current->redir != NULL)
    {
        t_redir *redir = current->redir;
        while (redir != NULL)
        {
            if (redir->type == MAJOR)
            {
                file_fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (file_fd == -1)
                {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                if (dup2(file_fd, STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
                close(file_fd);
            }
            redir = redir->next;
        }
    }
    if (current->next != NULL)
    {
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2 pipe");
            exit(EXIT_FAILURE);
        }
    }
    close(fd[0]);
    close(fd[1]);
}
