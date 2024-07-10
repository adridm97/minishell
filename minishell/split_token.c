/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 21:09:21 by kevin             #+#    #+#             */
/*   Updated: 2024/07/09 23:43:47 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*charstr(char c)
{
	char	*res;

	res = (char *)malloc(sizeof(char) * 2);
	if (!res)
		return (NULL);
	res[0] = c;
	res[1] = 0;
	return (res);
}

// return NULL if error, add 
char	*new_str(char **str, char c)
{
	char	*res;
	char	*letter;

	if (!*str)
	{
		res = charstr(c);
		return (res);
	}
	else
	{
		letter = charstr(c);
		if (!letter)
			return (NULL);
		res = ft_strjoin(*str, letter);
		if (letter)
			free(letter);
		if (*str)
		{
			free(*str);
			*str = NULL;
		}
		return (res);
	}
}

// is_special indica si el caracter que contiene es unode los siguientes: '"<>|$
int	is_special(char c, char *comp)
{
	char	*str;

	str = ft_strchr(comp, c);
	if (str)
		return (1);
	else
		return (0);
}

// save in str the string
void	is_simple_string(t_token **token, char **env, char **str)
{
	char	*res;

	res = NULL;
	if (*str)
		res = ft_strdup(*str);
	*token = (*token)->next;
	while (*token && (*token)->value != '\'')
	{
		(void)env;
		res = new_str(&res, (*token)->value);
		if (*token)
			*token = (*token)->next;
	}
	if (*token)
		*token = (*token)->next;
	free(*str);
	*str = res;
}

// save in str the string
void	is_double_string(t_token **token, char **env, char **str)
{
	char	*res;

	res = NULL;
	if (*str)
		res = ft_strdup(*str);
	*token = (*token)->next;
	while (*token && (*token)->value != '"')
	{
		if ((*token)->value == '$')
		{
			is_expandsor(token, &res, env);
			break ;
		}
		res = new_str(&res, (*token)->value);
		if (*token)
			*token = (*token)->next;
	}
	if (*token)
		*token = (*token)->next;
	free(*str);
	*str = res;
}

void	switch_case_redir(t_token **token, char **str, char **env)
{
	if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else if ((*token)->value == '"')
		is_double_string(token, env, str);
	else if ((*token)->value == '$')
		is_expandsor(token, str, env);
}

/*TODO Hay que hacerla más corta*/
void	init_redir(t_token **token, t_data **data, char **env, int type)
{
	char	*str;
	t_redir	*redir;
	t_redir	*l_redir;
	t_data	*c_data;

	c_data = *data;
	redir = (t_redir *)malloc(sizeof(t_redir));
	redir->next = NULL;
	redir->type = type;
	redir->path = NULL;
	str = NULL;
	while ((*token))
	{
		if (is_special((*token)->value, "'\"$"))
			switch_case_redir(token, &str, env);
		else if (is_special((*token)->value, "| <>") && str)
		{
			while (c_data->next)
				c_data = c_data->next;
			redir->path = str;
			if (!c_data->redir)
			{
				c_data->redir = redir;
				return ;
			}
			l_redir = c_data->redir;
			while (l_redir->next)
				l_redir = l_redir->next;
			l_redir->next = redir;
			return ;
		}
		else if ((*token)->value == ' ')
		{
			*token = (*token)->next;
		}
		else
		{
			str = new_str(&str, (*token)->value);
			*token = (*token)->next;
		}
	}
	while (c_data->next)
		c_data = c_data->next;
	redir->path = str;
	l_redir = c_data->redir;
	if (!l_redir)
	{
		c_data->redir = redir;
		return ;
	}
	while (l_redir->next)
		l_redir = l_redir->next;
	l_redir->next = redir;
}

// split the redirection and put in data
void	is_redir_input(t_token **token, t_data **data, char **str, char **env)
{
	if (*token)
		*token = (*token)->next;
	if ((*token)->value == '<')
	{
		*token = (*token)->next;
		init_redir(token, data, env, D_MINOR);
	}
	else if ((*token)->value == '>')
	{
		*token = (*token)->next;
		init_redir(token, data, env, SPACES);
	}
	else
	{
		init_redir(token, data, env, MINOR);
	}
	(void)str;
}

// split the redirection and put in data
void	is_redir_output(t_token **token, t_data **data, char **str, char **env)
{
	if (*token)
		*token = (*token)->next;
	if ((*token)->value == '>')
	{
		*token = (*token)->next;
		init_redir(token, data, env, D_MAJOR);
	}
	else if ((*token)->value == '<')
	{
		*token = (*token)->next;
		init_redir(token, data, env, SPACES);
	}
	else
		init_redir(token, data, env, MAJOR);
	printf("SALGO\n");
	(void)str;
}

