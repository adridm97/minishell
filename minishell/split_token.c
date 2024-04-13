/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 21:09:21 by kevin             #+#    #+#             */
/*   Updated: 2024/04/13 19:58:09 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*TODO*/
char **list_to_matrix(t_list *lst)
{
	(void)lst;
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
			if (ctoken->type != type && type >= 0 && ctoken)
			{
				*start += 1;
				if (!ctoken)
					break;
				if (str)
					str = ft_strjoin(str, ft_substr(&ctoken->value,0,1));
				else
					str = ft_substr(&ctoken->value,0,1);				
				ctoken = ctoken->next;
				break;
			}
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
		if (!ctoken)
			break;
		if (str)
			str = ft_strjoin(str, ft_substr(&ctoken->value,0,1));
		else
			str = ft_substr(&ctoken->value,0,1);
		ctoken = ctoken->next;
		*start += 1;
	}
	printf("start= %i, str= %s\n", *start, str);
	return (str);
}
/*TODO hola adios|hola>>cola no esta funcionando este caso. "hola afi""dada'" este tampoco*/
void	advance_special(t_token *token, int *start)
{
	t_token *ctoken;
	int	j;
	
	j = *start;
	ctoken = token;
	while (--j >= 0 && ctoken)
		ctoken = ctoken->next;
	if (ctoken && ctoken->type == MINOR)
	{
		*start += 1;
		ctoken = token;
		if (ctoken->type == MINOR)
			*start += 1;
	}
	else if (ctoken && ctoken->type == MAJOR)
	{
		*start += 1;
		ctoken = token;
		if (ctoken->type == MAJOR)
			*start += 1;
	}
	else
		*start += 1;
}

char	**split_token(char const *s, t_token *token)
{
	t_list	*lst;
	int		start;
	int		lenth;

	(void)s;
	start = 0;
	lenth = token_len(token);
	// ft_lstadd_back(&lst, ft_lstnew((char const *)next_word(token, &start)));
	while (start < lenth)
	{
		ft_lstadd_back(&lst, ft_lstnew((char const *)next_word(token, &start)));
		advance_special(token, &start);
	}

	return (list_to_matrix(lst));
}