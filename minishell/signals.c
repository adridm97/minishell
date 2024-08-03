/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/03 13:58:33 by kevin            ###   ########.fr       */
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

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_stat_code = SC_HEREDOC;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit(SC_HEREDOC);
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	child_handler(int signal)
{
	if (signal == SIGINT)
		g_stat_code = 130;
	else if (signal == SIGQUIT)
	{
		printf("Quit: 3\n");
		g_stat_code = 131;
	}
	return ;
}

// void	setup_signal_handlers(void)
// {
// 	if (signal(SIGINT, handle_sigint) == SIG_ERR)
// 	{
// 		perror("Error al configurar el manejador de SIGINT");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (signal(SIGQUIT, handle_sigquit) == SIG_ERR)
// 	{
// 		perror("Error al configurar el manejador de SIGQUIT");
// 		exit(EXIT_FAILURE);
// 	}
// }

void	wait_signal(int i)
{
	struct sigaction	sa;

	if (i)
	{
		sa.sa_handler = &handle_sigint;
	}
	else
	{
		sa.sa_handler = &child_handler;
	}
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
}
