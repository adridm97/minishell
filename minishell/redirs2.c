/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:24:04 by adrian            #+#    #+#             */
/*   Updated: 2024/08/13 07:25:51 by kevin            ###   ########.fr       */
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
	while (c_data->next)
		c_data = c_data->next;
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
