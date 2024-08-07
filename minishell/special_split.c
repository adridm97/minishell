/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_split.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 21:09:21 by kevin             #+#    #+#             */
/*   Updated: 2024/08/07 21:26:29 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_words(char const *s)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == '<' || s[i] == '>')
			in_word = 0;
		else
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
		i++;
	}
	return (count);
}

static int	lenword(char *s, int start)
{
	int	i;
	int	count;

	i = start;
	count = 0;
	while (((s[i] != '<' && s[i] != '>') && s[i] != '\0'))
	{
		i++;
		count++;
	}
	return (count);
}

static void	free_mat(char **mat, int c_words)
{
	while (c_words >= 0)
		free(mat[c_words--]);
	free(mat);
}

static int	fill_mat(char **mat, char *s, int n_words)
{
	int	start;
	int	c_word;
	int	end;

	start = 0;
	c_word = 0;
	while (c_word < n_words - 1)
	{
		while (s[start] == '<' || s[start] == '>')
			start++;
		end = lenword(s, start);
		mat[c_word] = ft_substr(s, start, end);
		if (!mat[c_word])
		{
			free_mat(mat, c_word);
			return (0);
		}
		start += end;
		c_word++;
	}
	mat[c_word] = NULL;
	return (1);
}

char	**special_split(char const *s)
{
	int		n_words;
	char	**mat;

	if (!s)
		return (NULL);
	n_words = count_words(s) + 1;
	mat = (char **)malloc(n_words * sizeof(char *));
	if (!mat)
		return (NULL);
	if (!fill_mat(mat, (char *)s, n_words))
		return (NULL);
	return (mat);
}
