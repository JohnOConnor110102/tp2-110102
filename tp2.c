#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu.h"
#include "src/tp1.h"

#define MAX_STRINGS 256
#define ROJO "\x1b[31m"
#define COLOR_PREDETERMINADO "\x1b[0m"

typedef struct valor {
	hospital_t *hospital;
	char nombre_hospital[MAX_STRINGS];
	bool activo;
} valor_t;

typedef struct hash_hospitales {
	hash_t *hospitales;
	char clave_hospital_activo[MAX_STRINGS];
} hash_hospitales_t;

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
	FILE *mensaje = (FILE *)_mensaje;
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

bool limpiar_terminal(void *_menu, void *_contexto)
{
	system("clear");
	return true;
}

void destructor_hash(void *_elemento)
{
	valor_t *valor = (valor_t *)_elemento;
	hospital_destruir(valor->hospital);
	free(valor);
}

void hash_hospitales_destruir(hash_hospitales_t *hash_hospitales)
{
	hash_destruir_todo(hash_hospitales->hospitales, destructor_hash);
	free(hash_hospitales);
}

bool salir_del_programa(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;
	menu_destruir((menu_t *)_menu);
	hash_hospitales_destruir(hash_hospitales);
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

valor_t *crear_valor(void *_hospital, char nombre_hospital[MAX_STRINGS])
{
	if (!_hospital)
		return NULL;

	valor_t *valor = calloc(1, sizeof(valor_t));
	if (!valor)
		return NULL;

	valor->hospital = (hospital_t *)_hospital;
	strcpy(valor->nombre_hospital, nombre_hospital);
	return valor;
}

bool cargar_hospital(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;
	char entrada[MAX_STRINGS];
	char numero_hospital[MAX_STRINGS];
	char nombre_hospital[MAX_STRINGS];
	strcpy(numero_hospital, "");
	strcpy(nombre_hospital, "");

	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                       CARGAR HOSPITAL                        ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║Ingresá el nombre del archivo en el que se encuentra el       ║\n"
	       "	║hospital y el número de identificación del mismo, según       ║\n"
	       "	║muestra el siguiente ejemplo: 1 hospital_pokemones.txt        ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n"
	       "hospital: ");
	fgets(entrada, MAX_STRINGS, stdin);
	int aux = sscanf(entrada, "%s %s", numero_hospital, nombre_hospital);
	if (aux != 2)
		return false;

	char direccion_archivo_pokemon[MAX_STRINGS] = "hospitales/";
	strcat(direccion_archivo_pokemon, nombre_hospital);
	void *hospital =
		(void *)hospital_crear_desde_archivo(direccion_archivo_pokemon);

	void *valor = crear_valor(hospital, nombre_hospital);
	if (!hospital || !hash_insertar(hash_hospitales->hospitales,
					numero_hospital, valor, NULL))
		return false;

	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║               HOSPITAL CARGADO CORRECTAMENTE!                ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool imprimir_hospital(const char *numero_hospital, void *_valor, void *aux)
{
	if (!numero_hospital || !_valor)
		return false;

	valor_t *valor = (valor_t *)_valor;
	if (valor->activo) {
		printf("	║" ROJO "(ACTIVO)" COLOR_PREDETERMINADO
		       " NÚMERO: %s	NOMBRE: %s\n",
		       numero_hospital, valor->nombre_hospital);
	} else {
		printf("	║	  NUMERO: %s	NOMBRE: %s\n",
		       numero_hospital, valor->nombre_hospital);
	}
	return true;
}

bool mostrar_estado_hospitales(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;
	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║               LISTA DE LOS HOSPITALES ACTUALES               ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n");
	if (hash_cantidad(hash_hospitales->hospitales) == 0)
		printf("	║Actualmente no hay hospitales cargados.		       ║\n");
	else
		hash_con_cada_clave(hash_hospitales->hospitales,
				    imprimir_hospital, NULL);
	printf("	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool activar_hospital(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;
	if (hash_cantidad(hash_hospitales->hospitales) == 0) {
		printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
		       "	║Actualmente no hay hospitales disponibles para su activación. ║\n"
		       "	╚══════════════════════════════════════════════════════════════╝\n");
		return false;
	}
	if (strcmp(hash_hospitales->clave_hospital_activo, "") != 0) {
		printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
		       "	║Ya hay un hospital activo, y sólo se permite tener uno a la   ║\n"
		       "	║vez. Si querés activar otro hospital, tenés que destruir el   ║\n"
		       "	║activo.						       ║\n"
		       "	╚══════════════════════════════════════════════════════════════╝\n");
		return false;
	}

	char entrada[MAX_STRINGS];
	char numero_hospital_a_activar[MAX_STRINGS];
	strcpy(numero_hospital_a_activar, "");

	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║                       ACTIVAR HOSPITAL                       ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n"
	       "	║Para activar un hospital, ingresá el número de identificación ║\n"
	       "	║del hospital a activar.				       ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
	printf("Número de identificación del hospital: ");
	fgets(entrada, MAX_STRINGS, stdin);
	int aux = sscanf(entrada, "%s", numero_hospital_a_activar);
	if (aux != 1)
		return false;

	valor_t *valor = (valor_t *)hash_obtener(hash_hospitales->hospitales,
						 numero_hospital_a_activar);
	if (!valor)
		return false;

	valor->activo = true;
	strcpy(hash_hospitales->clave_hospital_activo,
	       numero_hospital_a_activar);
	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║               HOSPITAL ACTIVADO CORRECTAMENTE!               ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool imprimir_pokemon(pokemon_t *pokemon, void *aux)
{
	if (!pokemon)
		return false;

	printf("	║Nombre: %s\n", pokemon_nombre(pokemon));
	return true;
}

bool mostrar_pokemones(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;

	valor_t *valor_activo =
		(valor_t *)hash_obtener(hash_hospitales->hospitales,
					hash_hospitales->clave_hospital_activo);

	if (!valor_activo || !valor_activo->activo) {
		printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
		       "	║Actualmente no hay ningún hospital que haya sido activado.    ║\n"
		       "	╚══════════════════════════════════════════════════════════════╝\n\n");
		return false;
	}
	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║         LISTA DE LOS POKEMONES EN EL HOSPITAL ACTIVO         ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n");
	if (hospital_cantidad_pokemones(valor_activo->hospital) == 0)
		printf("	║Actualmente no hay hospitales cargados.		       ║\n");
	else
		hospital_a_cada_pokemon(valor_activo->hospital,
					imprimir_pokemon, NULL);
	printf("	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool imprimir_pokemon_detallado(pokemon_t *pokemon, void *aux)
{
	if (!pokemon)
		return false;

	printf("	║Nombre: %s	ID: %lu		SALUD: %lu\n"
	       "	║Entrenador: %s\n	║\n",
	       pokemon_nombre(pokemon), pokemon_id(pokemon),
	       pokemon_salud(pokemon), pokemon_entrenador(pokemon));
	return true;
}

bool listar_pokemones(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;

	valor_t *valor_activo =
		(valor_t *)hash_obtener(hash_hospitales->hospitales,
					hash_hospitales->clave_hospital_activo);

	if (!valor_activo || !valor_activo->activo) {
		printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
		       "	║Actualmente no hay ningún hospital que haya sido activado.    ║\n"
		       "	╚══════════════════════════════════════════════════════════════╝\n\n");
		return false;
	}

	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║         LISTA DE LOS POKEMONES EN EL HOSPITAL ACTIVO         ║\n"
	       "	╠══════════════════════════════════════════════════════════════╣\n");
	if (hospital_cantidad_pokemones(valor_activo->hospital) == 0)
		printf("	║Actualmente no hay hospitales cargados.		       ║\n");
	else
		hospital_a_cada_pokemon(valor_activo->hospital,
					imprimir_pokemon_detallado, NULL);
	printf("	╚══════════════════════════════════════════════════════════════╝\n\n");
	return true;
}

