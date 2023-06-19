#include <string.h>
#include "menu.h"

/* void imprimir_archivo(FILE *archivo)
{
	if (!archivo)
		return;

	char c = (char)fgetc(archivo);
	while (c != EOF) {
		printf("%c", c);
		c = (char)fgetc(archivo);
	}
}

void menu_mostrar_mensaje(FILE *mensaje)
{
	if (!mensaje)
		return;

	imprimir_archivo(mensaje);
} */

menu_t *menu_crear()
{
	menu_t *menu = calloc(1, sizeof(menu_t));
	if (!menu) {
		return NULL;
	}
	menu->comandos = lista_crear();
	if (!menu->comandos) {
		free(menu);
		return NULL;
	}
	return menu;
}

comando_t *crear_comando(const char *nombre, const char *instruccion,
			 const char *descripcion,
			 bool (*funcion)(void *, void *), void *contexto)
{
	if (!nombre || !instruccion || !descripcion || !funcion)
		return NULL;

	comando_t *comando = malloc(sizeof(comando_t));
	if (!comando)
		return NULL;
	comando->nombre = nombre;
	comando->instruccion = instruccion;
	comando->descripcion = descripcion;
	comando->funcion = funcion;
	comando->contexto = contexto;
	return comando;
}

menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     bool (*funcion)(void *, void *), void *contexto)
{
	if (!menu || !nombre || !instruccion || !descripcion || !funcion)
		return NULL;

	comando_t *comando = crear_comando(nombre, instruccion, descripcion,
					   funcion, contexto);
	if (!comando)
		return NULL;

	lista_insertar(menu->comandos, comando);
	return menu;
}

int buscar_comando(void *_elemento, void *_instruccion)
{
	if (!_elemento || !_instruccion)
		return false;

	comando_t *comando = (comando_t *)_elemento;
	char *instruccion = (char *)_instruccion;
	return strcmp(comando->instruccion, instruccion);
}

menu_t *menu_ejecutar_comando(menu_t *menu, const char *instruccion)
{
	if (!menu || !instruccion)
		return NULL;
	comando_t *comando = lista_buscar_elemento(
		menu->comandos, buscar_comando, (void *)instruccion);
	if (!comando || !comando->funcion(menu, comando->contexto))
		return NULL;

	return menu;
}

void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	lista_destruir_todo(menu->comandos, free);
	free(menu);
}