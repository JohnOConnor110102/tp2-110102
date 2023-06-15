#include "pokemon.h"
#include <string.h>
#include <stdio.h>
#include "pokemon_privado.h"

#define FORMATO_LECTURA_STRING "%lu,%[^,],%lu,%s"
const int CANT_CAMPOS_POKEMON_T = 4;
const int STRING_VACIO = 0;
const int STRINGS_DISTINTOS = 0;

pokemon_t *pokemon_crear_desde_string(const char *string)
{
	if (string == NULL || strlen(string) == STRING_VACIO)
		return NULL;

	pokemon_t *poke_leido = malloc(sizeof(pokemon_t));
	if (poke_leido == NULL)
		return NULL;

	int leido = sscanf(string, FORMATO_LECTURA_STRING, &(poke_leido->id),
			   poke_leido->nombre, &(poke_leido->salud),
			   poke_leido->nombre_entrenador);

	if (leido == CANT_CAMPOS_POKEMON_T)
		return poke_leido;

	pokemon_destruir(poke_leido);
	return NULL;
}

pokemon_t *pokemon_copiar(pokemon_t *poke)
{
	if (poke == NULL)
		return NULL;

	pokemon_t *nuevo_poke = malloc(sizeof(pokemon_t));
	if (nuevo_poke == NULL)
		return NULL;

	nuevo_poke->id = poke->id;
	nuevo_poke->salud = poke->salud;
	strcpy(nuevo_poke->nombre, poke->nombre);
	strcpy(nuevo_poke->nombre_entrenador, poke->nombre_entrenador);

	return nuevo_poke;
}

bool pokemon_son_iguales(pokemon_t *pokemon1, pokemon_t *pokemon2)
{
	if (pokemon1 == NULL || pokemon2 == NULL)
		return false;

	if (pokemon1->id != pokemon2->id ||
	    pokemon1->salud != pokemon2->salud ||
	    strcmp(pokemon1->nombre, pokemon2->nombre) != STRINGS_DISTINTOS ||
	    strcmp(pokemon1->nombre_entrenador, pokemon2->nombre_entrenador) !=
		    STRINGS_DISTINTOS)
		return false;

	return true;
}

char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre;
}

char *pokemon_entrenador(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre_entrenador;
}

size_t pokemon_salud(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;

	return pokemon->salud;
}

size_t pokemon_id(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return 0;

	return pokemon->id;
}

void pokemon_destruir(pokemon_t *pkm)
{
	free(pkm);
}
