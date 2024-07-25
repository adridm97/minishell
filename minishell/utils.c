


#include "minishell.h"

void	ft_free_resources(t_data **data, char **input, char ***mat)
{
	if (*data)
	{
		free_data(data);
		*data = NULL;
	}
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
	if (*mat)
		clean_env(mat, -1);
}

void	ft_handle_env_file(char ***mat)
{
	int	fd;
	fd = open("/tmp/env.env", O_RDONLY);
	if (is_valid_file("/tmp/env.env", fd, "R"))
		sc_error(SC_PERMISSION_DENIED);
	if (mat && fd >= 0)
	{
		*mat = get_env_file(fd);
		close(fd);
		unlink("/tmp/env.env");
	}
}

