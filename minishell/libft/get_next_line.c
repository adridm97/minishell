/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 21:10:35 by aduenas-          #+#    #+#             */
/*   Updated: 2023/09/03 21:36:08 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
//#include <fcntl.h>
//#include <stdio.h>

char	*ft_free(char *buf, char *buffer)
{
	char	*temp;

	temp = ft_strjoin(buf, buffer);
	if (!temp)
		free(buffer);
	free(buf);
	return (temp);
}

char	*ft_next(char *buffer)
{
	int		i;
	int		j;
	char	*line;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	line = ft_calloc((ft_strlen(buffer) -i + 1), sizeof(char));
	if (!line)
	{
		free(buffer);
		return (NULL);
	}
	i++;
	j = 0;
	while (buffer[i])
		line[j++] = buffer[i++];
	free(buffer);
	return (line);
}

char	*read_file(int fd, char *buff)
{
	char	*buffer;
	int		bytes;

	buffer = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
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
		if (ft_strchr(buffer, '\n'))
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
	line = ft_calloc((i + 1), sizeof(char));
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
		buff = ft_calloc(1, 1);
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
