/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 14:40:34 by kevin             #+#    #+#             */
/*   Updated: 2024/08/25 00:00:35 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process(t_exec_vars *vars, int fd[2],
	pid_t pid, t_data **data)
{
	close_input_fd(&(vars->input_fd));
	update_input_fd(&(vars->input_fd), fd, *data);
	close_heredoc_fd(&(vars->heredoc_fd));
	update_heredoc_status(data, pid, &(vars->heredoc_processed));
}

void	handle_child_pipes(t_data **current, t_exec_vars *vars, int fd[2])
{
	if ((*current)->redir != NULL && (*current)->redir->type == D_MINOR && !(vars->heredoc_processed))
		handle_heredoc((*current), vars);
	else
		handle_input_redirection(&(vars->input_fd));
	handle_output_redirection((*current), fd, -1);
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
