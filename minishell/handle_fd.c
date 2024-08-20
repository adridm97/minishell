/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:16:53 by adrian            #+#    #+#             */
/*   Updated: 2024/08/19 22:16:02 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_input_fd(int *input_fd, int fd[2], t_data *current, t_exec_vars *vars)
{
	if (current->next && vars->heredoc_processed != 1)
	{
		close(fd[1]);
		*input_fd = fd[0];
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	close_heredoc_fd(int *heredoc_fd)
{
	if (*heredoc_fd != -1)
	{
		close(*heredoc_fd);
		*heredoc_fd = -1;
	}
}

void	close_input_fd(int *input_fd)
{
	if (*input_fd != STDIN_FILENO)
	{
		close(*input_fd);
		*input_fd = -1;
	}
}
