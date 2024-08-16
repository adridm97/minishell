/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:23:30 by adrian            #+#    #+#             */
/*   Updated: 2024/08/16 15:37:21 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sc_error(int sce, t_data **data)
{
	if (data && *data)
	{
		*(*data)->stat_code = sce;
	}
}

void	sc_error_int(int err, int *sce)
{
	if (sce)
	{
		*sce= err;
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
			update_status(pid, &pid, status, data);
			pid = wait(&status);
		}
		*processed = 0;
	}
}

void	update_status(pid_t pid, int *last_pid, int status, t_data **data)
{
	if (pid > *last_pid && WIFEXITED(status))
	{
		*(*data)->stat_code = WEXITSTATUS(status);
	}
	else if (pid > *last_pid && WIFSIGNALED(status))
	{
		*(*data)->stat_code = WTERMSIG(status) + 128;
	}
	*last_pid = pid;
}

void	wait_for_remaining_processes(int last_pid, t_data **data)
{
	int		status;
	pid_t	pid;
	pid = wait(&status);
	while (pid > 0)
	{
		update_status(pid, &last_pid, status, data);
		pid = wait(&status);
	}
}
