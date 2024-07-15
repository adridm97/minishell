/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/07/15 22:01:44 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "libft/libft.h"
# include "libft/get_next_line.h"
# include "libs/readline-4.3/readline.h"
# include "libs/readline-4.3/history.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>

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
	char			**env;
}	t_data;

// lexer.c
int		new_token(char c, int type, t_token **token, int index);
int		add_token(char c, int type, t_token **token);
t_data	*lexer(char *input, t_data **data, char **env);
int		typeing(char c, char *base);
void	is_error(t_error *error);
void	free_data(t_data **data);
int		init_data(t_data **data, char **env);
void	free_redir(t_redir **redir);
void	free_args(char **args);
void	free_token(t_token **token);
char	**get_env_file(int fd);
void	print_data(t_data *data);
void	clean_env(char ***env, int i);

// minishell.c
int		save_env(t_data *data);
int		file_exist(char *file);

// split things
char	**special_split(char const *s);
int		split_token(t_token *token, char **env, t_data **data);

//executor.c
char	**ft_matadd(char ***mat, char *str);
void	execute_pipeline(t_data *data);
int		is_valid_command(t_data *data);
int		ft_matsize(char **mat);
void	print_env(t_data *data, char *str);
void	print_env_env(char **env, char *str);
int	index_env(t_data *data, char *str);
void sc_error(int sce);

//split_token
int	is_expandsor(t_token **token, char **str, char **env);
void	is_expandsor_str_simple(t_token **token, char **str, char **env);
int		switch_case(t_token **token, char **env, t_data **data, char **str);
int		add_args(char ***arg, char **str);
int		add_last_data(t_data **data, char **str);
char	*key_to_res(char **key, char **env);
int		is_special(char c, char *comp);
void	execute_command(t_data **ddata, char *command_path);
void	handle_redir(t_data *data);
int		heredoc(t_data *data);
char	*charstr(char c);
char	*new_str(char **str, char c);

//handlers.c
void	setup_signal_handlers();

// Regular Colors
# define BLACK "\x1b[0m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define WHITE "\x1b[37m"

extern int	g_stat_code;

