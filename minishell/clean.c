/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:32:30 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 21:10:21 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redir(t_redir **redir)
{
	t_redir	*i;

	if (!(*redir))
		return ;
	while ((*redir))
	{
		i = (*redir)->next;
		free((*redir)->path);
		free(*redir);
		(*redir) = i;
	}
	redir = NULL;
}

void	free_args(char ***args)
{
	int	i;

	i = -1;
	if (*args)
	{
		while ((*args)[++i])
		{
			if ((*args)[i])
			{
				free((*args)[i]);
				(*args)[i] = NULL;
			}
		}
		free(*args);
	}
	*args = NULL;
}

void	free_data(t_data **data)
{
	t_data	*del;

	if (!*data)
		return ;
	while ((*data)->next)
	{
		del = (*data)->next;
		free_args(&(*data)->args);
		free_redir(&(*data)->redir);
		clean_env(&(*data)->env, -1);
		free(*data);
		*data = del;
	}
	clean_env(&(*data)->env, -1);
	if ((*data)->args)
		free_args(&(*data)->args);
	if ((*data)->redir)
		free_redir(&(*data)->redir);
	free(*data);
	*data = NULL;
}

void	clean_env(char ***env, int i)
{
	if (!*env)
		return ;
	if (i == -1)
	{
		while (env[0][++i])
		{
			free(env[0][i]);
		}
		free(*env);
	}
	else
	{
		while (--i >= 0)
			free(env[0][i]);
		free(*env);
	}
	*env = NULL;
}

void	free_token(t_token **token)
{
	t_token	*c_token;

	if (*token)
	{
		c_token = (*token)->next;
		while (*token)
		{
			free (*token);
			*token = c_token;
			if (*token)
				c_token = (*token)->next;
		}
	}
}
