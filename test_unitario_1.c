#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//Se incluye una librería para utilizar expresiones regulares
#include <regex.h>

#define MAX_LINE 80
//Declaramos los resultadoEjecutars del test
#define EXITO 0
#define FALLO 1
#define ERROR -1

int status;

int ejecutarComando();
int evaluarComando();

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
	//Recogemos el resultadoEjecutar de la ejecución del comando para evaluar si ha dado algún tipo de error o no
       	int resultadoEjecutar = execvp(args[0], args);

	if(resultadoEjecutar == 0) {
		exit(EXITO);
	} else {
		exit(FALLO);
	}
    } else if (pid > 0) {
        waitpid(pid,&status,0);
	printf("%d\n",status);
	if(status == 0) {
		return EXITO;
	} else {
		return FALLO;
	}
    } else {
        return ERROR; // Error al crear el proceso hijo
    }
	//No se debería alcanzar este código, por lo que se envía error
    return ERROR; // Éxito
}

/*
MODIFICACION:
Permitimos que al ejecutar este test el usuario pueda introducir un comando como argumento para la prueba

*/

int main(int argc, char* argv[]) {
    	char *comando[1];
	int resultadoEjecutar;
	int resultadoEvaluar;
	/*
	MODIFICACION:
	Como el propósito del test es evaluar la correcta ejecución de un comando, se incluye la función evaluarComando() que evalúa únicamente el primer carácter del comando para ver si es una letra o cualquier otro símbolo.
	Ejemplo I: evaluarComando("ls") -> Pasada porque evalúa 'l' y es letra
	Ejemplo II: evaluarComando("1s") -> Fallida porque evalúa '1' y no es una letra
	*/
	if(argc>1) {
		comando[0] = argv[1];
	} else {
		comando[0] = "ls";
	}
	resultadoEjecutar = ejecutarComando(comando[0]);
	printf("Vamos a evaluar el comando");
	resultadoEvaluar = evaluarComando(comando[0]);
    	if (resultadoEjecutar == EXITO) {
        	printf("Prueba 1: Pasada - El comando '%s' se ejecutó correctamente.\n", comando[0]);
	} else if(resultadoEjecutar == FALLO){
        	printf("Prueba 1: Fallida - Error al ejecutar el comando '%s'.\n", comando[0]);
	} else if (resultadoEjecutar == ERROR){
		printf("Prueba 1: Error - Error durante la ejecución de la prueba");
	}
	if (resultadoEvaluar == EXITO){
		printf("Prueba 2. Pasada - El comando '%s' es válido.",comando[0]);
	} else if (resultadoEvaluar == FALLO) {
		printf("Prueba 2: Fallida - El comando '%s' no es válido.", comando[0]);
	} else if (resultadoEvaluar == ERROR){
		printf("Prueba 2: Error - Error durante la ejecución de la prueba.");
	}

    return 0;
}

int evaluarComando(char* com){
	regex_t preg;
	char *expresion = "^[a-zA-Z][a-zA-Z]*";
	int compiladorExprRegular;
	compiladorExprRegular = regcomp(&preg, expresion, 0);
	
	if(0 != compiladorExprRegular){
		printf("\nHa ocurrido un fallo compilando la expresión regular: %d", EXIT_FAILURE);
		return ERROR;
	}


	int resultadoExpr;
	size_t nmatch = 2;
	regmatch_t pmatch[2];
	if(0 != (resultadoExpr = regexec(&preg, com, nmatch, pmatch,0))) {
		return FALLO;	
	} else {
		return EXITO;
	}
	return ERROR;
}
