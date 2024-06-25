/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:54:45 by kevin             #+#    #+#             */
/*   Updated: 2024/06/25 23:16:44 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*gon;
	t_list	*aux;
	t_list	*auxgon;

	aux = lst;
	gon = malloc(sizeof(t_list));
	auxgon = gon;
	while (aux)
	{
		auxgon->content = f(aux->content);
		auxgon->next = malloc(sizeof(t_list));
		if (auxgon->next == NULL)
			ft_lstclear(&aux, del);
		aux = aux->next;
		auxgon = auxgon->next;
	}
	return (gon);
}
