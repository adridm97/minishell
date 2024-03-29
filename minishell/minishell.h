/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/03/29 16:59:17 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define SPACE		0 // ' '
# define PIPE		1 // |
# define MAJOR		2 // >
# define MINOR		3 // <
# define D_MAJOR	4 // >>
# define D_MINOR	5 // <<
# define NO_QUOTE	6 // string sin comillas
# define QUOTE		7 // string con comillas simples
# define D_QUOTE	8 // string con comillas dobles

# define ERROR		0



typedef struct s_mini
{
	char			*path;
	struct s_error	*errors;
	struct s_token	*token;
}	t_mini;

typedef struct s_error
{
	char	*error;
	int		is_error;
}	t_error;

typedef struct s_redir
{
	struct s_token	*token;
	char			*comand;
	char			**args;
	struct s_data	*next;
}	t_redir;

/*
Structure:
	int				*key;
	char			*value;
	struct s_token	*next;
	struct s_data	*data;
-cuando encontramos un caracter especial la key sera special y el value 
	la macro del caracter
-cuando sea un comando en key pondremos el comando y en value la continuacion 
	hasta el caracter especial
*/
typedef struct s_token
{
	int				key;
	char			*value;
	struct s_token	*next;
}	t_token;

// Structure:
	// t_token			*token;
	// char			*comand;
	// char			**args;
	// struct s_data	*next;
// args ha de uncluir NULL su ultima posicion, 
//	esta struct es la utilizada por Adrian
typedef struct s_data
{
	t_token			*token;
	char			*comand;
	char			**args;
	struct s_data	*next;
	struct s_redir	*redir;
}	t_data;

#endif
