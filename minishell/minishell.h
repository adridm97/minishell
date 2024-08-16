/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kevin <kevin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/08/16 10:09:15 by kevin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <limits.h>
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
# include <bits/sigaction.h>

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
# define SA_RESTART   0x10000000
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
	int				type;
	char			*path;
	struct s_redir	*next;
}	t_redir;

typedef struct s_exec_vars
{
	int		input_fd;
	int		heredoc_fd;
	int		heredoc_processed;
	int		last_pid;
}	t_exec_vars;

typedef struct s_token
{
	int				index;
	int				type;
	char			value;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char			*comand;
	char			*path;
	char			**args;
	struct s_data	*next;
	struct s_redir	*redir;
	struct s_error	error;
	char			**env;
	int				heredoc;
	int				is_ex;
	int				exp;
	int				pipe;
	int				*stat_code;
}	t_data;

//checkers.c
int		check_error(t_token *token, char type, int flag);
int		check_gramathic(t_token *token, t_error *error, char flag, char type);
void	is_error(t_error *error);
int		check_closed(t_token *token, t_error *error);
int		typeing(char c, char *base);

//token.c
int		new_token(char c, int type, t_token **token, int index);
int		add_token(char c, int type, t_token **token);

// lexer.c
t_data	*lexer(char *input, t_data **data, char **env, int *sce);
int		init_data(t_data **data, char **env, int *sce);
int		count_lines(char *file, t_data **data);

//clean.c
void	free_redir(t_redir **redir);
void	free_args(char ***args);
void	free_data(t_data **data);
void	clean_env(char ***env, int i);
void	free_token(t_token **token);

// minishell.c
int		file_exist(char *file);

// special_split.c
char	**special_split(char const *s);

//status.c
void	wait_for_remaining_processes(int last_pid, t_data **data);
void	sc_error(int sce, t_data **data);
void	update_heredoc_status(t_data **data, pid_t pid, int *processed);
void	update_status(pid_t pid, int *last_pid, int status, t_data **data);

//print.c
void	print_redir(t_redir *redir);
void	print_data(t_data *data);
void	print_export(t_data *data, char *str);
void	print_env(t_data *data, char *str);
void	print_env_env(char **env, char *str);

//mat.c
char	**ft_matadd(char ***mat, char *str, t_data **data);
int		ft_matsize(char **mat);
char	**ft_matcpy(char **mat);

//handle.c
void	handle_missing_command(t_data *data, int heredoc_processed);
void	handle_dups(int fd, t_redir *redir, t_data *data, \
int heredoc_processed);
void	handle_redir(t_data *data, int heredoc_processed);
void	handle_parent_process(t_exec_vars *vars, int fd[2], \
pid_t pid, t_data **data);
void	handle_child_pipes(t_data **current, t_exec_vars *vars, int fd[2]);

//handle2.c
void	handle_child_process(t_data **ddata, char *command_path, int processed);
void	handle_output_redirection(t_data *current, int fd[2]);
void	handle_input_redirection(int *input_fd);
void	handle_heredoc(t_data *current, t_exec_vars *vars);

//handle_fd.c
void	update_input_fd(int *input_fd, int fd[2], t_data *current);
void	close_heredoc_fd(int *heredoc_fd);
void	close_input_fd(int *input_fd);

//executor2.c
int		search_and_execute_command(t_data **data, char *path, \
int heredoc_processed);
int		find_command_in_paths(t_data **data, char **token, \
int heredoc_processed);
char	*build_command_path(char *dir, char *comand);

//executor.c
int		is_valid_command(t_data **data, int heredoc_processed);
int		is_valid_file(char *filename, int fd, char *check, t_data **data);
void	execute_command(t_data **ddata, char *command_path, \
int heredoc_processed);

//builtins.c
void	switch_builtin(t_data **ddata);
int		is_builtin(char *comand);

//export.c
void	b_export(t_data **data);
void	process_argument(t_data *cdata, char *arg);
void	finalize_export(t_data *cdata);
int		is_valid_key(char *key);
char	*extract_key(char *arg, t_data **data);

//pipes.c
void	execute_pipeline(t_data **data);
void	initialize_pipe_vars(t_exec_vars *vars);
int		is_pipe(t_token **token, t_data **data, char **str, int *sce);

//cd.c
void	b_cd(t_data **data, char *home);
void	init_cd(char **last_pwd, char **pwd, int size, t_data *data);
void	find_home(char **res, char **pwd, t_data *data);

