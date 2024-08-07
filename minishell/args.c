/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 16:40:18 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 17:17:13 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
		i++;
	return (i);
}

void	free_args_triple(char ***arg)
{
	int	i;

	i = -1;
	if (!*arg)
		return ;
	while ((*arg)[++i])
	{
		free((*arg)[i]);
	}
	free(*arg);
	*arg = NULL;
}

int	add_args(char ***arg, char **str)
{
	char	**args;
	int		i;

	i = 0;
	if (*arg)
		args = (char **)malloc(sizeof(char *) * (count_args(*arg) + 2));
	else
		args = (char **)malloc(sizeof(char *) * 2);
	if (!args)
		return (free_args_triple(arg), 0);
	while ((*arg) && (*arg)[i])
	{
		args[i] = ft_strdup((*arg)[i]);
		if (!args[i])
			return (free_args_triple(&args), free_args_triple(arg), 0);
		i++;
	}
	args[i] = ft_strdup(*str);
	if (!args[i])
		return (free_args_triple(&args), free_args_triple(arg), 0);
	args[i + 1] = NULL;
	(free(*str), free_args_triple(arg));
	*str = NULL;
	*arg = args;
	return (1);
}

int	exp_var(int *exp, int type)
{
	if (type == D_MINOR)
		*exp = 0;
	else
		*exp = 1;
	return (1);
}
