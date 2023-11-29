#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80
//Declaramos los resultados del test
#define EXITO 0
#define FALLO 1
#define ERROR -1

int status;

/*
El proposito de este test unitario es evaluar si se ejecuta correctamente un comando:
Exito -> El comando se ha ejecutado correctamente
Fallo -> El comando no se ha ejecutado correctamente
Para utilizar este test se puede ejecutar sin introducir ningún argumento, o introducir un comando como argumento.
Es necesario que el comando introducido sea un comando válido
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
	//MODIFICACION
	//Recogemos el resultado de la ejecución del comando para evaluar si ha dado algún tipo de error o no
       	int resultado = execvp(args[0], args);

	if(resultado == 0) {
		exit(EXITO);
	} else {
		exit(FALLO);
	}
    } else if (pid > 0) {
        waitpid(pid,&status,0);
	if(status == 0) {
		return EXITO;
	} else {
		return FALLO;
	}
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
		resultado = ejecutarComando(comando_ls[0]);
	}
    if (resultado == 0) {
        printf("Prueba 1: Pasada - El comando '%s' se ejecutó correctamente.\n", comando_ls[0]);
    } else {
        printf("Prueba 1: Fallida - Error al ejecutar el comando '%s'.\n", comando_ls[0]);
    }

    return 0;
}

