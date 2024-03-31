/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kluna-bo <kluna-bo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:01:34 by kluna-bo          #+#    #+#             */
/*   Updated: 2024/03/29 16:59:17 by kluna-bo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	new_token(char *input, int start, int end, t_token **token)
{
	t_token	new;

	new.next = NULL;
	new.value = ft_substr(input, start, end);
	*token = &new;
}

void	add_token(char *input, int start, int end, t_token **token)
{
	t_token	new;
	t_token	*lst;

	lst = *token;
	while (lst->next)
		lst = lst->next;
	new.next = NULL;
	new.value = ft_substr(input, start, end);
	lst->next = &new;
}

int	is_special(char c)
{
	if (c == '|' || c == ' ' || !c)
		return (1);
	return (0);
}
/*Por algun motivo solo imprimer la utima palabra pero debería imprimir la primera*/
int	lexer(char *input, t_data *data)
{
	t_token	*token;
	int		i;
	int		start;
	int		end;

	token = NULL;
	i = -1;
	while (input[++i])
	{
		if (!is_special(input[i]))
		{
			start = i;
			while (!is_special(input[++i]))
			{
				end = i;
			}
			if (!token)
			{
			//	printf("start: %i, end: %i, %c, %c\n", start, end, input[start], input[end]);
				new_token(input, start, end + 1, &token);
			}
			else
			 	add_token(input, start, end, &token);
		}
	}
	if (!token)
	{
		return (1);
	}
	data->token = token;
	printf("%s\n", data->token->value);
	return (1);
}

/*TODO por algun motivo al poner adios el history falla*/
int	main(int argc, char *argv[], char *env[])
{
	static char	*input;
	t_data		data;

	while (1)
	{
		if (input)
		{
			free (input);
			input = (char *) NULL;
		}
		input = readline("\x1b[34m""Minishell: ""\x1b[0m");
		if (!strcmp(input, "exit"))
			break ;
		if (input && *input)
			add_history (input);
		if (!lexer(input, &data))
			exit(EXIT_FAILURE);
		// printf("%s\n", data.token->value);
	}
	return (0);
}

/*int es_comando_valido(char *comando) {
    // Aquí podrías implementar una lógica para verificar si el comando es válido
    // Por ejemplo, verificar si el archivo ejecutable existe en el sistema
    if (access(comando, X_OK) != -1) {
        return 1;
    }
    return 0;
}
int main(int argc, char *argv[], char *env[])
{
    static char *input;

    while(1)
    {
        input = readline("\x1b[34m""Minishell:""\x1b[0m");
        if(!strcmp(input, "exit"))
            break ;
        //printf("%s\n",input);
 		if (!es_comando_valido(input)) {
            printf("Comando inválido: %s\n", input);
  //          return 1;
        }
//		if (execve(input, (char **){"touch", "test.txt", NULL}, env) == -1) {
  //      perror("Error al ejecutar el comando");
    //    return 1;
//    }		
    }
}*/
