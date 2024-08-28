/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/28 18:44:42 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_minishell(int sig)
{
	(void)sig;
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

int	ft_signal_handler(int sce)
{
	if (g_sigint_received)
	{
		sce = 130;
		g_sigint_received = 0;
	}
	return (sce);
}
