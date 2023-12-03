#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


#define MAX_LINE 80
//Declaramos los resultadoEjecutars del test
#define EXITO 0
#define FALLO 1
#define ERROR -1
//Declaramos esta variable para poder guardar el resultadoEjecutar del proceso hijo
int status;
int resultadoEjecutar;
int resultadoTimer;
//Declaramos la función de ejecutarComando
int ejecutarComando();
int crearHilo();
void *contador();
/*
ORIGINAL:
int ejecutarComando(char+ comando) { ... }
Por limpieza definimos la función tras el main
*/

/*
El propósito de este test unitario es evaluar el resultadoEjecutar de la ejecución de un comando inventado
Exito -> Si el comando inventado falla
Fallo -> Si el comando inventado se ejecuta correctamente
*/

/*
ORIGINAL:
En un principio se evaluaba la ejecución del comando como correcta independientemente de que fuera inventado o no, lo que provocaba que el resultadoEjecutar de todos los test fuera fallo aún cuando no debía serlo.
*/

int main(int argc, char* argv[]) {
    	char *comandoEvaluado[1];
	resultadoTimer=0;
	if(argc>1){
		comandoEvaluado[0] = argv[1];
    	} else {
		comandoEvaluado[0] = "xyzabc";
	}
	//MODIFICACION:
	//Creamos un hilo que contará hasta n
	//Si termina de contar antes de que se haya ejecutado el test, el resultado es fallido
	crearHilo();
	resultadoEjecutar = ejecutarComando(comandoEvaluado[0]);
	if(resultadoTimer!=0) {
		printf("Prueba 2: Fallida - El test ha tardado mas de la cuenta");
	} else{
		printf("Prueba 2: Pasada - El test ha tardado menos de la cuenta");
	}

	/*
	ORIGINAL:
	if (resultadoEjecutar == -1) {
    	Esta línea pretende evaluar si hemos recibido un código de error pero no funciona correctamente
	*/
	//Si todo ha ido bien el resultadoEjecutar será 0, por lo que evaluamos si ha habido algún error:
	if (resultadoEjecutar == FALLO) {
        	printf("Prueba 2: Pasada - Error al ejecutar comando inexistente \"%s\"\n",comandoEvaluado[0]);
    	} else if (resultadoEjecutar == EXITO) {
        	printf("Prueba 2: Fallida - El comando inexistente \"%s\" se ejecutó correctamente\n", comandoEvaluado[0]);
    	} else {
		printf("Prueba 2: Error - Se ha producido un error durante la ejecución del comando \"%s\"\n", comandoEvaluado[0]);
	}

    	return 0;
}

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
	
	/*
	Original:
	execvp(args[0], args);
	exit(0);
	*/

	//Guardamos el resultadoEjecutar de la ejecución del comando
        int resultadoEjecutar = execvp(args[0], args);
	//Evaluamos si el resultadoEjecutar de la ejecución del comando ha sido correcta o no
	if(resultadoEjecutar == EXITO) {
		//Si la ejecución del comando ha salido bien devolvemos 0
		exit(EXITO);
	} else {
		exit(ERROR);
	}
    } else if (pid > 0) {
	/*
	ORIGINAL:
	wait(NULL);
	Este código espera a que el proceso hijo termine, pero ignora completamente el resultadoEjecutar de ese proceso hijo
	*/
	//Aquí esperamos a que el proceso hijo termine, pero además guardamos el resultadoEjecutar en una variable
	waitpid(pid,&status, 0);
	//Devolvemos el resultadoEjecutar del proceso hijo
	if(status == EXITO) {
		return EXITO;
	} else {
		return ERROR;
	}
    } else {
        return ERROR; // Error al crear el proceso hijo
    }
	//Esta parte del código no debería ser accesible, por lo que devuelve error si se ha alcanzado
    return ERROR; // Error
}

int crearHilo() {
	pthread_t cont;
	pthread_create(&cont, NULL, contador,NULL);	
	return 0;
}

void *contador() {
	for(int i=0; i<200000; i++) {
	};
	if(resultadoEjecutar == NULL){
		resultadoTimer = 1;
	}
}
