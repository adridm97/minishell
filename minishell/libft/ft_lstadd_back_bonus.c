/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 09:50:57 by kevin             #+#    #+#             */
/*   Updated: 2024/04/09 09:51:13 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list *temporal;

	temporal = (*alst);
	if ((*alst))
	{
		while (temporal->next != NULL)
			temporal = temporal->next;
		temporal->next = new;
	}
	if (!(*alst))
		((*alst) = new);
}