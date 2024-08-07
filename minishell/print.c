/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrian <adrian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:46:17 by adrian            #+#    #+#             */
/*   Updated: 2024/08/07 11:49:59 by adrian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export(t_data *data, char *str)
{
	int		i;
	char	*key;

	i = -1;
	if (!data->pipe && data->env)
	{
		while (data->env[++i])
		{
			if (ft_strchr(data->env[i], '='))
			{
				key = (char *)malloc(sizeof(char) * \
				(ft_strchr(data->env[i], '=') - data->env[i]) + 2);
				if (key)
				{
					ft_strncpy(key, data->env[i], \
					ft_strchr(data->env[i], '=') - data->env[i] + 1);
					printf("%s%s\"%s\"\n", str, key, \
					data->env[i] + ft_strlen(key));
					free(key);
				}
			}
			else
				printf("%s%s\n", str, data->env[i]);
		}
	}
}

void	print_env(t_data *data, char *str)
{
	int	i;

	i = -1;
	if (data->env)
	{
		while (data->env[++i])
			printf("%s%s\n", str, data->env[i]);
	}
}

void	print_env_env(char **env, char *str)
{
	int	i;

	i = -1;
	if (env)
	{
		while (env[++i])
			printf("%s%s\n", str, env[i]);
	}
}

void	print_redir(t_redir *redir)
{
	if (!redir)
		return ;
	while (redir)
	{
		printf("Redirs\n");
		printf("path: %s, type: %i\n", redir->path, redir->type);
		redir = redir->next;
	}
}

void	print_data(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (data)
	{
		printf("data %i\nComand: %s|\n", i, data->comand);
		while (data->args && data->args[++j])
			printf("arg[%i]: %s|\nnext = %p\n", j, data->args[j], data->next);
		j = -1;
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		print_redir(data->redir);
		data = data->next;
		j = -1;
		i++;
	}
}
