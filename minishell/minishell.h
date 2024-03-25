#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

//TODO ahora son int
# define PIPE		"|"
# define MAJOR		">"
# define MINOR		"<"
# define D_MAJOR	">>"
# define D_MINOR	"<<"
# define DOLLAR		"$"

typedef struct s_mini{
    char *path;
    t_error *errors;
	t_token *token;
} t_mini;

typedef struct s_error{
	char	*error;
	int		is_error;
} t_error;

// cuando encontramos un caracter especial la key sera special y el value la macro del caracter
// cuando sea un comando en key pondremos el comando y en value la continuacion hasta el caracter especial
typedef struct s_token{
	int		*key;
	char	*value;
	t_token	*next;
	t_data	*data;
} t_token;

// args ha de uncluir NULL su ultima posicion
typedef struct	s_data{
	t_token	*token;
	char	*comand;
	char	*args[];
} t_data;

#endif
