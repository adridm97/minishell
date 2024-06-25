/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 20:25:49 by aduenas-          #+#    #+#             */
/*   Updated: 2024/06/22 18:32:54 by kevin            ###   ########.fr       */
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
char	*gnl_ft_strchr(const char *string, int c);
char	*ft_line(char *buff);
void	*gnl_ft_calloc(size_t count, size_t size);
char	*ft_next(char *buffer);
size_t	gnl_ft_strlen(const char *str);

#endif
