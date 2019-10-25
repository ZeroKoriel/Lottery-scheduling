#ifndef NODO_H
#define NODO_H 

#include <stdlib.h>

typedef struct Nodo
{
	struct Nodo* siguiente;
	struct Nodo* anterior;
	void* data;
}Nodo;

Nodo* crearNodo(void* data);
void destruirNodo(Nodo* nodo);

#endif