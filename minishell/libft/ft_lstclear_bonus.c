/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:52:26 by kevin             #+#    #+#             */
/*   Updated: 2024/04/09 09:52:28 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *gon;
	t_list *aux;

	if (!(aux = *lst))
		return ;
	while (aux)
	{
		gon = aux->next;
		del(aux->content);
		free(aux);
		aux = gon;
	}
	*lst = NULL;
}