/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:42:09 by aduenas-          #+#    #+#             */
/*   Updated: 2024/08/18 20:23:42 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sigint_received = 0;

void handle_sigint_global(int sig)
{
    (void)sig;
    g_sigint_received = 1;
}

void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigint_minishell(int sig)
{
	(void)sig;
}

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit(130);
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	child_handler(int signal)
{
	if (signal == SIGQUIT)
	{
		printf("Quit: (Core dumped)\n");
		return ;
	}
}

void	wait_signal(int i)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	ft_memset(&sa_int, 0, sizeof(sa_int));
	ft_memset(&sa_quit, 0, sizeof(sa_quit));
	if(i == 2)
	{
		sa_int.sa_handler = &handle_sigint_minishell;
		sa_quit.sa_handler = &handle_sigquit;
	}
	else if (i)
	{
		sa_int.sa_handler = &handle_sigint;
		sa_quit.sa_handler = &handle_sigquit;
	}
	else
	{
		sa_int.sa_handler = &child_handler;
		sa_quit.sa_handler = &child_handler;
	}
	sa_int.sa_flags = SA_RESTART;
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
	sigaction(SIGTERM, &sa_int, NULL);
}
