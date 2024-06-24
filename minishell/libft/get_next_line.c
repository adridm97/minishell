/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 21:10:35 by aduenas-          #+#    #+#             */
/*   Updated: 2024/06/22 18:34:25 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
//#include <fcntl.h>
//#include <stdio.h>

static char	*ft_strjoin(char const *str1, char const *str2)
{
	char		*buffer;
	size_t		i;
	size_t		j;

	buffer = (char *)malloc(sizeof(*str1) \
				* (gnl_ft_strlen(str1) + gnl_ft_strlen(str2) + 1));
	if (!buffer)
		return (NULL);
	i = 0;
	j = 0;
	while (str1[i])
	{
		buffer[j++] = str1[i];
		i++;
	}
	i = 0;
	while (str2[i])
	{
		buffer[j++] = str2[i];
		i++;
	}
	buffer[j] = 0;
	return (buffer);
}

char	*ft_free(char *buf, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(buf, buffer);
	if (!temp)
		free(buffer);
	free(buf);
	return (temp);
}



char	*read_file(int fd, char *buff)
{
	char	*buffer;
	int		bytes;

	buffer = gnl_ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (free(buff), NULL);
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buff), free(buffer), NULL);
		buffer[bytes] = 0;
		buff = ft_free(buff, buffer);
		if (!buff)
			return (NULL);
		if (gnl_ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (buff);
}

char	*ft_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
		return (free(buffer), NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	line = gnl_ft_calloc((i + 1), sizeof(char));
	i = 0;
	if (!line)
		return (free(buffer), NULL);
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i] = '\n';
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*buff = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buff)
	{
		buff = gnl_ft_calloc(1, 1);
		if (!buff)
			return (NULL);
	}
	buff = read_file(fd, buff);
	if (!buff)
		return (NULL);
	line = ft_line(buff);
	if (line == NULL)
	{
		buff = NULL;
		return (NULL);
	}
	buff = ft_next(buff);
	return (line);
}
/*
int	main()
{
	int	fd;

	fd = open("error.txt", O_RDONLY);
	if (fd == -1)
	{
		printf("Error en leer el archivo");
		return (1);
	}
	char	*line;
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Linea leida: %s\n", line);
		free(line);
	}
	close(fd);
	return (0);
}
*/
