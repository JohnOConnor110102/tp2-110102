#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "lista.h"

#define ERROR -1

typedef struct comando {
	const char *nombre;
	const char *instruccion;
	const char *descripcion;
	bool (*funcion)(void *, void *);
	void *contexto;
} comando_t;

typedef struct menu {
	lista_t *comandos;
	int cantidad_comandos;
} menu_t;

/* 
	* Crea el menu, reservando la memoria necesaria e inicializando
	* los campos del mismo.
	* Devuelve NULL en caso de no poder crearlo, o el menu creado en
	* caso contrario.
*/
menu_t *menu_crear();

/* 
	* Agrega el comando al menu y lo hace disponible para su 
	* utilización.
	* Devuelve NULL en caso de error o de no poder agregarlo,
	* o el menu en caso constrario.
*/
menu_t *menu_agregar_comando(menu_t *menu, const char *nombre,
			     const char *instruccion, const char *descripcion,
			     bool (*funcion)(void *, void *), void *contexto);

/* 
	* Ejecuta el comando dado del menú.
	* Devuelve NULL en caso de error o de no poder eliminarlo, 
	* o el menú en caso contrario.
*/
menu_t *menu_ejecutar_comando(menu_t *menu, const char *instruccion);

/* 
	* Devuelve true si el menú contiene el comando dado, o 
	* false en caso contrario.
 */
bool menu_contiene_comando(menu_t *menu, const char *instruccion);

/* 
	* Busca el comando dado del menú.
	* Devuelve NULL en caso de error o de no encontrarlo,
	* o el comando en caso contrario.
*/
comando_t *menu_obtener_comando(menu_t *menu, const char *instruccion);

/* 
	* Devuelve true si el menú no tiene comandos o no existe,
	* y false en caso contrario.
*/
bool menu_vacio(menu_t *menu);

/* 
	* Devuelve la cantidad de comandos del menú.
	* Devuelve 0 si el menú no existe.
*/
size_t menu_cantidad_comandos(menu_t *menu);

/* 
	* Elimina el comando dado del menú y libera la memoria 
	* reservada para el mismo.
	* Devuelve NULL en caso de error o de no poder eliminarlo, 
	* o el menú en caso contrario.
*/
menu_t *menu_eliminar_comando(menu_t *menu, const char *instruccion);

/* 
	* Libera la memoria reservada para el menu
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_H__