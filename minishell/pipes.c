/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:27:48 by adrian            #+#    #+#             */
/*   Updated: 2024/08/19 19:06:43 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_pipe_vars(t_exec_vars *vars)
{
	vars->input_fd = STDIN_FILENO;
	vars->heredoc_fd = -1;
	vars->heredoc_processed = 0;
	vars->last_pid = 0;
	signal(SIGINT, handle_sigint_global);
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
		current->pipe = 1;
		if (pipe(fd) == -1)
			(perror("pipe"), exit(EXIT_FAILURE));
		pid = fork();
		if (pid == -1)
			(perror("fork"), exit(EXIT_FAILURE));
		else if (pid == 0)
			handle_child_pipes(&current, &vars, fd);
		else
		{
			handle_parent_process(&vars, fd, pid, &current);
			if (g_sigint_received)
			{
				sc_error(130, *(&data));
				kill(pid, SIGINT);
				break;
			}
		}
		current = current->next;
	}
	wait_for_remaining_processes(vars.last_pid, data);
	if (g_sigint_received)
		kill(0, SIGINT);
}

int	is_pipe(t_token **token, t_data **data, char **str, int *sce)
{
	t_data	*n_data;
	t_data	*last_data;

	if (!init_data(&n_data, (*data)->env, sce))
		return (free(*str), 0);
	last_data = (*data);
	while (last_data->next)
		last_data = last_data->next;
	if (*str)
	{
		if (!add_args(&(last_data)->args, str))
			return (0);
	}
	last_data->next = n_data;
	*token = (*token)->next;
	return (1);
}
