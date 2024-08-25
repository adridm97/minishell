/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 14:40:34 by kevin             #+#    #+#             */
/*   Updated: 2024/08/25 18:36:48 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process(t_exec_vars *vars, int fd[2],
	pid_t pid, t_data **data)
{
	if (vars->heredoc_processed != 1)
	{
		close_input_fd(&(vars->input_fd));
		update_input_fd(&(vars->input_fd), fd, *data);
	}
	close_heredoc_fd(&(vars->heredoc_fd));
	update_heredoc_status(data, pid, &(vars->heredoc_processed));
}

void	handle_child_pipes(t_data **current, t_exec_vars *vars, int fd[2])
{
	if ((*current)->redir != NULL && !(vars->heredoc_processed))
	{
		handle_heredoc((*current), vars);
	}
	handle_input_redirection(&(vars->input_fd));
	handle_output_redirection((*current), fd, -1);
	close(fd[0]);
	close(fd[1]);
	if (vars->heredoc_fd > 2)
		close(vars->heredoc_fd);
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
