/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/04/24 23:24:10 by aduenas-         ###   ########.fr       */
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
# include "libft/get_next_line.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

// Signals
# define CTRL_C SIGINT
# define CTRL_SLASH SIGQUIT

// Mode
# define DEFAULT 0
# define CHILDS 1

# define SPACES		0 // ' '
# define PIPE		1 // |
# define MAJOR		2 // >
# define MINOR		3 // <
# define QUOTE		4 // string con comillas simples
# define D_QUOTE	5 // string con comillas dobles
# define NO_QUOTE	6 // string sin comillas
# define D_MAJOR	7 // >>
# define D_MINOR	8 // <<

# define ERROR		0

/*	char			*path;
	struct s_error	*errors;
	struct s_token	*token;*/
typedef struct s_mini
{
	char			*path;
	struct s_error	*errors;
	struct s_token	*token;
}	t_mini;

/*	char	*error;
	int		is_error;*/
typedef struct s_error
{
	char	*error;
	int		is_error;
}	t_error;

/*	int				type;
	char			*path;
	struct s_redir	*next;*/
typedef struct s_redir
{
	int				type;
	char			*path;
	struct s_redir	*next;
}	t_redir;

/*
typedef struct s_heredoc
{
	int					type;
	char				*file;
	struct s_heredoc	*next;
}	t_heredoc;
*/

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
	int				index;
	int				type;
	char			value;
	struct s_token	*next;
}	t_token;

// Structure:
	// char			*comand;
	// char			**args;
	// struct s_data	*next;
	// struct s_redir	*redir;
	// struct s_error	error;
// args ha de uncluir NULL su ultima posicion, 
//	esta struct es la utilizada por Adrian
typedef struct s_data
{
	char			*comand;
	char			*path;
	char			**args;
	struct s_data	*next;
	struct s_redir	*redir;
	struct s_error	error;
}	t_data;

// lexer.c
int		new_token(char c, int type, t_token **token, int index);
int		add_token(char c, int type, t_token **token);
t_data	*lexer(char *input, t_data *data, char **env);
int		typeing(char c, char *base);
void	is_error(t_error *error);
void	free_data(t_data **data);
int		init_data(t_data **data);
void	free_redir(t_redir **redir);


// split things
char	**special_split(char const *s);
int		split_token(t_token *token, char **env, t_data **data);

//executor.c
int		is_valid_command(t_data *data);

//split_token
void	is_expandsor(t_token **token, char **str, char **env);
void	is_expandsor_str_simple(t_token **token, char **str, char **env);
int		switch_case(t_token **token, char **env, t_data **data, char **str);
int		add_args(char ***arg, char **str);
int		add_last_data(t_data **data, char **str);


void	execute_command(t_data *data, char *command_path);
void	handle_redir(t_data *data);
int	heredoc(t_data *data);

// Regular Colors
# define BLACK "\x1b[0m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define WHITE "\x1b[37m"

#endif
