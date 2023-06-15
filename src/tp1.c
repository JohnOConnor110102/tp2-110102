#include "tp1.h"

#include "pokemon.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FORMATO_LECTURA_ARCHIVO "%s"
#define FIN_DE_LINEA "\n"
#define RETURN_FUNCION 1

const int LINEA_LEIDA = 1;
const size_t MAX_CARACTERES_LINEA = 30;
const int HOSPITAL_VACIO = 0;

struct _hospital_pkm_t {
	pokemon_t **pokemones;
	size_t cantidad_pokemon;
	size_t cantidad_entrenadores;
};

void insertar_pokemones_ordenadamente(hospital_t *hospital,
				      pokemon_t **pokemones_ambulancia,
				      size_t cant_pokes_ambulancia)
{
	if (hospital == NULL || pokemones_ambulancia == NULL)
		return;

	for (size_t i = 0; i < cant_pokes_ambulancia; i++) {
		int indice_a_insertar = 0;
		int j = 0;
		bool indice_determinado = false;
		while (j < hospital->cantidad_pokemon && !indice_determinado) {
			if (pokemon_salud(hospital->pokemones[j]) <=
			    pokemon_salud(pokemones_ambulancia[i])) {
				indice_a_insertar++;
			} else {
				indice_determinado = true;
			}
			j++;
		}

		for (size_t k = hospital->cantidad_pokemon;
		     k > indice_a_insertar; k--) {
			hospital->pokemones[k] = hospital->pokemones[k - 1];
		}

		hospital->pokemones[indice_a_insertar] =
			pokemones_ambulancia[i];
		hospital->cantidad_pokemon++;
	}
}

void ordenar_hospital(hospital_t *hospital)
{
	if (hospital == NULL)
		return;

	bool esta_ordenado = false;
	while (!esta_ordenado) {
		esta_ordenado = true;
		for (int i = 0; i < (hospital->cantidad_pokemon) - 1; i++) {
			if (pokemon_salud(hospital->pokemones[i + 1]) <
			    pokemon_salud(hospital->pokemones[i])) {
				esta_ordenado = false;
				pokemon_t *aux = hospital->pokemones[i];
				hospital->pokemones[i] =
					hospital->pokemones[i + 1];
				hospital->pokemones[i + 1] = aux;
			}
		}
	}
}

void aborto_crear_hospital(FILE *archivo, hospital_t *hospital)
{
	if (archivo == NULL || hospital == NULL)
		return;

	fclose(archivo);
	hospital_destruir(hospital);
}

bool ingresar_pokemones_desde_archivo(FILE *archivo, hospital_t *hospital)
{
	if (archivo == NULL || hospital == NULL)
		return false;

	char linea_leida[MAX_CARACTERES_LINEA];
	int leido = fscanf(archivo, FORMATO_LECTURA_ARCHIVO, linea_leida);
	if (leido != LINEA_LEIDA) {
		aborto_crear_hospital(archivo, hospital);
		return false;
	}

	int i = 0;
	while (leido == LINEA_LEIDA) {
		if (hospital->cantidad_pokemon > HOSPITAL_VACIO) {
			pokemon_t **pokemones_aux =
				realloc(hospital->pokemones,
					(hospital->cantidad_pokemon + 1) *
						sizeof(pokemon_t *));
			if (pokemones_aux == NULL) {
				aborto_crear_hospital(archivo, hospital);
				return false;
			}
			hospital->pokemones = pokemones_aux;
		}
		hospital->pokemones[i] =
			pokemon_crear_desde_string(linea_leida);
		if (hospital->pokemones[i] == NULL) {
			aborto_crear_hospital(archivo, hospital);
			return false;
		}
		hospital->cantidad_pokemon++;
		hospital->cantidad_entrenadores++;

		i++;
		leido = fscanf(archivo, FORMATO_LECTURA_ARCHIVO, linea_leida);
	}
	return true;
}

hospital_t *hospital_crear_desde_archivo(const char *nombre_archivo)
{
	if (nombre_archivo == NULL)
		return NULL;

	FILE *archivo = fopen(nombre_archivo, "r");
	if (archivo == NULL)
		return NULL;

	hospital_t *hospital = malloc(sizeof(hospital_t));
	if (hospital == NULL) {
		fclose(archivo);
		return NULL;
	}
	hospital->cantidad_entrenadores = 0;
	hospital->cantidad_pokemon = 0;

	hospital->pokemones = malloc(sizeof(pokemon_t *));
	if (hospital->pokemones == NULL) {
		aborto_crear_hospital(archivo, hospital);
		return NULL;
	}

	bool estado = ingresar_pokemones_desde_archivo(archivo, hospital);
	if (estado == false)
		return NULL;

	fclose(archivo);
	ordenar_hospital(hospital);

	return hospital;
}

size_t hospital_cantidad_pokemones(hospital_t *hospital)
{
	if (hospital == NULL)
		return 0;

	return hospital->cantidad_pokemon;
}

size_t hospital_a_cada_pokemon(hospital_t *hospital,
			       bool (*funcion)(pokemon_t *p, void *aux),
			       void *aux)
{
	if (hospital == NULL || funcion == NULL)
		return 0;

	size_t contador = 0;
	for (int i = 0; i < hospital->cantidad_pokemon; i++) {
		contador++;
		if (!funcion(hospital->pokemones[i], aux)) {
			return contador;
		}
	}

	return contador;
}

int hospital_aceptar_emergencias(hospital_t *hospital,
				 pokemon_t **pokemones_ambulancia,
				 size_t cant_pokes_ambulancia)
{
	if (hospital == NULL || pokemones_ambulancia == NULL)
		return ERROR;

	pokemon_t **pokemones_aux =
		realloc(hospital->pokemones,
			(hospital->cantidad_pokemon + cant_pokes_ambulancia) *
				sizeof(char *));

	if (pokemones_aux == NULL) {
		hospital_destruir(hospital);
		return ERROR;
	}
	hospital->pokemones = pokemones_aux;

	insertar_pokemones_ordenadamente(hospital, pokemones_ambulancia,
					 cant_pokes_ambulancia);
	return EXITO;
}

pokemon_t *hospital_obtener_pokemon(hospital_t *hospital, size_t prioridad)
{
	if (hospital == NULL || prioridad >= hospital->cantidad_pokemon)
		return NULL;

	return hospital->pokemones[prioridad];
}

void hospital_destruir(hospital_t *hospital)
{
	for (size_t i = 0; i < hospital->cantidad_pokemon; i++) {
		free(hospital->pokemones[i]);
	}
	free(hospital->pokemones);
	free(hospital);
}