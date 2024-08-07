/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:23:30 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 14:46:23 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sc_error(int sce)
{
	g_stat_code = sce;
}

void	update_heredoc_status(t_data **data, pid_t pid, int *processed)
{
	int	status;

	if ((*data)->heredoc == 1)
	{
		pid = wait(&status);
		while (pid > 0)
		{
			update_status(pid, &pid, status);
			pid = wait(&status);
		}
		*processed = 0;
	}
}

void	update_status(pid_t pid, int *last_pid, int status)
{
	if (pid > *last_pid && WIFEXITED(status))
		g_stat_code = WEXITSTATUS(status);
	else if (pid > *last_pid && WIFSIGNALED(status))
		g_stat_code = WTERMSIG(status);
	*last_pid = pid;
}

void	wait_for_remaining_processes(int last_pid)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		update_status(pid, &last_pid, status);
		pid = wait(&status);
	}
}