bool destruir_hospital(void *_menu, void *_hash_hospitales)
{
	if (!_menu || !_hash_hospitales)
		return false;

	hash_hospitales_t *hash_hospitales =
		(hash_hospitales_t *)_hash_hospitales;
	valor_t *valor_activo =
		hash_quitar(hash_hospitales->hospitales,
			    hash_hospitales->clave_hospital_activo);
	if (!valor_activo || !valor_activo->activo) {
		printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
		       "	║Actualmente no hay ningún hospital que haya sido activado.    ║\n"
		       "	╚══════════════════════════════════════════════════════════════╝\n\n");
		return false;
	}
	hospital_destruir(valor_activo->hospital);
	free(valor_activo);
	printf("\n	╔══════════════════════════════════════════════════════════════╗\n"
	       "	║               HOSPITAL DESTRUIDO CORRECTAMENTE!              ║\n"
	       "	╚══════════════════════════════════════════════════════════════╝\n\n");

	return true;
}

bool agregar_comandos(menu_t *menu, FILE *mensaje_ayuda,
		      hash_hospitales_t *hash_hospitales)
{
	if (!menu_agregar_comando(menu, "Limpiar la pantalla", "x",
				  "limpiar toda la información anterior.",
				  limpiar_terminal, NULL) ||
	    !menu_agregar_comando(
		    menu, "Mostrar la ayuda", "h",
		    "mostrar el menu de ayuda con los comandos disponibles.",
		    mostrar_mensaje, mensaje_ayuda) ||
	    !menu_agregar_comando(menu, "Salir del programa", "s",
				  "salir del programa.", salir_del_programa,
				  hash_hospitales) ||
	    !menu_agregar_comando(
		    menu, "Cargar un hospital", "c",
		    "crear y cargar un hospital tomando los datos desde un archivo.",
		    cargar_hospital, hash_hospitales) ||
	    !menu_agregar_comando(
		    menu, "Mostrar el estado de los hospitales", "e",
		    "mostrar un listado con el estado de los hospitales existentes.",
		    mostrar_estado_hospitales, hash_hospitales) ||
	    !menu_agregar_comando(
		    menu, "Activar un hospital", "a",
		    "activar un hospital dado un numero de identificación del mismo.",
		    activar_hospital, hash_hospitales) ||
	    !menu_agregar_comando(
		    menu, "Mostrar los pokemones", "m",
		    "mostrar los pokemones presentes en el hospital.",
		    mostrar_pokemones, hash_hospitales) ||
	    !menu_agregar_comando(
		    menu, "Listar los pokemones", "l",
		    "mostrar un listado detallado de los pokemones presentes en el hospital.",
		    listar_pokemones, hash_hospitales) ||
	    !menu_agregar_comando(menu, "Destruir un hospital", "d",
				  "destruir el hospital activo.",
				  destruir_hospital, hash_hospitales)) {
		return false;
	}

	return true;
}

