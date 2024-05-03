/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 21:09:21 by kevin             #+#    #+#             */
/*   Updated: 2024/05/03 11:42:54 by kevin            ###   ########.fr       */
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
char *new_str(char **str, char c)
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
	if(str)
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
		if ((*token)->value == '$')
		{
			is_expandsor(token, &res, env);
			break;	
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
			break;	
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

// void	add_redir(t_data **data, t_redir **redir)
// {

// }
void	switch_case_redir(t_token **token, char **str, char **env)
{
	if ((*token)->value == '\'')
		is_simple_string(token, env, str);
	else if ((*token)->value == '"')
		is_double_string(token, env, str);
	else if ((*token)->value == '$')
		is_expandsor(token, str, env);
}
void	init_redir(t_token **token, t_data **data, char **env, int type)
{
	char	*str;
	t_redir *redir;
	t_redir	*l_redir;

	redir = (t_redir *)malloc(sizeof(t_redir));
	redir->next = NULL;
	redir->type = type;
	redir->path = NULL;
	str = charstr((*token)->value);
	*token = (*token)->next;
	while ((*token))
	{
		if (is_special((*token)->value, "'\"$"))
			switch_case_redir(token, &str, env);
		else if (is_special((*token)->value, "| <>"))
		{
			//TODO logica para incluir el redir e incluir str Puedo ayudarme de la func add_args
			redir->path = str;
			printf("al inicio en path %s\n", redir->path);
			if (!(*data)->redir)
			{
				printf("ENTRE?\n");
				(*data)->redir = redir;
				printf("SALGA\n");
				return;
			}
			l_redir = (*data)->redir;
			while (l_redir->next)
				l_redir = l_redir->next;
			l_redir->next = redir;
			return;
		}
		else
		{
			str = new_str(&str, (*token)->value);
			*token = (*token)->next;
		}
	}
	redir->path = str;
	printf("al final en path %s\n", redir->path);
	l_redir = (*data)->redir;
	if (!l_redir)
	{
		(*data)->redir = redir;
		return;
	}
	while (l_redir->next)
		l_redir = l_redir->next;
	l_redir->next = redir;
	return;
}

// split the redirection and put in data
void	is_redir_input(t_token **token, t_data **data, char **str, char **env)
{
	t_data	*last_data;

	last_data = (*data);
	while (last_data->next)
		last_data = last_data->next;
	if(*str)
	printf("antes de add args\n"), add_args(&last_data->args, str);
	printf("segunda vuelta\n");
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
		init_redir(token, data, env, MINOR);
	printf("Saliendo de is_redir_input\n");
}


// split the redirection and put in data
void	is_redir_output(t_token **token, t_data **data, char **str, char **env)
{
	t_data	*last_data;

	last_data = (*data);
	while (last_data->next)
		last_data = last_data->next;
	add_args(&last_data->args, str);
	*token = (*token)->next;
	if ((*token)->value == '>')
	{
		*token = (*token)->next;
		init_redir(token, data, env, D_MAJOR);
	}
	else if ((*token)->value == '>')
	{
		*token = (*token)->next;
		init_redir(token, data, env, SPACES);
	}
	else
		init_redir(token, data, env, MAJOR);
}


// split the pipe and put in data
void	is_pipe(t_token **token, t_data **data, char **str)
{
	t_data	*n_data;
	t_data	*last_data;

	init_data(&n_data);
	last_data = (*data);
	while (last_data->next)
		last_data = last_data->next;
	if(*str)
	{
		if(!add_args(&(last_data)->args, str))
			(void)str; //TODO Controlar error
	}
	last_data->next = n_data;
	*token = (*token)->next;
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
		if (ft_strnstr(env[i], *key, ft_strlen(*key)) && env[i][ft_strlen(*key)] == '=')
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
void	is_expandsor(t_token **token, char **str, char **env)
{
	char	*key;
	char	*res;

	key = NULL;
	*token = (*token)->next;
	if (!*token || is_special((*token)->value, "<> |"))
		*str = new_str(str, '$');
	else if ((*token)->value == '"')
	{
		*token = (*token)->next;
		is_double_string(token, env, str);
	}
	else if ((*token)->value == '\'')
	{
		*token = (*token)->next;
		is_simple_string(token, env, str);
	}
	else
	{
		if (*token && take_key(token, &key, " <>|'\""))
		{
			key = key_to_res(&key, env);
			if (key)
			{
				if (!*str)
					res = ft_strjoin("", key);
				else
					res = ft_strjoin(*str, key);
				free(*str), free(key);
				*str = res;
			}
		}
	}
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	if(!args)
	 return (0);
	while (args[i])
		i++;
	return (i);
}

/*TODO OJO si falla malloc no se esta liberando arg*/
int add_args(char ***arg, char **str)
{
	char	**args;
	int		i;

	i = 0;
	if(*arg)
		args = (char **)malloc(sizeof(char *) * (count_args(*arg) + 2));
	else
		args = (char **)malloc(sizeof(char *) * 2);
	if (!args)
		return (0);
	while((*arg) && (*arg)[i])
	{
		args[i] = ft_strdup((*arg)[i]);
		if(args[i])
			i = i;//TODO GESTIONAR ERROR con return
		free((*arg)[i]);
		i++;
	}
	args[i] = ft_strdup(*str);
	if(args[i])
		i = i;//TODO GESTIONAR ERROR con return
	args[i + 1] = NULL;
	free(*str), free(*arg);
	*str = NULL;
	*arg = args;
	return (1);
}

void	is_space(t_token **token, t_data **data, char **str)
{
	*token = (*token)->next;
	add_last_data(data, str);
	// if(*str)
	// {
	// 	if(!add_args(&(*data)->args, str))
	// 		(void)str; //TODO Controlar error
	// }
	// if ((*token))
	// 	(*token) = (*token)->next;
	// printf("Estoy despues de add args\n");
	// if ((*token) && is_special((*token)->value, " |\"'<>$"))
	// {
	// 	switch_case(token, env, data, str);
	// }
	// printf("DESPUES DEL SWITCH\n");
}

//switch case en funcion delcaracter
void	switch_case(t_token **token, char **env, t_data **data, char **str)
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
		is_pipe(token, data, str);
	else if ((*token)->value == '$')
		is_expandsor(token, str, env);
	else if ((*token)->value == ' ')
		is_space(token, data, str);
}

void	add_last_data(t_data **data, char **str)
{
	t_data *n_data;

	n_data = *data;
	while (n_data->next)
		n_data = n_data->next;
	if (!add_args(&n_data->args, str))
		(void)str; //TODO Controlar error
}

/*TODO Poner en path el path || OJOOOO $PATH hola Pierde la h*/
void	split_token(t_token *token, char **env, t_data **data)
{
	char	*str;

	if(!init_data(data))
		(void)data; //falta gestionar errores
	str = NULL;
	while (token)
	{
		if (is_special(token->value, " |\"'<>$"))
		{
			switch_case(&token, env, data, &str);
			// printf("despues del SC\n");
			// printf("EN STR=%s, en value=%c\n", str, (*token).value);
		}
		else
		{
			str = new_str(&str, token->value);
			token = token->next;
			// TODO if (!str)
			// 	return (ERROR);
		}
	}
	printf("str EN SPLIT_TOKEN: %s\n", str);
	if(str)
	{
		add_last_data(data, &str);
	}
	// if((*data)->args)
	// {
	// 	if ((*data)->args[0])
	// 		printf("data EN SPLIT_TOKEN: %s\n", (*data)->args[0]);
	// 	if ((*data)->args[1])
	// 		printf("data EN SPLIT_TOKEN: %s\n", (*data)->args[1]);
	// }
}

/*
void	advance_token_list(t_token **token, t_list **lst, int *i)
{
	char	*str;
	if((*token))
		(*token) = (*token)->next;
	printf("DENTRO\n");
	str = (char *)(*lst)->content;
	if(!str[*i])
	{
		*i = 0;
		(*lst) = (*lst)->next;
	}
	else
	*i += 1;
	printf("FINAL\n");
}

int	token_len(t_token *token)
{
	int len;
	t_token *ctoken;

	len = 0;
	ctoken = token;
	while(ctoken)
	{
		ctoken = ctoken->next;
		len++;
	}
	return (len);
}

char **list_to_matrix(t_list *lst)
{
	while (lst)
	{
		printf("-%s-\n", (char *)lst->content);
		lst = lst->next;
	}
	return (NULL);
}

char *next_word(t_token *token, int *start)
{
	t_token *ctoken;
	int		len;
	int		is_coma;
	char	*str;
	int		type;
	
	len = *start;
	ctoken = token;
	is_coma = 0;
	str = NULL;
	type = -1;
	while (--len >= 0)
		ctoken = ctoken->next;
	while (ctoken && (!(ctoken->type >= SPACES && ctoken->type <= MINOR) || is_coma))
	{
		if (ctoken->type >= QUOTE && ctoken->type <= D_QUOTE && ctoken)
		{
			if (ctoken && type >= 0 && ctoken->type != type)
			{
				*start += 1;
				if (!ctoken)
					break;
				if (str)
					str = ft_strjoin(str, ft_substr(&ctoken->value,0,1));
				else
					str = ft_substr(&ctoken->value,0,1);				
				ctoken = ctoken->next;
			}
			else
			{
				type = ctoken->type;
				*start += 1;
				ctoken = ctoken->next;
				if (!is_coma)
					is_coma = 1;
				else
				{
					is_coma = 0;
					type = -1;
				}
			}
		}
		else
		{
			if (!ctoken)
				break;
			if (str)
				str = ft_strjoin(str, ft_substr(&ctoken->value,0,1));
			else
				str = ft_substr(&ctoken->value,0,1);
			ctoken = ctoken->next;
			*start += 1;
		}
	}
	// printf("start= %i, str= %s\n", *start, str);
	return (str);
}

void	advance_special(t_token *token, int *start)
{
	t_token *ctoken;
	int	j;
	
	j = *start;
	ctoken = token;
	while (ctoken && --j >= 0)
		ctoken = ctoken->next;
	if (ctoken && ctoken->type == MINOR)
	{
		*start += 1;
		ctoken = ctoken->next;
		if (ctoken->type == MINOR)
			*start += 1;
	}
	else if (ctoken && ctoken->type == MAJOR)
	{
		*start += 1;
		ctoken = ctoken->next;
		if (ctoken->type == MAJOR)
			*start += 1;
	}
	else if (ctoken && ctoken->type == SPACES)
	{
		while(ctoken && ctoken->type == SPACES)
		{
			*start += 1;
			ctoken = ctoken->next;
		}
	}
	else
		*start += 1;
}

int	have_expansor(t_token *token)
{
	t_token *ctoken;

	ctoken = token;

	while (ctoken)
	{
		if (ctoken->value == '$')
			return (1);
		ctoken = ctoken->next;
	}
	return (0);
}

//ya paso el inicio del string, comilla incluida para saber cómo avanzar
void	expand_string(t_token **token, t_list **lst, int *i)
{
	t_list *clst;
	char	*str;
	char	*res;
	char	*s_lst;

	s_lst = (char *)(*lst)->content;
	clst = (*lst);
	printf("token: %c, s_lst c: %c, s_lst %s", (*token)->value, s_lst[*i], s_lst);
	while (0)
	{
		if (ft_strchr((char *)clst->content, '$'))
		{
			str = ft_substr((char *)clst->content, 0, ft_strchr((char *)clst->content, '$') - (char *)clst->content);
			res = ft_strjoin(str, ft_strchr((char *)clst->content, '$') + 1);
			free (str);
			free (clst->content);
			clst->content = res;
			return ;
		}
		else
			clst = clst->next;
	}

	// t_list *clst;
	// char	*str;
	// char	*res;

	// clst = lst;
	// while (clst)
	// {
	// 	if (ft_strchr((char *)clst->content, '$'))
	// 	{
	// 		str = ft_substr((char *)clst->content, 0, ft_strchr((char *)clst->content, '$') - (char *)clst->content);
	// 		res = ft_strjoin(str, ft_strchr((char *)clst->content, '$') + 1);
	// 		free (str);
	// 		free (clst->content);
	// 		clst->content = res;
	// 		return ;
	// 	}
	// 	else
	// 		clst = clst->next;
	// }
}

void	free_matrix(char **mat)
{
	int	i;

	i = -1;
	if (!mat)
		return;
	while (mat[++i])
		free(mat[i]);
	free(mat);
	mat = NULL;
}

// TODO No está funcionando como se espera, no devuelve el path adecuado
int	search_key_env(char **env, char *res)
{
	int	i;
	char **key_value;

	i = -1;
	printf("RES = %s", res);
	while (env[++i])
	{
		key_value = ft_split(env[i], '=');
		if (!key_value)
			return (-1);
		// printf("env %i = %s\n", i, env[i]);
		// printf("%i qui: %s\n", i, key_value[0]);
		if (!ft_strncmp(key_value[0], res, ft_strlen(key_value[0])))
			return (free_matrix(key_value), i);
		free_matrix(key_value);
	}
	return (-1);
}

char	*expandsor(char **env, t_token *token, int iter)
{
	t_token *ctoken;
	char	*str;
	char	*res;
	int		i;

	ctoken = token;
	str = ft_strdup("");
	while (ctoken->value && ctoken->value != '$' && iter > 0)
	{
		if (iter && ctoken->value == '$')
			iter--;
		ctoken = ctoken->next;	
	}
	ctoken = ctoken->next;
		if (ctoken->value != '"' && ctoken->value != '\'')
		{
			while (ctoken && ft_isalnum(ctoken->value))
			{
				res = ft_strjoin(str, &ctoken->value);
				free(str);
				str = res;
				ctoken = ctoken->next;
			}
		}
		i = search_key_env(env, res);
		// printf("OJOOO %i\n", i);
		if (i >= 0)
			str = ft_strdup(ft_strchr(env[i],'='));
		// printf("hhhhhhhhhh:%s\n", str);
		if (!str)
			return (NULL);
	return (str);
}

void	expand_env(t_list *lst, char **env, t_token *token)
{
	t_list *clst;
	int		iter;
	char	*str;
	char	*exp;
	char	*res;

	clst = lst;
	iter = 0;
	exp = NULL;
	while (clst)
	{
		if (ft_strchr((char *)clst->content, '$'))
		{
			exp = expandsor(env, token, iter);
			iter++;
			str = ft_substr((char *)clst->content, 0, ft_strchr((char *)clst->content, '$') - (char *)clst->content);
			//TODO peta aquí
			res = ft_strjoin(str, exp);
			free (str);
			free (clst->content);
			free (exp);
			exp = NULL;
			clst->content = res;
			return ;
		}
		else
			clst = clst->next;
	}
}

// TODO ojo que no expanda si el $ esta dentro de las ""
void	add_expansor(t_token *token, char **env, t_list *lst)
{
	t_token *ctoken;
	int		i;
	char	*content;

	content = (char*)lst->content;
	ctoken = token;
	i = 0;
	printf("ANTES token: %c, s_lst c: %c, s_lst %s\n", ctoken->value, content[i], (char*)lst->content);
	advance_token_list(&ctoken, &lst, &i);
	printf("DESPUES token: %c, s_lst c: %c, s_lst %s\n", ctoken->value, content[i], (char*)lst->content);
	while (ctoken)
	{
		if (ctoken->value == '$')
		{
			// ctoken = ctoken->next;
			advance_token_list(&ctoken, &lst, &i);
			if (ctoken->value == '\'' || ctoken->value == '"')
				{
					expand_string(&ctoken, &lst, &i);
					// TODO avanzar token hasta encontrar las comillas
				}
			else
				expand_env(lst, env, token);
		}
		ctoken = ctoken->next;
	}
}

//TODO no funciona: hola adios col> dedo guarda 1 null que no debería
char	**split_token(char const *s, t_token *token, char **env)
{
	t_list	*lst;
	int		start;
	int		lenth;
	void	*str;

	if (1)
		return (NULL);
	(void)s;
	start = 0;
	lenth = token_len(token);
	lst = NULL;
	// ft_lstadd_back(&lst, ft_lstnew((char const *)next_word(token, &start)));
	while (start < lenth)
	{
		str = next_word(token, &start);
		if (str)
			ft_lstadd_back(&lst, ft_lstnew((char const *)str));
		advance_special(token, &start);
	}
	if (have_expansor(token))
		add_expansor(token, env, lst);
	return (list_to_matrix(lst));
}

*/
