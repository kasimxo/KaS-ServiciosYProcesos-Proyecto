#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_LINE 80 /* Longitud máxima del comando */

int main() {
    char input[MAX_LINE];
    int should_run = 1;

    while (should_run) {
        printf("MiShell> ");
        fflush(stdout);
	
        fgets(input, MAX_LINE, stdin);

        // Reemplazar el salto de línea con un terminador de cadena
        input[strlen(input) - 1] = '\0';
	
	//MODIFICACION:
	//Incluimos un log para que el usurio pueda ver qué comandos ha introducido 
	FILE* ComandosLog;
	ComandosLog = fopen("ComandosLog", "a");
	//Vamos a incluir también la hora en la que ha introducido el comando
	time_t hora;
	time(&hora);
	struct tm tm = *localtime(&hora);
	char buffer[25];
	int cont;
	cont = snprintf(buffer, 25, "\n%d/%d/%d - ",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
	
	fwrite(buffer, 1, cont, ComandosLog);
	
	fwrite(input,1, sizeof(input), ComandosLog);
	fclose(ComandosLog);	

        
	// Salir si el usuario ingresa "exit"
        if (strcmp(input, "exit") == 0) {
            should_run = 0;
	//MODIFICACION:
	//Añadimos la posibilidad de que el usuario consulte su archivo ComandosLog
       	} else if (strcmp(input, "log") == 0) {
		system("cat ComandosLog");
	} else {
        	pid_t pid = fork();

            if (pid == 0) {
                // Este es el proceso hijo
                char *args[MAX_LINE / 2 + 1]; // Argumentos del comando
                char *token = strtok(input, " ");

                int i = 0;
                while (token != NULL) {
                    args[i] = token;
                    token = strtok(NULL, " ");
                    i++;
                }
                args[i] = NULL; // Marcar el final de los argumentos

                execvp(args[0], args);
                exit(0);
            } else {
                // Este es el proceso padre
                wait(NULL);
            }
        }
    }
    return 0;
}

