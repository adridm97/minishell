/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 14:40:34 by kevin             #+#    #+#             */
/*   Updated: 2024/08/24 19:48:13 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_parent_process(t_exec_vars *vars, int fd[2],
	pid_t pid, t_data **data)
{
	// if (vars->heredoc_processed == 0)
	// {
	close_input_fd(&(vars->input_fd));
	update_input_fd(&(vars->input_fd), fd, *data, vars);
	// }
	close_heredoc_fd(&(vars->heredoc_fd));
	update_heredoc_status(data, pid, &(vars->heredoc_processed));
}

//TODO condicion para heredoc es correcta?
void	handle_child_pipes(t_data **current, t_exec_vars *vars, int fd[2])
{
	printf("handle_child_pipes inicio\n");
	if ((*current)->redir != NULL && !(vars->heredoc_processed))
	{
		printf("handle_child_pipes antes heredoc\n");
		handle_heredoc((*current), vars);
	}
	else
	{
		printf("handle_child_pipes antes handle_input_redirection\n");
		handle_input_redirection(&(vars->input_fd));
	}
	printf("handle_child_pipes antes handle_output_redirection\n");
	handle_output_redirection((*current), fd, -1);
	ft_putstr_fd("handle_child_pipes despues out\n", 2);
	close(fd[0]);
	close(fd[1]);
	ft_putstr_fd("handle_child_pipes fallo alcerrar\n", 2);
	if ((*current)->redir != NULL)
	{
		ft_putstr_fd("handle_child_pipes POR FAVOR AQUI NO\n", 2);
		handle_redir((*current), vars->heredoc_processed);
	}
	ft_putstr_fd("handle_child_pipes antes is_valid comand\n", 2);
	if (!is_valid_command(current, vars->heredoc_processed))
	{
		sc_error(SC_KEY_HAS_EXPIRED, current);
		exit(*(*current)->stat_code);
	}
	ft_putstr_fd("handle_child_pipes DESPUES is_valid comand\n", 2);
	sc_error(SC_SUCCESS, current);
	exit(*(*current)->stat_code);
}
