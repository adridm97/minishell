/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/03/29 10:54:19 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

//TODO ahora son int
# define PIPE		0 // |
# define MAJOR		1 // >
# define MINOR		2 // <
# define D_MAJOR	3 // >>
# define D_MINOR	4 // <<
# define D_QUOTE	5 // "
# define QUOTE		6 // '
# define SPACE		7 // ' '


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

/*
-cuando encontramos un caracter especial la key sera special y el value 
	la macro del caracter
-cuando sea un comando en key pondremos el comando y en value la continuacion 
	hasta el caracter especial
*/
typedef struct s_token
{
	int				*key;
	char			*value;
	struct s_token	*next;
	struct s_data	*data;
}	t_token;

// args ha de uncluir NULL su ultima posicion, 
//	esta struct es la utilizada por Adrian
typedef struct s_data
{
	t_token			*token;
	char			*comand;
	char			**args;
	struct s_data	*next;
}	t_data;

#endif