# define SC_SUCCESS												0
# define SC_OPERATION_NOT_PERMITTED								1
# define SC_NO_SUCH_FILE_OR_DIRECTORY							2
# define SC_NO_SUCH_PROCESS										3
# define SC_INTERRUPTED_SYSTEM_CALL								4
# define SC_INPUTOUTPUT_ERROR									5
# define SC_NO_SUCH_DEVICE_OR_ADDRESS							6
# define SC_ARGUMENT_LIST_TOO_LONG								7
# define SC_EXEC_FORMAT_ERROR									8
# define SC_BAD_FILE_DESCRIPTOR									9
# define SC_NO_CHILD_PROCESSES									10
# define SC_RESOURCE_TEMPORARILY_UNAVAILABLE					11
# define SC_CANNOT_ALLOCATE_MEMORY								12
# define SC_PERMISSION_DENIED									13
# define SC_BAD_ADDRESS											14
# define SC_BLOCK_DEVICE_REQUIRED								15
# define SC_DEVICE_OR_RESOURCE_BUSY								16
# define SC_FILE_EXISTS											17
# define SC_INVALID_CROSSDEVICE_LINK 							18
# define SC_NO_SUCH_DEVICE										19
# define SC_NOT_A_DIRECTORY										20
# define SC_IS_A_DIRECTORY										21
# define SC_INVALID_ARGUMENT									22
# define SC_TOO_MANY_OPEN_FILES_IN_SYSTEM						23
# define SC_TOO_MANY_OPEN_FILES									24
# define SC_INAPPROPRIATE_IOCTL_FOR_DEVICE						25
# define SC_TEXT_FILE_BUSY										26
# define SC_FILE_TOO_LARGE										27
# define SC_NO_SPACE_LEFT_ON_DEVICE								28
# define SC_ILLEGAL_SEEK										29
# define SC_READONLY_FILE_SYSTEM 								30
# define SC_TOO_MANY_LINKS										31
# define SC_BROKEN_PIPE											32
# define SC_NUMERICAL_ARGUMENT_OUT_OF_DOMAIN					33
# define SC_NUMERICAL_RESULT_OUT_OF_RANGE						34
# define SC_RESOURCE_DEADLOCK_AVOIDED							35
# define SC_FILE_NAME_TOO_LONG									36
# define SC_NO_LOCKS_AVAILABLE									37
# define SC_FUNCTION_NOT_IMPLEMENTED							38
# define SC_DIRECTORY_NOT_EMPTY									39
# define SC_TOO_MANY_LEVELS_OF_SYMBOLIC_LINKS					40
# define SC_NO_MESSAGE_OF_DESIRED_TYPE							42
# define SC_IDENTIFIER_REMOVED									43
# define SC_CHANNEL_NUMBER_OUT_OF_RANGE							44
# define SC_LEVEL_2_NOT_SYNCHRONIZED							45
# define SC_LEVEL_3_HALTED										46
# define SC_LEVEL_3_RESET										47
# define SC_LINK_NUMBER_OUT_OF_RANGE							48
# define SC_PROTOCOL_DRIVER_NOT_ATTACHED						49
# define SC_NO_CSI_STRUCTURE_AVAILABLE							50
# define SC_LEVEL_2_HALTED										51
# define SC_INVALID_EXCHANGE									52
# define SC_INVALID_REQUEST_DESCRIPTOR							53
# define SC_EXCHANGE_FULL										54
# define SC_NO_ANODE											55
# define SC_INVALID_REQUEST_CODE								56
# define SC_INVALID_SLOT										57
# define SC_BAD_FONT_FILE_FORMAT								59
# define SC_DEVICE_NOT_A_STREAM									60
# define SC_NO_DATA_AVAILABLE									61
# define SC_TIMER_EXPIRED										62
# define SC_OUT_OF_STREAMS_RESOURCES							63
# define SC_MACHINE_IS_NOT_ON_THE_NETWORK						64
# define SC_PACKAGE_NOT_INSTALLED								65
# define SC_OBJECT_IS_REMOTE									66
# define SC_LINK_HAS_BEEN_SEVERED								67
# define SC_ADVERTISE_ERROR										68
# define SC_SRMOUNT_ERROR										69
# define SC_COMMUNICATION_ERROR_ON_SEND							70
# define SC_PROTOCOL_ERROR										71
# define SC_MULTIHOP_ATTEMPTED									72
# define SC_RFS_SPECIFIC_ERROR									73
# define SC_BAD_MESSAGE											74
# define SC_VALUE_TOO_LARGE_FOR_DEFINED_DATA_TYPE				75
# define SC_NAME_NOT_UNIQUE_ON_NETWORK							76
# define SC_FILE_DESCRIPTOR_IN_BAD_STATE						77
# define SC_REMOTE_ADDRESS_CHANGED								78
# define SC_CAN_NOT_ACCESS_A_NEEDED_SHARED_LIBRARY				79
# define SC_ACCESSING_A_CORRUPTED_SHARED_LIBRARY				80
# define SC_LIB_SECTION_IN_AOUT_CORRUPTED 						81
# define SC_ATTEMPTING_TO_LINK_IN_TOO_MANY_SHARED_LIBRARIES		82
# define SC_CANNOT_EXEC_A_SHARED_LIBRARY_DIRECTLY				83
# define SC_INVALID_OR_INCOMPLETE_MULTIBYTE_OR_WIDE_CHARACTER	84
# define SC_INTERRUPTED_SYSTEM_CALL_SHOULD_BE_RESTARTED			85
# define SC_STREAMS_PIPE_ERROR									86
# define SC_TOO_MANY_USERS										87
# define SC_SOCKET_OPERATION_ON_NONSOCKET						88
# define SC_DESTINATION_ADDRESS_REQUIRED						89
# define SC_MESSAGE_TOO_LONG									90
# define SC_PROTOCOL_WRONG_TYPE_FOR_SOCKET						91
# define SC_PROTOCOL_NOT_AVAILABLE								92
# define SC_PROTOCOL_NOT_SUPPORTED								93
# define SC_SOCKET_TYPE_NOT_SUPPORTED							94
# define SC_OPERATION_NOT_SUPPORTED								95
# define SC_PROTOCOL_FAMILY_NOT_SUPPORTED						96
# define SC_ADDRESS_FAMILY_NOT_SUPPORTED_BY_PROTOCOL			97
# define SC_ADDRESS_ALREADY_IN_USE								98
# define SC_CANNOT_ASSIGN_REQUESTED_ADDRESS						99
# define SC_NETWORK_IS_DOWN										100
# define SC_NETWORK_IS_UNREACHABLE								101
# define SC_NETWORK_DROPPED_CONNECTION_ON_RESET					102
# define SC_SOFTWARE_CAUSED_CONNECTION_ABORT					103
# define SC_CONNECTION_RESET_BY_PEER							104
# define SC_NO_BUFFER_SPACE_AVAILABLE							105
# define SC_TRANSPORT_ENDPOINT_IS_ALREADY_CONNECTED				106
# define SC_TRANSPORT_ENDPOINT_IS_NOT_CONNECTED					107
# define SC_CANNOT_SEND_AFTER_TRANSPORT_ENDPOINT_SHUTDOWN		108
# define SC_TOO_MANY_REFERENCES									109
# define SC_CONNECTION_TIMED_OUT								110
# define SC_CONNECTION_REFUSED									111
# define SC_HOST_IS_DOWN										112
# define SC_NO_ROUTE_TO_HOST									113
# define SC_OPERATION_ALREADY_IN_PROGRESS						114
# define SC_OPERATION_NOW_IN_PROGRESS							115
# define SC_STALE_FILE_HANDLE									116
# define SC_STRUCTURE_NEEDS_CLEANING							117
# define SC_NOT_A_XENIX_NAMED_TYPE_FILE							118
# define SC_NO_XENIX_SEMAPHORES_AVAILABLE						119
# define SC_IS_A_NAMED_TYPE_FILE								120
# define SC_REMOTE_IO_ERROR										121
# define SC_DISK_QUOTA_EXCEEDED									122
# define SC_NO_MEDIUM_FOUND										123
# define SC_OPERATION_CANCELED									125
# define SC_REQUIRED_KEY_NOT_AVAILABLE							126
# define SC_KEY_HAS_EXPIRED										127
# define SC_KEY_HAS_BEEN_REVOKED								128
# define SC_KEY_WAS_REJECTED_BY_SERVICE							129
# define SC_OWNER_DIED											130
# define SC_STATE_NOT_RECOVERABLE								131
# define SC_OPERATION_NOT_POSSIBLE_DUE_TO_RFKILL				132
# define SC_MEMORY_PAGE_HAS_HARDWARE_ERROR						133

#endif					