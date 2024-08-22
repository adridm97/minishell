/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:37:58 by adrian            #+#    #+#             */
/*   Updated: 2024/08/22 09:18:11 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_missing_command(t_data *data, int heredoc_processed)
{
	int	fd;

	if (data->redir != NULL)
	{
		handle_redir(data, heredoc_processed);
		return (*data->stat_code);
	}
	else
	{
		fd = open(data->comand, O_EXCL);
		if (!data->comand)
		{
			sc_error(SC_KEY_HAS_EXPIRED, &data);
			return (ft_putstr_fd(": command not found\n", 2), *data->stat_code);
		}
		else if (data->comand && !is_valid_file(data->comand, fd, "X", &data))
			return (execute_command(&data, data->comand, heredoc_processed), *data->stat_code);
		else if (data->comand)
		{
			sc_error(SC_KEY_HAS_EXPIRED, &data);
			ft_putstr_fd(data->comand, 2);
			return (ft_putstr_fd(": command not found\n", 2), *data->stat_code);
		}
		return (close(fd), *data->stat_code);
	}
}

void	handle_dups_simple(int fd, t_redir *redir, t_data *data, int heredoc_processed)
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

void	handle_dups(int fd, t_redir *redir, t_data *data)
{
	if (fd == -1)
	{
		if (data->redir->path)
			ft_putstr_fd(data->redir->path, 2);
		return (ft_putstr_fd(" No such file or directory\n", 2), sc_error(SC_KEY_HAS_EXPIRED, &data));
	}
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
}

void	handle_redir_simple(t_data *data, int heredoc_processed)
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
		handle_dups_simple(fd, redir, data, heredoc_processed);
		close(fd);
		redir = redir->next;
	}
}

void	handle_redir(t_data *data, int heredoc_processed)
{
	int			fd;
	t_redir		*redir;
	// t_exec_vars	vars;
		
	// initialize_pipe_vars(&vars);
	redir = data->redir;
	while (redir != NULL)
	{
		if (data && (redir != NULL && redir->type == D_MINOR) \
				&& heredoc_processed == 0)
			fd = heredoc(redir, data);
		else if (redir->type == MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == D_MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == MINOR)
			fd = open(redir->path, O_RDONLY);
		handle_dups(fd, redir, data);
		redir = redir->next;
		close(fd);
	}
}

void	handle_parent_process(t_exec_vars *vars, int fd[2], \
pid_t pid, t_data **data)
{
	if (vars->heredoc_processed == 0)
	{
		close_input_fd(&(vars->input_fd));
		update_input_fd(&(vars->input_fd), fd, *data, vars);
	}
	close_heredoc_fd(&(vars->heredoc_fd));
	update_heredoc_status(data, pid, &(vars->heredoc_processed));
}

void	handle_child_pipes(t_data **current, t_exec_vars *vars, int fd[2])
{
	if ((*current)->redir != NULL && !(vars->heredoc_processed))
		handle_heredoc((*current), vars);
	else
		handle_input_redirection(&(vars->input_fd));
	handle_output_redirection((*current), fd);
	close(fd[0]);
	close(fd[1]);
	if ((*current)->redir != NULL)
		handle_redir((*current), vars->heredoc_processed);
	if (!is_valid_command(current, vars->heredoc_processed))
	{
		sc_error(SC_KEY_HAS_EXPIRED, current);
		exit(*(*current)->stat_code);
	}
	sc_error(SC_SUCCESS, current);
	exit(*(*current)->stat_code);
}
