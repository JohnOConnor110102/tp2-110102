#include "pa2m.h"
#include "TDAs/menu.h"

void pruebas_menu_crear()
{
	menu_t *menu = menu_crear();
	pa2m_afirmar(!!menu, "Se puede crear un menú.");
	pa2m_afirmar(menu->cantidad_comandos == 0,
		     "La cantidad de comandos inicial es cero.");
	pa2m_afirmar(!!menu->comandos,
		     "La lista de comandos se crea correctamente.");
	menu_destruir(menu);
}

bool devolver_true(void *_ptr1, void *_ptr2)
{
	return true;
}

bool contexto_en_uno(void *_menu, void *_contexto)
{
	if (!_menu)
		return false;

	int *contexto = (int *)_contexto;
	*contexto = 1;
	return true;
}

void pruebas_menu_agregar_comando()
{
	menu_t *menu = menu_crear();
	pa2m_afirmar(!menu_agregar_comando(NULL, "juancito", "j",
					   "un buen comando", devolver_true,
					   (void *)false),
		     "No se puede agregar un comando con un menú nulo.");
	pa2m_afirmar(!menu_agregar_comando(menu, NULL, "j", "un buen comando",
					   devolver_true, (void *)false),
		     "No se puede agregar un comando con nombre nulo.");
	pa2m_afirmar(
		!menu_agregar_comando(menu, "juancito", NULL, "un buen comando",
				      devolver_true, (void *)false),
		"No se puede agregar un comando con una instrucción nula.");
	pa2m_afirmar(
		!menu_agregar_comando(menu, "juancito", "j", NULL,
				      devolver_true, (void *)false),
		"No se puede agregar un comando con una descripción nula.");
	pa2m_afirmar(!menu_agregar_comando(menu, "juancito", "j",
					   "un buen comando", NULL,
					   (void *)false),
		     "No se puede agregar un comando con una función nula.");
	menu = menu_agregar_comando(menu, "juancito", "j", "un buen comando",
				    devolver_true, NULL);
	comando_t *comando = menu_obtener_comando(menu, "j");
	pa2m_afirmar(
		!!menu && strcmp(comando->instruccion, "j") == 0 &&
			menu_cantidad_comandos(menu) == 1,
		"Se puede agregar y se encuentra un comando con un auxiliar nulo.");
	pa2m_afirmar(
		!menu_agregar_comando(menu, "juancito", "j", "un buen comando",
				      devolver_true, (void *)false),
		"No se puede agregar un comando con la misma instrucción que un comando existente.");

	menu = menu_agregar_comando(menu, "comando2", "2", "comando2",
				    devolver_true, (void *)true);
	comando_t *comando1 = menu_obtener_comando(menu, "2");
	pa2m_afirmar(
		!!menu && strcmp(comando1->instruccion, "2") == 0 &&
			menu_cantidad_comandos(menu) == 2,
		"Se puede agregar y se encuentra un comando con parámetros válidos.\n");
	menu_destruir(menu);
}

void pruebas_menu_ejecutar_comando()
{
	menu_t *menu = menu_crear();
	int aux = 0;
	menu_agregar_comando(menu, "contexto igual a uno", "s",
			     "inicializa al contexto en 1", contexto_en_uno,
			     &aux);
	pa2m_afirmar(!menu_ejecutar_comando(NULL, "s") && aux == 0,
		     "No se puede ejecutar un comando con un menú nulo.");
	pa2m_afirmar(
		!menu_ejecutar_comando(menu, NULL) && aux == 0,
		"No se puede ejecutar un comando con una instrucción nula.");
	pa2m_afirmar(!!menu_ejecutar_comando(menu, "s") && aux == 1,
		     "Se puede ejecutar un comando correctamente.\n");
	menu_destruir(menu);
}

void pruebas_menu_contiene_comando()
{
	menu_t *menu = menu_crear();
	int aux = 0;
	menu_agregar_comando(menu, "contexto igual a uno", "s",
			     "inicializa al contexto en 1", contexto_en_uno,
			     &aux);
	pa2m_afirmar(!menu_contiene_comando(NULL, "s"),
		     "Devuelve false si el menú es nulo.");
	pa2m_afirmar(!menu_contiene_comando(menu, NULL),
		     "Devuelve false si la instrucción es nula.");
	pa2m_afirmar(menu_contiene_comando(menu, "s"),
		     "Devuelve true si el comando existe.\n");
	menu_destruir(menu);
}

