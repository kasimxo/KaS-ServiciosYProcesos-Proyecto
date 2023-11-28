#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80

/*
El proposito de este test unitario es evaluar si se ejecuta correctamente un comando:
Exito -> El comando se ha ejecutado correctamente
Fallo -> El comando no se ha ejecutado correctamente
*/


int ejecutarComando(char* comando) {
    pid_t pid = fork();

    if (pid == 0) {
        char *args[MAX_LINE / 2 + 1];
        char *token = strtok(comando, " ");

        int i = 0;
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL;

        execvp(args[0], args);
        exit(0);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        return -1; // Error al crear el proceso hijo
    }

    return 0; // Éxito
}

/*
MODIFICACION:
Permitimos que al ejecutar este test el usuario pueda introducir un comando como argumento para la prueba

*/

int main(int argc, char* argv[]) {
    	char *comando_ls[1];
	int resultado;
	if(argc>1) {
		resultado = ejecutarComando(argv[1]);
		comando_ls[0] = argv[1];
	} else {
		comando_ls[0] = "ls";
		resultado = ejecutarComando(comando_ls);
	}
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando '%s' se ejecutó correctamente.\n", comando_ls[0]);
    } else {
        printf("Prueba 1: Fallida - Error al ejecutar el comando '%s'.\n", comando_ls[0]);
    }

    return 0;
}

