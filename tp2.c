#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "src/tp1.h"

#define MAX_ENTRADA 256

void imprimir_archivo(FILE *archivo)
{
	if (!archivo)
		return;
	rewind(archivo);
	char c = (char)fgetc(archivo);
	while (c != EOF) {
		printf("%c", c);
		c = (char)fgetc(archivo);
	}
}

bool mostrar_mensaje(void *_menu, void *_mensaje)
{
	if (!_mensaje)
		return false;
	FILE *mensaje = _mensaje;
	imprimir_archivo(mensaje);
	return true;
}

void mostrar_mensaje_bienvenida()
{
	FILE *bienvenida = fopen("mensajes/mensaje_bienvenida.txt", "r");
	if (!bienvenida)
		return;

	mostrar_mensaje(NULL, bienvenida);
	fclose(bienvenida);
}

bool salir_del_programa(void *_menu, void *_contexto)
{
	menu_destruir((menu_t *)_menu);
	return true;
}

void agregar_comandos(menu_t *menu, FILE *mensaje_ayuda)
{
	menu_agregar_comando(
		menu, "Mostrar la ayuda", "h",
		"mostrar el menu de ayuda con los comandos disponibles.",
		mostrar_mensaje, mensaje_ayuda);
	menu_agregar_comando(menu, "Salir del programa", "s",
			     "salir del programa.", salir_del_programa, NULL);
	/* menu_agregar_comando(
		menu, "Cargar un hospital", "C",
		"crear y cargar un hospital tomando los datos desde un archivo.",
		cargar_hospital);
	menu_agregar_comando(
		menu, "Mostrar el estado de los hospitales", "E",
		"mostrar un listado con el estado de los hospitales existentes.",
		mostrar_estado_hospitales);
	menu_agregar_comando(
		menu, "Activar un hospital", "A",
		"activar un hospital dado un numero de identificación del mismo.",
		activar_hospital);
	menu_agregar_comando(menu, "Mostrar los pokemones", "M",
			     "mostrar los pokemones presentes en el hospital.",
			     mostrar_pokemones);
	menu_agregar_comando(
		menu, "Listar los pokemones", "L",
		"mostrar un listado detallado de los pokemones presentes en el hospital.",
		listar_pokemones);
	menu_agregar_comando(menu, "Destruir un hospital", "D",
			     "destruir el hospital activo.",
			     destruir_hospital); */
}

menu_t *crear_hospital_pokemones(FILE *mensaje_ayuda)
{
	menu_t *menu = menu_crear();
	if (!menu)
		return NULL;

	agregar_comandos(menu, mensaje_ayuda);
	return menu;
}

char *determinar_comando(char *instruccion)
{
	if (!instruccion)
		return NULL;

	if (strcmp(instruccion, "help") == 0 ||
	    strcmp(instruccion, "ayuda") == 0) {
		strcpy(instruccion, "h");
	} else if (strcmp(instruccion, "salir") == 0 ||
		   strcmp(instruccion, "exit") == 0 ||
		   strcmp(instruccion, "quit") == 0 ||
		   strcmp(instruccion, "q") == 0) {
		strcpy(instruccion, "s");
	}

	return instruccion;
}

char *registrar_entrada(char *entrada, char *instruccion)
{
	printf("Ingrese un comando a realizar: ");
	fgets(entrada, MAX_ENTRADA, stdin);
	sscanf(entrada, "%s", instruccion);
	for (int j = 0; instruccion[j]; j++) {
		instruccion[j] = (char)tolower(instruccion[j]);
	}
	return determinar_comando(instruccion);
}

int main(int argc, char *argv[])
{
	system("clear");
	FILE *mensaje_ayuda = fopen("mensajes/mensaje_ayuda.txt", "r");
	if (!mensaje_ayuda)
		return ERROR;

	menu_t *menu = crear_hospital_pokemones(mensaje_ayuda);
	if (!menu)
		return ERROR;

	mostrar_mensaje_bienvenida();
	bool finalizado = false;
	while (!finalizado) {
		char entrada[MAX_ENTRADA];
		char instruccion[MAX_ENTRADA];
		if (strcmp(registrar_entrada(entrada, instruccion), "s") == 0)
			finalizado = true;
		printf("instruccion: %s\n", instruccion);
		menu_ejecutar_comando(menu, instruccion);
	}

	fclose(mensaje_ayuda);
	return 0;
}
