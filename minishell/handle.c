/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:37:58 by adrian            #+#    #+#             */
/*   Updated: 2024/08/27 07:43:46 by adrian           ###   ########.fr       */
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
			return (execute_command(&data, data->comand, heredoc_processed),
				*data->stat_code);
		else if (data->comand)
			return (sc_error(SC_KEY_HAS_EXPIRED, &data),
				ft_putstr_fd(data->comand, 2),
				ft_putstr_fd(": command not found\n", 2), *data->stat_code);
		return (close(fd), *data->stat_code);
	}
}

void	handle_dups_simple(int fd, t_redir *redir, t_data **data)
{
	if (fd == -1)
	{
		if (redir->path)
			ft_putstr_fd(redir->path, 2);
		return (sc_error(1, data), ft_putstr_fd(" No such file or directory\n", 2));
	}
	if ((redir->type == MAJOR || redir->type == D_MAJOR) \
	&& (*data)->comand != NULL)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			(perror("dup2"), exit(EXIT_FAILURE));
	}
	else if (redir->type == MINOR && (*data)->comand != NULL)
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
		return (ft_putstr_fd(" No such file or directory\n", 2),
			sc_error(1, &data));
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

/*		
// if (data && (redir->type == D_MINOR && heredoc_processed == 0))
// {
// 	fd = heredoc(redir, data);
// 	if (fd == -1)
// 	{
// 		perror("heredoc");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (last_heredoc != -1)
// 		close(last_heredoc);
// 	last_heredoc = fd;
// }
*/

void	handle_redir_simple(t_data **data)
{
	int		fd;
	t_redir	*redir;

	redir = (*data)->redir;
	while (redir != NULL)
	{
		fd = -1;
		if (redir->type == MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == D_MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == MINOR)
			fd = open(redir->path, O_RDONLY);
		else
		{
			printf("avanza path\n");
			redir = redir->next;
			continue ;
		}
		print_redir(redir);
		handle_dups_simple(fd, redir, data);
		if (redir->type != D_MINOR)
			close(fd);
		redir = redir->next;
	}
}

void	handle_redir(t_data *data, int heredoc_processed)
{
	int			fd;
	t_redir		*redir;

	fd = -2;
	(void)heredoc_processed;
	redir = data->redir;
	while (redir != NULL)
	{
		if (data && (redir != NULL && redir->type == D_MINOR) \
				&& heredoc_processed == 0)
			fd = heredoc(redir, data);
		if (redir->type == MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == D_MAJOR)
			fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == MINOR)
			fd = open(redir->path, O_RDONLY);
		if (fd != -2)
			handle_dups(fd, redir, data);
		redir = redir->next;
		if (fd != -2)
			close(fd);
	}
}
