


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