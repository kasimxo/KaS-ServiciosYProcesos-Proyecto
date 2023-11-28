#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80

//Declaramos esta variable para poder guardar el resultado del proceso hijo
int status;
//Declaramos la función de ejecutarComando
int ejecutarComando();

/*
ORIGINAL:
int ejecutarComando(char+ comando) { ... }
Por limpieza definimos la función tras el main
*/

/*
El propósito de este test unitario es evaluar el resultado de la ejecución de un comando inventado
Exito -> Si el comando inventado falla
Fallo -> Si el comando inventado se ejecuta correctamente
*/

/*
ORIGINAL:
En un principio se evaluaba la ejecución del comando como correcta independientemente de que fuera inventado o no, lo que provocaba que el resultado de todos los test fuera fallo aún cuando no debía serlo.
*/

int main() {
    	//char comando_inexistente[] = "xyzabc";
    	char comando_inexistente[] = "datxxxxxe";
    	int resultado = ejecutarComando(comando_inexistente);
    	/*
	ORIGINAL:
	if (resultado == -1) {
    	Esta línea pretende evaluar si hemos recibido un código de error pero no funciona correctamente
	*/
	//Si todo ha ido bien el resultado será 0, por lo que evaluamos si ha habido algún error:
	if (resultado != 0) {
        	printf("Prueba 2: Pasada - Error al ejecutar comando inexistente.\n");
    	} else {
        	printf("Prueba 2: Fallida - El comando inexistente se ejecutó correctamente.\n");
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

	//Guardamos el resultado de la ejecución del comando
        int resultado = execvp(args[0], args);
	//Evaluamos si el resultado de la ejecución del comando ha sido correcta o no
	if(resultado == 0) {
		//Si la ejecución del comando ha salido bien devolvemos 0
		exit(0);
	} else {
		//Si ha fallado devolvemos un código de error
		exit(-1);
	}
    } else if (pid > 0) {
	/*
	ORIGINAL:
	wait(NULL);
	Este código espera a que el proceso hijo termine, pero ignora completamente el resultado de ese proceso hijo
	*/
	//Aquí esperamos a que el proceso hijo termine, pero además guardamos el resultado en una variable
	waitpid(pid,&status, 0);
	//Devolvemos el resultado del proceso hijo
	return status;
    } else {
        return -1; // Error al crear el proceso hijo
    }
	
    return 0; // Éxito
}
