#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_LINE 80 /* Longitud máxima del comando */
#define TRUE 1
#define FALSE 0

//Function declaration
void checkAlias();

//Variable declaration
char input[MAX_LINE];

int main() {
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

	checkAlias();

       	/*
	MODIFICACION:
	Le damos al usuario la posibilidad de añadir alias para los comandos
	Ejemplo: cls = clear
	Para acceder a esta funcionalidad el usuario debe introducir el comando "alias"
	*/
	if(strcmp(input, "alias") == 0) {
		//Para editar el archivo alias usamos el editor por defecto del sistema.
		//En mi caso VIM pero puede ser cualquier otro que haya definido el usuario
		system("$EDITOR Alias");	
        } else if (strcmp(input, "exit") == 0) {
	// Salir si el usuario ingresa "exit"
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

void checkAlias(){
	//Primero comprobamos si existe el archivo Alias y si no, lo creamos
	if(access("Alias", F_OK) != 0){
		//Al crear el archivo, le metemos instrucciones
		system("echo \"#Puedes escribir todos los atajos o alias que queieras\n#Para ello, debes seguir la estructura: alias = comando\n#También puedes comentar líneas utilizando '#' al principio de dicha línea\"> Alias");
	}
	FILE* fileAlias;
	fileAlias = fopen("Alias", "r");
	char linea[200];
	//Vamos leyendo el archivo línea a línea
	while(fgets(linea, sizeof(linea), fileAlias) != NULL) {
		//Ignoramos las líneas que están comentadas
		if(linea[0] != '#') {
			//Recogemos el alias que hayamos puesto
			char alias[100] = "";
			int index = 0;
			while(linea[index] != ' ' && linea[index] != '=' && linea[index] != '\n' && index<100) {
				alias[index] = linea[index];
				index++;
			}
			//Comparamos el alias con el input
			if(strcmp(input, alias) == 0) {
				//printf("Se ha detectado una coincidencia");
				//Volvemos a recorrer la línea buscando el '=' para empezar a leer
				index = 0;
				while(linea[index] != '=') {
					index++;
				}
				//añadimos uno para posicionarnos en el caracter siguiente al igual
				index++;
				char comando[100];
				int indexComando = 0;
				//Continuamos recorriendo a partir del = guardando el comando
				while(linea[index] != '\n' && linea[index] != EOF && linea[index] != '\0'){
					//Ignoramos espacios blancos
					if(linea[index] != ' '){
						comando[indexComando] = linea[index];
						indexComando++;
					}
					index++;
				}
				//Por último, cambiamos input para usar el comando
				for(int i = 0; i<sizeof(comando); i++){
					input[i] = comando[i];
				}
			}
		}
	}
	fflush(stdout);	
	fclose(fileAlias);	
}

