/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:24:04 by adrian            #+#    #+#             */
/*   Updated: 2024/08/25 18:37:27 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_redir(int type, t_redir **redir, t_data **data)
{
	(*redir) = (t_redir *)malloc(sizeof(t_redir));
	if (!(*redir))
	{
		sc_error(SC_CANNOT_ALLOCATE_MEMORY, data);
		return (0);
	}
	(*redir)->next = NULL;
	(*redir)->type = type;
	if (type == D_MINOR)
		(*data)->heredoc = 1;
	(*redir)->path = NULL;
	return (1);
}

int	have_heredoc(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == D_MINOR)
			return (1);
		redir = redir->next;
	}
	return (0);
}

void	manage_redirs(t_data **data, t_redir **credir, char *str)
{
	t_data	*c_data;
	t_redir	*l_redir;
	t_redir	*redir;

	redir = *credir;
	c_data = *data;
	while (c_data->next)
		c_data = c_data->next;
	if (have_heredoc(redir))
		c_data->heredoc = 1;
	if (!c_data->redir)
	{
		c_data->redir = redir;
		redir->path = str;
		return ;
	}
	l_redir = c_data->redir;
	while (l_redir->next)
		l_redir = l_redir->next;
	l_redir->next = redir;
	redir->path = str;
}
