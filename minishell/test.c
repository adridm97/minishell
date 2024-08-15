#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void run_command(const char *shell, const char *command, char *result, size_t result_size) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "%s -c \"%s\"", shell, command);

    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        snprintf(result, result_size, "Error: Failed to run command\n");
        return;
    }

    char buffer[128];
    result[0] = '\0';  // Inicializar el buffer de resultados
    char last_line[128] = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Si la línea contiene "exit", no concatenamos ni la línea ni la anterior
        if (strstr(buffer, "exit") != NULL) {
            // No agregamos "last_line" ni "buffer" al resultado.
            last_line[0] = '\0';  // Limpiar el contenido de last_line
            continue;
        }

        // Concatenar la línea anterior si no es la que precede a "exit"
        if (last_line[0] != '\0') {
            strncat(result, last_line, result_size - strlen(result) - 1);
        }

        // Guardar la línea actual en last_line
        strncpy(last_line, buffer, sizeof(last_line) - 1);
        last_line[sizeof(last_line) - 1] = '\0';  // Asegurarse de que last_line esté terminada en nulo
    }

    // Concatenar la última línea, si no fue eliminada
    if (last_line[0] != '\0') {
        strncat(result, last_line, result_size - strlen(result) - 1);
    }

    pclose(pipe);
}

void run_custom_shell(const char *shell, const char *command, char *result, size_t result_size) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "echo \"%s\" | %s", command, shell);

    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        snprintf(result, result_size, "Error: Failed to run command\n");
        return;
    }

    char buffer[128];
    result[0] = '\0';  // Inicializar el buffer de resultados
    char last_line[128] = "";
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        // Si la línea contiene "exit", no concatenamos ni la línea ni la anterior
        if (strstr(buffer, "exit") != NULL) {
            // No agregamos "last_line" ni "buffer" al resultado.
            last_line[0] = '\0';  // Limpiar el contenido de last_line
            continue;
        }

        // Concatenar la línea anterior si no es la que precede a "exit"
        if (last_line[0] != '\0') {
            strncat(result, last_line, result_size - strlen(result) - 1);
        }

        // Guardar la línea actual en last_line
        strncpy(last_line, buffer, sizeof(last_line) - 1);
        last_line[sizeof(last_line) - 1] = '\0';  // Asegurarse de que last_line esté terminada en nulo
    }

    // Concatenar la última línea, si no fue eliminada
    if (last_line[0] != '\0') {
        strncat(result, last_line, result_size - strlen(result) - 1);
    }

    pclose(pipe);
}

int main() {
    const char *commands[] = {
        "ls | grep a",
        "echo 'Hello, World!'",
        "pwd",
        "echo $USER",
        "echo \"$USER\"",
        "echo '$USER'",
        "echo \"Hello, $USER\"",
        "echo 'Hello, $USER'",
        "echo \"\"$USER\" $USER\"",
        NULL // Este NULL marca el final de la lista de comandos
    };

    char minishell_result[1024];
    char bash_result[1024];
    int i = -1;
    while (commands[++i] != NULL)
    {
        const char *command = commands[i];

        // Ejecutar el comando en minishell
        run_custom_shell("./minishell", command, minishell_result, sizeof(minishell_result));

        // Ejecutar el comando en Bash
        // run_command("/bin/bash", command, bash_result, sizeof(bash_result));

        // Imprimir los resultados para cada comando
        printf("----------------%s----------------\n", command);
        // printf("\x1b[32m""++++Minishell++++\n""\x1b[0m");
        printf("%s", minishell_result);
        // printf("\x1b[35m""++++Bash++++\n""\x1b[0m");
        // printf("%s", bash_result);
    }

    return 0;
}