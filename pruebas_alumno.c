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

void pruebas_menu_agregar_comando()
{
	menu_t *menu = menu_crear();
	pa2m_afirmar(!menu_agregar_comando(NULL, "juancito", "j",
					   "un buen comando", devolver_true,
					   (void *)false),
		     "No se puede agregar un comando en un hospital nulo.");
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
		     "No se puede agregar un comando con una funcion nula.");
	pa2m_afirmar(!!menu_agregar_comando(menu, "juancito", "j",
					    "un buen comando", devolver_true,
					    NULL),
		     "Se puede agregar un comando con un auxiliar nulo.");
	pa2m_afirmar(
		!menu_agregar_comando(menu, "juancito", "j", "un buen comando",
				      devolver_true, (void *)false),
		"No se puede agregar un comando con la misma instrucción que un comando existente.");
	pa2m_afirmar(!!menu_agregar_comando(menu, "comando2", "2", "comando2",
					    devolver_true, (void *)true),
		     "Se puede agregar un comando con parámetros válidos.\n");
	menu_destruir(menu);
}

void pruebas_de_lista()
{
	pa2m_nuevo_grupo("Pruebas `menu_crear`");
	pruebas_menu_crear();
	pa2m_nuevo_grupo("Pruebas `menu_agregar_comando`");
	pruebas_menu_agregar_comando();
}

int main()
{
	pa2m_nuevo_grupo(
		"================= PRUEBAS TDA MENÚ =================");
	pruebas_de_lista();

	return pa2m_mostrar_reporte();
}