menu_t *crear_hospital_pokemones(FILE *mensaje_ayuda,
				 hash_hospitales_t *hash_hospitales)
{
	menu_t *menu = menu_crear();
	if (!menu || !agregar_comandos(menu, mensaje_ayuda, hash_hospitales))
		return NULL;

	return menu;
}

char *determinar_comando(char *instruccion)
{
	if (!instruccion)
		return NULL;

	if (strcmp(instruccion, "clear") == 0 ||
	    strcmp(instruccion, "limpiar") == 0) {
		strcpy(instruccion, "x");
	} else if (strcmp(instruccion, "help") == 0 ||
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
	} else if (strcmp(instruccion, "estado") == 0) {
		strcpy(instruccion, "e");
	} else if (strcmp(instruccion, "activar") == 0 ||
		   strcmp(instruccion, "habilitar") == 0) {
		strcpy(instruccion, "a");
	} else if (strcmp(instruccion, "mostrar") == 0) {
		strcpy(instruccion, "m");
	} else if (strcmp(instruccion, "listar") == 0 ||
		   strcmp(instruccion, "lista") == 0 ||
		   strcmp(instruccion, "pokemones") == 0) {
		strcpy(instruccion, "l");
	} else if (strcmp(instruccion, "destruir") == 0 ||
		   strcmp(instruccion, "destroy") == 0 ||
		   strcmp(instruccion, "kill") == 0 ||
		   strcmp(instruccion, "k") == 0) {
		strcpy(instruccion, "d");
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

void imprimir_error()
{
	printf("╔══════════════════════════════════════════════════════════════╗\n"
	       "║Lo sentimos mucho! No se pudo ejecutar el programa 	       ║\n"
	       "║correctamente. Podés intentarlo de nuevo. Recordá que	       ║\n"
	       "║ingresando el comando de ayuda (H, help, ayuda, etc) te damos ║\n"
	       "║información útil.					       ║\n"
	       "╚══════════════════════════════════════════════════════════════╝\n\n");
}

hash_hospitales_t *crear_hash_hospitales(size_t capacidad)
{
	hash_hospitales_t *hash_hospitales =
		calloc(1, sizeof(hash_hospitales_t));
	if (!hash_hospitales)
		return NULL;

	hash_hospitales->hospitales = hash_crear(capacidad);
	if (!hash_hospitales)
		return NULL;

	strcpy(hash_hospitales->clave_hospital_activo, "");
	return hash_hospitales;
}

int main(int argc, char *argv[])
{
	system("clear");
	FILE *mensaje_ayuda = fopen("mensajes/mensaje_ayuda.txt", "r");
	if (!mensaje_ayuda) {
		imprimir_error();
		return ERROR;
	}
	FILE *mensaje_error_comando =
		fopen("mensajes/mensaje_error_comando.txt", "r");
	if (!mensaje_error_comando) {
		imprimir_error();
		fclose(mensaje_ayuda);
		return ERROR;
	}
	hash_hospitales_t *hash_hospitales = crear_hash_hospitales(13);
	if (!hash_hospitales) {
		imprimir_error();
		fclose(mensaje_ayuda);
		fclose(mensaje_error_comando);
		return ERROR;
	}
	menu_t *menu = crear_hospital_pokemones(mensaje_ayuda, hash_hospitales);
	if (!menu) {
		imprimir_error();
		fclose(mensaje_ayuda);
		fclose(mensaje_error_comando);
		hash_hospitales_destruir(hash_hospitales);
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

	fclose(mensaje_ayuda);
	fclose(mensaje_error_comando);
	return 0;
}
