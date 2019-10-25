#include "Nodo.h"

Nodo* crearNodo(void* data) {
	Nodo* nodo = calloc(1, sizeof(Nodo));
	nodo->data = data;
	nodo->siguiente = NULL;
	nodo->anterior = NULL;
	return nodo;
}

void destruirNodo(Nodo* nodo) {
	nodo->data = NULL;
	nodo->siguiente = NULL;
	nodo->anterior = NULL;
	free(nodo);
}