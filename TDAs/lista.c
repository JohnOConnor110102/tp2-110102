#include "lista.h"
#include <stddef.h>
#include <stdlib.h>

#define ERROR -1
#define EXITO 0

const int MINIMA_POSICION_LISTA = 0;
const int LISTA_SIN_NODOS = 0;
const int LISTA_UN_NODO = 1;
const int ELEMENTO_ENCONTRADO = 0;
const int POSICION_CERO = 0;

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *inicio;
	nodo_t *fin;
	size_t cantidad_nodos;
};

struct lista_iterador {
	nodo_t *nodo_actual;
	size_t posicion_actual;
	lista_t *lista;
};

lista_t *insertar_en_lista_sin_nodos(lista_t *lista, nodo_t *nodo)
{
	if (lista == NULL)
		return NULL;

	lista->inicio = nodo;
	lista->fin = nodo;
	lista->cantidad_nodos++;
	return lista;
}

nodo_t *iterar_hasta_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL || posicion >= lista->cantidad_nodos)
		return NULL;

	size_t posicion_actual = 0;
	nodo_t *nodo_actual = lista->inicio;
	nodo_t *nodo_aux;
	while (posicion_actual < posicion) {
		nodo_aux = nodo_actual->siguiente;
		nodo_actual = nodo_aux;
		posicion_actual++;
	}
	return nodo_actual;
}

nodo_t *crear_nodo(void *elemento)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL)
		return NULL;

	nodo->elemento = elemento;
	nodo->siguiente = NULL;
	return nodo;
}

lista_t *insertar_en_posicion_cero(lista_t *lista, nodo_t *nodo_a_agregar)
{
	if (lista == NULL || nodo_a_agregar == NULL)
		return NULL;

	nodo_a_agregar->siguiente = lista->inicio;
	lista->inicio = nodo_a_agregar;
	lista->cantidad_nodos++;
	return lista;
}

void *quitar_de_lista_con_un_nodo(lista_t *lista)
{
	if (lista == NULL)
		return NULL;

	void *elemento_unico_nodo = lista->inicio->elemento;
	free(lista->inicio);
	lista->cantidad_nodos--;
	lista->fin = NULL;
	lista->inicio = NULL;
	return elemento_unico_nodo;
}

void *quitar_de_posicion_cero(lista_t *lista)
{
	if (lista == NULL)
		return NULL;

	void *elemento_nodo_cero = lista->inicio->elemento;
	nodo_t *nodo_aux = lista->inicio->siguiente;
	free(lista->inicio);
	lista->inicio = nodo_aux;
	lista->cantidad_nodos--;
	return elemento_nodo_cero;
}

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL)
		return NULL;

	nodo_t *nodo = crear_nodo(elemento);
	if (nodo == NULL)
		return NULL;

	if (lista->cantidad_nodos == LISTA_SIN_NODOS)
		return insertar_en_lista_sin_nodos(lista, nodo);

	lista->fin->siguiente = nodo;
	lista->fin = nodo;
	lista->cantidad_nodos++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL)
		return NULL;

	if (posicion >= lista->cantidad_nodos)
		return lista_insertar(lista, elemento);

	nodo_t *nodo_a_agregar = crear_nodo(elemento);
	if (nodo_a_agregar == NULL)
		return NULL;

	if (posicion == POSICION_CERO)
		return insertar_en_posicion_cero(lista, nodo_a_agregar);

	nodo_t *nodo_posicion_menos_uno =
		iterar_hasta_posicion(lista, posicion - 1);
	nodo_a_agregar->siguiente = nodo_posicion_menos_uno->siguiente;
	nodo_posicion_menos_uno->siguiente = nodo_a_agregar;
	lista->cantidad_nodos++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_nodos == LISTA_SIN_NODOS)
		return NULL;

	if (lista->cantidad_nodos == LISTA_UN_NODO)
		return quitar_de_lista_con_un_nodo(lista);

	nodo_t *nodo_fin_menos_uno =
		iterar_hasta_posicion(lista, lista->cantidad_nodos - 2);
	void *elemento_nodo = nodo_fin_menos_uno->siguiente->elemento;
	free(nodo_fin_menos_uno->siguiente);
	nodo_fin_menos_uno->siguiente = NULL;
	lista->cantidad_nodos--;
	lista->fin = nodo_fin_menos_uno;
	return elemento_nodo;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL || lista->cantidad_nodos == LISTA_SIN_NODOS)
		return NULL;

	if (posicion >= lista->cantidad_nodos - 1)
		return lista_quitar(lista);

	if (posicion == POSICION_CERO)
		return quitar_de_posicion_cero(lista);

	nodo_t *nodo_eliminar_menos_uno =
		iterar_hasta_posicion(lista, posicion - 1);
	nodo_t *nodo_eliminar = nodo_eliminar_menos_uno->siguiente;
	void *elemento_nodo = nodo_eliminar->elemento;
	nodo_eliminar_menos_uno->siguiente = nodo_eliminar->siguiente;
	free(nodo_eliminar);
	lista->cantidad_nodos--;
	return elemento_nodo;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista == NULL || posicion >= lista->cantidad_nodos)
		return NULL;

	return iterar_hasta_posicion(lista, posicion)->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (lista == NULL || comparador == NULL ||
	    lista->cantidad_nodos == LISTA_SIN_NODOS)
		return NULL;

	void *elemento_buscado = NULL;
	nodo_t *nodo_actual = lista->inicio;
	while (nodo_actual != NULL &&
	       comparador(nodo_actual->elemento, contexto) !=
		       ELEMENTO_ENCONTRADO) {
		nodo_actual = nodo_actual->siguiente;
	}
	if (nodo_actual)
		elemento_buscado = nodo_actual->elemento;

	return elemento_buscado;
}

