#include "menu.h"

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

int buscar_comando(void *_elemento, void *_instruccion)
{
	if (!_elemento || !_instruccion)
		return false;

	comando_t *comando = (comando_t *)_elemento;
	char *instruccion = (char *)_instruccion;
	return strcmp(comando->instruccion, instruccion);
}

menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     bool (*funcion)(void *, void *), void *contexto)
{
	if (!menu || !nombre || !instruccion || !descripcion || !funcion ||
	    !!lista_buscar_elemento(menu->comandos, buscar_comando,
				    (void *)instruccion))
		return NULL;

	comando_t *comando = crear_comando(nombre, instruccion, descripcion,
					   funcion, contexto);
	if (!comando)
		return NULL;

	lista_insertar(menu->comandos, comando);
	return menu;
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

bool menu_contiene_comando(menu_t *menu, const char *instruccion)
{
	if (!menu || !instruccion || !menu_obtener_comando(menu, instruccion))
		return false;

	return true;
}

comando_t *menu_obtener_comando(menu_t *menu, const char *instruccion)
{
	if (!menu || !instruccion)
		return NULL;

	return lista_buscar_elemento(menu->comandos, buscar_comando,
				     (void *)instruccion);
}

bool menu_vacio(menu_t *menu)
{
	if (!menu || lista_vacia(menu->comandos) == true)
		return true;

	return false;
}

size_t menu_cantidad_comandos(menu_t *menu)
{
	if (!menu)
		return 0;

	return lista_tamanio(menu->comandos);
}

menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion)
{
	if (!menu || !instruccion)
		return NULL;

	size_t posicion_comando = 0;
	lista_iterador_t *lista_iterador = lista_iterador_crear(menu->comandos);
	bool comando_encontrado = false;
	while (lista_iterador_tiene_siguiente(lista_iterador) &&
	       !comando_encontrado) {
		posicion_comando++;
		comando_t *comando =
			lista_iterador_elemento_actual(lista_iterador);
		if (strcmp(comando->instruccion, instruccion) == 0) {
			posicion_comando--;
			comando_encontrado = true;
		}
	}
	if (!comando_encontrado)
		return NULL;

	comando_t *comando_eliminado =
		lista_quitar_de_posicion(menu->comandos, posicion_comando);
	free(comando_eliminado);
	lista_iterador_destruir(lista_iterador);
	return menu;
}

void menu_destruir(menu_t *menu)
{
	if (!menu)
		return;

	lista_destruir_todo(menu->comandos, free);
	free(menu);
}