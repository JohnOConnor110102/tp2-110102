#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "src/tp1.h"

#define MAX_STRINGS 256

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

void destructor_hash(void *_elemento)
{
	hospital_t *hospital = (hospital_t *)_elemento;
	hospital_destruir(hospital);
}

bool salir_del_programa(void *_menu, void *_hospitales)
{
	menu_destruir((menu_t *)_menu);
	hash_destruir_todo((hash_t *)_hospitales, destructor_hash);
	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                        HASTA PRONTO!                         ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║Muchas gracias por haber visitado el Hospital de Pokemones!   ║\n"
	       "	║Esperamos que nos vuelva a elegir para su próxima	       ║\n"
	       "	║poke-urgencia.						       ║\n"
	       "	║							       ║\n"
	       "	║Atentamente, el equipo de Hospital de Pokemones.	       ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool cargar_hospital(void *_menu, void *_hospitales)
{
	if (!_menu || !_hospitales)
		return false;

	hash_t *hospitales = (hash_t *)_hospitales;
	char entrada[MAX_STRINGS];
	char clave[MAX_STRINGS];
	char nombre_archivo_pokemon[MAX_STRINGS];
	strcpy(clave, "");
	strcpy(nombre_archivo_pokemon, "");

	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                       CARGAR HOSPITAL                        ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║Ingresá el nombre del archivo en el que se encuentra el       ║\n"
	       "	║hospital y el número de identificación del mismo, según       ║\n"
	       "	║muestra el siguiente ejemplo: 1 hospital_pokemones.txt        ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n"
	       "hospital: ");
	fgets(entrada, MAX_STRINGS, stdin);
	int aux = sscanf(entrada, "%s %s", clave, nombre_archivo_pokemon);
	if (aux != 2)
		return false;

	char direccion_archivo_pokemon[MAX_STRINGS] = "hospitales/";
	strcat(direccion_archivo_pokemon, nombre_archivo_pokemon);
	void *hospital =
		(void *)hospital_crear_desde_archivo(direccion_archivo_pokemon);
	if (!hospital || !hash_insertar(hospitales, clave, hospital, NULL))
		return false;

	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║               HOSPITAL CARGADO CORRECTAMENTE!                ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool mostrar_estado_hospitales(void *menu, void *_hospitales)
{
}

void agregar_comandos(menu_t *menu, FILE *mensaje_ayuda, hash_t *hospitales)
{
	menu_agregar_comando(
		menu, "Mostrar la ayuda", "h",
		"mostrar el menu de ayuda con los comandos disponibles.",
		mostrar_mensaje, mensaje_ayuda);
	menu_agregar_comando(menu, "Salir del programa", "s",
			     "salir del programa.", salir_del_programa,
			     hospitales);
	menu_agregar_comando(
		menu, "Cargar un hospital", "c",
		"crear y cargar un hospital tomando los datos desde un archivo.",
		cargar_hospital, hospitales);
	menu_agregar_comando(
		menu, "Mostrar el estado de los hospitales", "E",
		"mostrar un listado con el estado de los hospitales existentes.",
		mostrar_estado_hospitales, hospitales);
	/* menu_agregar_comando(
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

menu_t *crear_hospital_pokemones(FILE *mensaje_ayuda, hash_t *hospitales)
{
	menu_t *menu = menu_crear();
	if (!menu)
		return NULL;

	agregar_comandos(menu, mensaje_ayuda, hospitales);
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
	} else if (strcmp(instruccion, "cargar") == 0 ||
		   strcmp(instruccion, "crear") == 0) {
		strcpy(instruccion, "c");
	}

	return instruccion;
}

char *registrar_entrada(char *entrada, char *instruccion)
{
	printf("Ingresá un comando a realizar: ");
	fgets(entrada, MAX_STRINGS, stdin);
	sscanf(entrada, "%s", instruccion);
	for (int j = 0; instruccion[j]; j++) {
		instruccion[j] = (char)tolower(instruccion[j]);
	}
	return determinar_comando(instruccion);
}

int main(int argc, char *argv[])
{
	system("clear");
	FILE *mensaje_error = fopen("mensajes/mensaje_error.txt", "r");
	if (!mensaje_error) {
		printf("╔══════════════════════════════════════════════════════════════╗\n"
		       "║Lo sentimos mucho! No se pudo ejecutar el programa 	       ║\n"
		       "║correctamente.Podés intentarlo de nuevo.Recordá que	       ║\n"
		       "║ingresando el comando de ayuda (H, help, ayuda, etc) te damos ║\n"
		       "║información útil.					       ║\n"
		       "╚══════════════════════════════════════════════════════════════╝\n\n");
		return ERROR;
	}
	FILE *mensaje_ayuda = fopen("mensajes/mensaje_ayuda.txt", "r");
	if (!mensaje_ayuda) {
		mostrar_mensaje(NULL, mensaje_error);
		fclose(mensaje_error);
		return ERROR;
	}
	FILE *mensaje_error_comando =
		fopen("mensajes/mensaje_error_comando.txt", "r");
	if (!mensaje_error_comando) {
		mostrar_mensaje(NULL, mensaje_error);
		fclose(mensaje_error);
		fclose(mensaje_ayuda);
		return ERROR;
	}
	hash_t *hospitales = hash_crear(13);
	if (!hospitales) {
		mostrar_mensaje(NULL, mensaje_error);
		fclose(mensaje_error);
		fclose(mensaje_ayuda);
		fclose(mensaje_error_comando);
		return ERROR;
	}
	menu_t *menu = crear_hospital_pokemones(mensaje_ayuda, hospitales);
	if (!menu) {
		mostrar_mensaje(NULL, mensaje_error);
		fclose(mensaje_error);
		fclose(mensaje_ayuda);
		fclose(mensaje_error_comando);
		hash_destruir(hospitales);
		return ERROR;
	}

	mostrar_mensaje_bienvenida();
	bool finalizado = false;
	while (!finalizado) {
		char entrada[MAX_STRINGS];
		char instruccion[MAX_STRINGS];
		if (strcmp(registrar_entrada(entrada, instruccion), "s") == 0)
			finalizado = true;
		if (!menu_ejecutar_comando(menu, instruccion)) {
			mostrar_mensaje(NULL, mensaje_error_comando);
		}
	}

	fclose(mensaje_error);
	fclose(mensaje_ayuda);
	fclose(mensaje_error_comando);
	return 0;
}