// split the pipe and put in data
int	is_pipe(t_token **token, t_data **data, char **str)
{
	t_data	*n_data;
	t_data	*last_data;

	if (!init_data(&n_data, (*data)->env))
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

int	take_key(t_token **token, char **key, char *str)
{
	while (*token && !is_special((*token)->value, str))
	{
		*key = new_str(key, (*token)->value);
		*token = (*token)->next;
	}
	if (key)
		return (1);
	else
		return (0);
}

char	*key_to_res(char **key, char **env)
{
	int		i;
	char	*res;

	i = -1;
	res = NULL;
	while (env[++i])
	{
		if (ft_strnstr(env[i], *key, ft_strlen(*key)) \
				&& env[i][ft_strlen(*key)] == '=')
		{
			res = ft_strdup(&env[i][ft_strlen(*key) + 1]);
			free(*key);
			return (res);
		}
	}
	free (*key);
	return (NULL);
}

// Expand the $ with env
int	is_expandsor(t_token **token, char **str, char **env)
{
	char	*key;
	char	*res;

	key = NULL;
	*token = (*token)->next;
	if (!*token || is_special((*token)->value, "<> |"))
		*str = new_str(str, '$');
	else if ((*token)->value == '"')
	{
		is_double_string(token, env, str);
	}
	else if ((*token)->value == '\'')
	{
		is_simple_string(token, env, str);
	}
	else
	{
		if (*token && take_key(token, &key, " <>|'\".,-+*!¡?¿%%=·@#ªº¬€$"))
		{
			key = key_to_res(&key, env);
			if (key)
			{
				if (!*str)
					res = ft_strjoin("", key);
				else
					res = ft_strjoin(*str, key);
				free(*str);
				free(key);
				*str = res;
			}
			else
				return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
		}
		else
				return (sc_error(SC_CANNOT_ALLOCATE_MEMORY), 0);
	}
	return (1);
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

void	free_args_triple(char ***arg)
{
	int	i;

	i = -1;
	if (!*arg)
		return ;
	while ((*arg)[++i])
	{
		free((*arg)[i]);
	}
	free(*arg);
	*arg = NULL;
}

int	add_args(char ***arg, char **str)
{
	char	**args;
	int		i;

	i = 0;
	if (*arg)
		args = (char **)malloc(sizeof(char *) * (count_args(*arg) + 2));
	else
		args = (char **)malloc(sizeof(char *) * 2);
	if (!args)
		return (free_args_triple(arg), 0);
	while ((*arg) && (*arg)[i])
	{
		args[i] = ft_strdup((*arg)[i]);
		if (!args[i])
			return (free_args_triple(&args), free_args_triple(arg), 0);
		i++;
	}
	args[i] = ft_strdup(*str);
	if (!args[i])
		return (free_args_triple(&args), free_args_triple(arg), 0);
	args[i + 1] = NULL;
	(free(*str), free_args_triple(arg));
	*str = NULL;
	*arg = args;
	return (1);
}

void	is_space(t_token **token, t_data **data, char **str)
{
	if (*token)
		*token = (*token)->next;
	if (*str)
		add_last_data(data, str);
}

//switch case en funcion delcaracter
int	switch_case(t_token **token, char **env, t_data **data, char **str)
{
	if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else if ((*token)->value == '"')
		is_double_string(token, env, str);
	else if ((*token)->value == '<')
		is_redir_input(token, data, str, env);
	else if ((*token)->value == '>')
		is_redir_output(token, data, str, env);
	else if ((*token)->value == '|')
		return (is_pipe(token, data, str));
	else if ((*token)->value == '$')
		is_expandsor(token, str, env);
	else if ((*token)->value == ' ')
		is_space(token, data, str);
	return (1);
}

int	add_last_data(t_data **data, char **str)
{
	t_data	*n_data;

	n_data = *data;
	while (n_data->next)
		n_data = n_data->next;
	if (!add_args(&n_data->args, str))
		return (0);
	return (1);
}

void	include_comand(t_data **data)
{
	t_data	*ldata;

	ldata = *data;
	while (ldata)
	{
		if (ldata->args)
			ldata->comand = ldata->args[0];
		ldata = ldata->next;
	}
}
/*TODO Poner en path el path || OJOOOO $PATH hola Pierde la h
¿El siguiente comando da este resultado. Es correcto?
a>a a>a hola
data 0
Comand: (null)|
arg[0]: a|
arg[1]: a|
arg[2]: hola|
Redirs
path: a, type: 2
Redirs
path: a, type: 2
*/

int	split_token(t_token *token, char **env, t_data **data)
{
	char	*str;

	if (!init_data(data, env))
		return (free_data(data), 0);
	str = NULL;
	while (token)
	{
		if (is_special(token->value, " |\"'<>$"))
		{
			switch_case(&token, (*data)->env, data, &str);
		}
		else
		{
			str = new_str(&str, token->value);
			token = token->next;
		}
	}
	if (str)
	{
		if (!add_last_data(data, &str))
			return (0);
	}
	include_comand(data);
	return (1);
}