void pruebas_menu_obtener_comando()
{
	menu_t *menu = menu_crear();
	int aux = 0;
	menu_agregar_comando(menu, "contexto igual a uno", "s",
			     "inicializa al contexto en 1", contexto_en_uno,
			     &aux);
	pa2m_afirmar(!menu_obtener_comando(NULL, "s"),
		     "No se puede obtener un comando con un menú nulo.");
	pa2m_afirmar(
		!menu_obtener_comando(menu, NULL),
		"No se puede obtener un comando con una instrucción nula.");
	comando_t *comando = menu_obtener_comando(menu, "s");
	pa2m_afirmar(comando->contexto == &aux &&
			     strcmp(comando->descripcion,
				    "inicializa al contexto en 1") == 0 &&
			     strcmp(comando->instruccion, "s") == 0 &&
			     strcmp(comando->nombre, "contexto igual a uno") ==
				     0,
		     "Se obtiene el comando correctamente.\n");
	menu_destruir(menu);
}

void pruebas_menu_vacio()
{
	menu_t *menu = menu_crear();
	int aux = 0;
	pa2m_afirmar(menu_vacio(NULL), "Devuelve true con un menú nulo.");
	pa2m_afirmar(menu_vacio(menu), "Devuelve true si el menú está vacío.");
	menu_agregar_comando(menu, "contexto igual a uno", "s",
			     "inicializa al contexto en 1", contexto_en_uno,
			     &aux);
	pa2m_afirmar(!menu_vacio(menu),
		     "Devuelve false si el menú no está vacío.\n");
	menu_destruir(menu);
}

void pruebas_menu_cantidad_comandos()
{
	menu_t *menu = menu_crear();
	int aux = 0;
	pa2m_afirmar(menu_cantidad_comandos(NULL) == 0,
		     "Devuelve 0 si el menú es nulo.");
	pa2m_afirmar(menu_cantidad_comandos(menu) == 0,
		     "Devuelve 0 si el menú está vacío.");
	menu_agregar_comando(menu, "contexto igual a uno", "s",
			     "inicializa al contexto en 1", contexto_en_uno,
			     &aux);
	menu_agregar_comando(menu, "un comando honesto", "v",
			     "solo dice la verdad", devolver_true, &aux);
	pa2m_afirmar(menu_cantidad_comandos(menu) == 2,
		     "Devuelve la cantidad correcta de comandos.\n");
	menu_destruir(menu);
}

void pruebas_menu_eliminar_comando()
{
	menu_t *menu = menu_crear();
	int aux = 0;
	menu_agregar_comando(menu, "contexto igual a uno", "s",
			     "inicializa al contexto en 1", contexto_en_uno,
			     &aux);
	pa2m_afirmar(!menu_eliminar_comando(NULL, "s"),
		     "No se puede eliminar un comando con un menú nulo.");
	pa2m_afirmar(!menu_eliminar_comando(menu, NULL),
		     "No se puede eliminar un comando con instrucción nula.");
	pa2m_afirmar(!!menu_eliminar_comando(menu, "s") &&
			     menu_cantidad_comandos(menu) == 0,
		     "Se puede eliminar un comando correctamente.\n");
	menu_destruir(menu);
}

void pruebas_de_lista()
{
	pa2m_nuevo_grupo("Pruebas `menu_crear`");
	pruebas_menu_crear();
	pa2m_nuevo_grupo("Pruebas `menu_agregar_comando`");
	pruebas_menu_agregar_comando();
	pa2m_nuevo_grupo("Pruebas `menu_ejecutar_comando`");
	pruebas_menu_ejecutar_comando();
	pa2m_nuevo_grupo("Pruebas `menu_obtener_comando`");
	pruebas_menu_obtener_comando();
	pa2m_nuevo_grupo("Pruebas `menu_contiene_comando`");
	pruebas_menu_contiene_comando();
	pa2m_nuevo_grupo("Pruebas `menu_vacio`");
	pruebas_menu_vacio();
	pa2m_nuevo_grupo("Pruebas `menu_cantidad_comandos`");
	pruebas_menu_cantidad_comandos();
	pa2m_nuevo_grupo("Pruebas `menu_eliminar_comando`");
	pruebas_menu_eliminar_comando();
}

int main()
{
	pa2m_nuevo_grupo(
		"================= PRUEBAS TDA MENÚ =================");
	pruebas_de_lista();

	return pa2m_mostrar_reporte();
}
