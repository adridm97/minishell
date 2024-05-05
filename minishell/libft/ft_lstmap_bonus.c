/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:54:45 by kevin             #+#    #+#             */
/*   Updated: 2024/04/09 09:54:49 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *gon;
	t_list *aux;
	t_list *auxgon;

	aux = lst;
	if (!(gon = malloc(sizeof(t_list))))
		return (0);
	auxgon = gon;
	while (aux)
	{
		auxgon->content = f(aux->content);
		if (!(auxgon->next = malloc(sizeof(t_list))))
			ft_lstclear(&aux, del);
		aux = aux->next;
		auxgon = auxgon->next;
	}
	return (gon);
}