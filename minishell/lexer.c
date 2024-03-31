/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 13:20:02 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/03/31 13:21:14 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_token(char *input, int start, int end, t_token **token)
{
	t_token	new;

	printf("start: %i, end: %i\n", start, end);
	new.next = NULL;
	new.value = ft_substr(input, start, end);
	*token = &new;
	printf("EN NEW AL FINAL: %s\n", (*token)->value);
}

void	add_token(char *input, int start, int end, t_token **token)
{
	t_token	new;
	t_token	*lst;

	printf("start: %i, end: %i\n", start, end);
	printf("value de token: %s\n", (*token)->value);
	lst = *token;
	printf("value de token: %s\n", lst->value);
	while (lst->next)
	{
		printf("AGADGASDRGAERWGARG\n");
		lst = lst->next;
	}
	new.next = NULL;
	printf("EN ADD AL FINAL: %s\n",lst->value);
	new.value = ft_substr(input, start, end);
	printf("EN ADD AL FINAL: %s\n",lst->value);
	lst->next = &new;
}

int	is_special(char c)
{
	if (c == '|' || c == ' ' || !c)
		return (1);
	return (0);
}
/*Por algun motivo solo imprimer la utima palabra pero debería imprimir la primera*/
int	lexer(char *input, t_data *data)
{
	t_token	*token;
	int		i;
	int		start;
	int		end;

	token = NULL;
	i = -1;
	while (input[++i])
	{
		if (!is_special(input[i]))
		{
			start = i;
			while (!is_special(input[++i]))
			{
				end = i;
			}
			if (!token)
			{
			//	printf("start: %i, end: %i, %c, %c\n", start, end, input[start], input[end]);
				new_token(input, start, end + 1, &token);
			}
			else
			 	add_token(input, start, end, &token);
		}
	}
	if (!token)
	{
		return (1);
	}
	token = token;
	printf("%s\n", token->value);
	return (1);
}