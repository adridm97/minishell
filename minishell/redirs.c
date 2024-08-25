/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:08:03 by adrian            #+#    #+#             */
/*   Updated: 2024/08/25 18:40:32 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirs(t_data **c_data, char **str, t_redir **redir, t_redir **l_redir)
{
	while ((*c_data)->next)
	{
		(*c_data) = (*c_data)->next;
	}
	(*redir)->path = *str;
	(*l_redir) = (*c_data)->redir;
	if (!(*l_redir))
	{
		(*c_data)->redir = (*redir);
		return ;
	}
	while ((*l_redir)->next)
	{
		(*l_redir) = (*l_redir)->next;
	}
	(*l_redir)->next = (*redir);
}

int	init_redir(t_token **token, t_data **data, char **env, int type)
{
	char	*str;
	t_redir	*redir;
	t_redir	*l_redir;
	t_data	*c_data;

	if (exp_var(&(*data)->exp, type) && !set_redir(type, &redir, data))
		return (0);
	c_data = *data;
	str = NULL;
	while ((*token))
	{
		if (is_special((*token)->value, "'\"$" ) && (*data)->exp)
			(switch_case_redir(token, &str, env, data));
		else if (is_special((*token)->value, "| <>") && str)
			return (manage_redirs(&c_data, &redir, str), 1);
		else if ((*token)->value == ' ')
			*token = (*token)->next;
		else
		{
			str = new_str(&str, (*token)->value);
			*token = (*token)->next;
		}
	}
	redirs(&c_data, &str, &redir, &l_redir);
	return (1);
}

int	is_redir_input(t_token **token, t_data **data, char **str, char **env)
{
	if (*token)
		*token = (*token)->next;
	if ((*token)->value == '<')
	{
		*token = (*token)->next;
		if (!init_redir(token, data, env, D_MINOR))
			return (0);
	}
	else if ((*token)->value == '>')
	{
		*token = (*token)->next;
		if (!init_redir(token, data, env, SPACES))
			return (0);
	}
	else
	{
		if (!init_redir(token, data, env, MINOR))
			return (0);
	}
	(void)str;
	return (1);
}

int	is_redir_output(t_token **token, t_data **data, char **str, char **env)
{
	if (*token)
		*token = (*token)->next;
	if ((*token)->value == '>')
	{
		*token = (*token)->next;
		if (!init_redir(token, data, env, D_MAJOR))
			return (0);
	}
	else if ((*token)->value == '<')
	{
		*token = (*token)->next;
		if (!init_redir(token, data, env, SPACES))
			return (0);
	}
	else
	{
		if (!init_redir(token, data, env, MAJOR))
			return (0);
	}
	(void)str;
	return (1);
}

void	switch_case_redir(t_token **token,
		char **str, char **env, t_data **data)
{
	if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else if ((*token)->value == '"')
		is_double_string(token, env, str, data);
	else if ((*token)->value == '$')
		is_expandsor(token, str, env, data);
}
