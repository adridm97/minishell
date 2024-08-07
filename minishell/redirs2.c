/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:24:04 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 16:26:21 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_redir(int type, t_redir **redir)
{
	(*redir) = (t_redir *)malloc(sizeof(t_redir));
	if (!(*redir))
	{
		sc_error(SC_CANNOT_ALLOCATE_MEMORY);
		return (0);
	}
	(*redir)->next = NULL;
	(*redir)->type = type;
	(*redir)->path = NULL;
	return (1);
}

void	manage_redirs(t_data **data, t_redir **credir, char *str)
{
	t_data	*c_data;
	t_redir	*l_redir;
	t_redir	*redir;

	redir = *credir;
	c_data = *data;
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
