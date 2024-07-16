/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/07/16 08:30:01 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	setup_signal_handlers(void)
{
	if (signal(SIGINT, handle_sigint) == SIG_ERR)
	{
		perror("Error al configurar el manejador de SIGINT");
		sc_error(EXIT_FAILURE), exit(g_stat_code);
	}
	if (signal(SIGQUIT, handle_sigquit) == SIG_ERR)
	{
		perror("Error al configurar el manejador de SIGQUIT");
		sc_error(EXIT_FAILURE), exit(EXIT_FAILURE);
	}
}