/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 21:09:21 by kevin             #+#    #+#             */
/*   Updated: 2024/04/13 11:31:47 by kevin            ###   ########.fr       */
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

}

char	**split_token(char const *s, t_token *token)
{
	t_list	*lst;
	int		start;
	int		lenth;

	start = 0;
	lenth = token_len(token);
	lst = ft_lstnew(next_word(token, &start));
	return (list_to_matrix(lst));
}