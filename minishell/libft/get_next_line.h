/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduenas- <aduenas-@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:25:49 by aduenas-          #+#    #+#             */
/*   Updated: 2023/09/03 19:00:09 by aduenas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/uio.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2
# endif

char	*read_file(int fd, char *buff);
char	*get_next_line(int fd);
char	*ft_strchr(const char *string, int c);
char	*ft_line(char *buff);
char	*ft_strjoin(char const *str1, char const *str2);
void	*ft_calloc(size_t count, size_t size);

size_t	ft_strlen(const char *str);

#endif