//args.c
int		count_args(char **args);
void	free_args_triple(char ***arg);
int		add_args(char ***arg, char **str);
int		exp_var(int *exp, int type);

//split_token
int		is_expandsor(t_token **token, char **str, char **env, t_data **data);
void	is_expandsor_str_simple(t_token **token, char **str, char **env);
int		switch_case(t_token **token, char **env, t_data **data, char **str);
int		add_last_data(t_data **data, char **str);
int		split_token(t_token *token, char **env, t_data **data, int *sce);

//redirs.c
void	redirs(t_data **c_data, char **str, t_redir **redir, t_redir **l_redir);
int		init_redir(t_token **token, t_data **data, char **env, int type);
int		is_redir_input(t_token **token, t_data **data, char **str, char **env);
int		is_redir_output(t_token **token, t_data **data, char **str, char **env);
void	switch_case_redir(t_token **token, char **str, char **env, t_data **data);

//redirs2.c
int		set_redir(int type, t_redir **redir, t_data **data);
void	manage_redirs(t_data **data, t_redir **credir, char *str);

//heredoc.c
int		heredoc(t_redir	*aux, t_data *data);
int		expand_line(char **expanded_line, char **line, int fd, t_data *data);
char	*heredoc_tokenizer(char *str, t_data *data);
int		manage_token_heredoc(t_token **token, char **input, int *i, t_data **data);
int		token_to_str(t_token **token, char **res, t_data **data);

//signals.c
void	handle_sigint_heredoc(int sig);
void	wait_signal(int i);
void	child_handler(int signal);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

//env.c
int		set_env(char *key, char *val, char ***env, t_data **data);
char	**create_env_first(char **cenv, t_data **data);
int		index_env_env(char **env, char *str);
int		save_env(t_data *data);
char	**set_env_i(char ***env, t_data **data);

//env2.c
void	check_shlvl(int *n);
void	ft_set_shell(char *env[], char ***mat, t_data **data);
void	ft_handle_env_file(char ***mat, t_data **data);
void	handle_env_file(t_data **data);
int		get_file_env(int fd, t_data **data);

//env3.c
char	**get_env_file(int fd, t_data **data);
void	update_env(t_data *cdata, char *key, char *arg, int i);
int		index_env(t_data *data, char *str);
void	b_env(t_data *data);
int		create_env(t_data **data, char **env);

//echo.c
void	b_echo(t_data *data);
void	print_args_echo(t_data *data, int *i, int fd);
int		ft_is_n(char **str, int *j);

//exit.c
void	b_exit(t_data **data);
void	check_numeric_argument(char *arg, t_data **data);
void	try_max_num(char *arg, t_data **data);

//unset.c
void	b_unset(t_data *data);
char	**ft_mat_rem_index(char ***mat, int index);
void	is_space(t_token **token, t_data **data, char **str);

//utils.c
void	ft_free_resources(t_data **data, char **input, char ***mat, int *sce);
void	handle_input(t_data **data, char ***mat, char *input, int *sce);
char	*get_input(void);
int		is_special_string(char *c, char *comp);
int		handle_status_code(char **str, char *key, t_data **data);

//utils2.c
char	*charstr(char c);
char	*new_str(char **str, char c);
int		is_special(char c, char *comp);
void	is_simple_string(t_token **token, char **env, char **str);
void	is_double_string(t_token **token, char **env, char **str, t_data **data);

//key.c
int		take_key(t_token **token, char **key, char *str);
char	*key_to_res(char **key, char **env);
int		handle_key(char **str, char **env, char **key, t_data **data);
int		handle_string(char **str);
int		process_token(t_token **token, char **str, char **env, t_data **data);

//pwd.c
int		check_pwd(t_data *data);
void	find_oldpwd(char **res, char **pwd, t_data *data);
void	find_pwd(char **res, t_data **data);

//pwd2.c
void	b_pwd(t_data *data);
void	managing_env(char **res, int i, char **last_pwd, t_data **data);
void	ft_oldpwd(t_data **data, char **last_pwd, char **res);
void	ft_pwd(char **pwd, char **res, t_data **data, int size);

void	ft_free_char(char **f);
void	sc_error_int(int err, int *sce);
int	save_env_mat(char **env, int *stat_code);

// Regular Colors
# define BLACK "\x1b[0m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define WHITE "\x1b[37m"

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
# define SC_HEREDOC												150
#endif