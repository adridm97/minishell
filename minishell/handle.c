/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:37:58 by adrian            #+#    #+#             */
/*   Updated: 2024/08/11 23:12:47 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_missing_command(t_data *data, int heredoc_processed)
{
	if (data->redir != NULL && data->redir->type == D_MINOR)
		execute_command(&data, data->comand, heredoc_processed);
	else if (data->redir != NULL)
		handle_redir(data, heredoc_processed);
	else
	{
		if (data->comand)
		{
			ft_putstr_fd(data->comand, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
		}
	}
}

void	handle_dups(int fd, t_redir *redir, t_data *data, int heredoc_processed)
{
	if (fd == -1)
		return (ft_putstr_fd(data->redir->path, 2), \
		ft_putstr_fd(" No such file or directory\n", 2));
	if ((redir->type == MAJOR || redir->type == D_MAJOR) \
	&& data->comand != NULL)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), exit(EXIT_FAILURE));
	}
	else if (redir->type == MINOR && data->comand != NULL)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			(perror("dup2"), exit(EXIT_FAILURE));
	}
	else if (!heredoc_processed && !redir->next && \
	redir->type == D_MINOR && data->comand != NULL)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			(perror("dup2"), exit(EXIT_FAILURE));
	}
}

void	handle_redir(t_data *data, int heredoc_processed)
{
	int		fd;
	t_redir	*redir;

	redir = data->redir;
	while (redir != NULL)
	{
		if (data && (data->redir != NULL && redir->type == D_MINOR) \
				&& heredoc_processed == 0)
			fd = heredoc(redir, data);
		else if (redir->type == MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == D_MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == MINOR)
			fd = open(redir->path, O_RDONLY);
		else
		{
			redir = redir->next;
			continue ;
		}
		handle_dups(fd, redir, data, heredoc_processed);
		close(fd);
		redir = redir->next;
	}
}

void	handle_parent_process(t_exec_vars *vars, int fd[2], \
pid_t pid, t_data **data)
{
	close_input_fd(&(vars->input_fd));
	update_input_fd(&(vars->input_fd), fd, *data);
	close_heredoc_fd(&(vars->heredoc_fd));
	update_heredoc_status(data, pid, &(vars->heredoc_processed));
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
		sc_error(SC_KEY_HAS_EXPIRED, &current);
		exit(*current->stat_code);
	}
	sc_error(SC_SUCCESS, &current);
	exit(*current->stat_code);
}
