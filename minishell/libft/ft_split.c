/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/23 11:29:46 by kluna-bo          #+#    #+#             */
/*   Updated: 2023/09/23 11:29:46 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;
	int	in_word;

	i = -1;
	count = 0;
	in_word = 0;
	while (s[++i])
	{
		if (s[i] == c)
			in_word = 0;
		else
		{
			if (!in_word)
			{
				count++;
				in_word = 1;
			}
		}
	}
	return (count);
}

static int	lenword(char *s, char c, int start)
{
	int	i;
	int	count;

	i = start;
	count = 0;
	while ((s[i] != c && s[i] != '\0'))
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

static int	fill_mat(char **mat, char *s, char c, int n_words)
{
	int	start;
	int	c_word;
	int	end;

	start = 0;
	c_word = 0;
	while (c_word < n_words - 1)
	{
		while (s[start] == c)
			start++;
		end = lenword(s, c, start);
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

char	**ft_split(char const *s, char c)
{
	int		n_words;
	char	**mat;

	if (!s)
		return (NULL);
	n_words = count_words(s, c) + 1;
	mat = (char **)malloc(n_words * sizeof(char *));
	if (!mat)
		return (NULL);
	if (!fill_mat(mat, (char *)s, c, n_words))
		return (NULL);
	return (mat);
}
/*
int	main(void)
{
	char	**mat;
	int		c;

	c = -1;
	mat = ft_split("Hola Soy Kevin", ' ');
	if (mat)
	{
		while (mat[c])
		{
			printf("|%s|\n", mat[c]);
			free(mat[c]);
			c++;
		}
		free(mat);
	}
}
*/