void *lista_primero(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_nodos == LISTA_SIN_NODOS)
		return NULL;

	return lista->inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_nodos == LISTA_SIN_NODOS)
		return NULL;

	return lista->fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL || lista->cantidad_nodos == LISTA_SIN_NODOS)
		return true;

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista == NULL)
		return 0;

	return lista->cantidad_nodos;
}

void lista_destruir(lista_t *lista)
{
	if (lista == NULL)
		return;

	lista_destruir_todo(lista, NULL);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista == NULL)
		return;
	if (lista_tamanio(lista) > 0) {
		int contador = 0;
		nodo_t *nodo_actual = lista->inicio;
		nodo_t *nodo_aux = nodo_actual->siguiente;
		while (contador < lista->cantidad_nodos) {
			nodo_aux = nodo_actual->siguiente;
			if (funcion != NULL)
				funcion(nodo_actual->elemento);

			free(nodo_actual);
			nodo_actual = nodo_aux;
			contador++;
		}
	}

	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (iterador == NULL)
		return NULL;

	iterador->lista = lista;
	iterador->posicion_actual = 0;
	iterador->nodo_actual = lista->inicio;
	return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (iterador == NULL ||
	    iterador->lista->cantidad_nodos == LISTA_SIN_NODOS)
		return false;

	if (iterador->posicion_actual < iterador->lista->cantidad_nodos)
		return true;

	return false;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (iterador == NULL ||
	    iterador->lista->cantidad_nodos == LISTA_SIN_NODOS ||
	    iterador->posicion_actual > iterador->lista->cantidad_nodos - 1)
		return false;

	iterador->nodo_actual = iterador->nodo_actual->siguiente;
	iterador->posicion_actual++;

	if (iterador->posicion_actual == iterador->lista->cantidad_nodos)
		return false;

	return true;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (iterador == NULL ||
	    iterador->lista->cantidad_nodos == LISTA_SIN_NODOS ||
	    iterador->posicion_actual >= iterador->lista->cantidad_nodos) {
		return NULL;
	}

	return iterador->nodo_actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	if (iterador == NULL)
		return;

	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (lista == NULL || funcion == NULL)
		return 0;

	size_t contador = 0;
	nodo_t *nodo = lista->inicio;
	for (size_t i = 0; i < lista->cantidad_nodos; i++) {
		contador++;
		if (!funcion(nodo->elemento, contexto))
			return contador;

		nodo = nodo->siguiente;
	}

	return contador;
}
