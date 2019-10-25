#include "Lista.h"

Lista* crearLista() {
	Lista* lista = calloc(1, sizeof(Lista));
	lista->head = NULL;
	lista->cantidadNodos = 0;
	return lista;
}

void destruirLista(Lista* lista) {
	while (lista->head) {
		Nodo* eliminado = lista->head;
		lista->head = lista->head->siguiente;
		--lista->cantidadNodos;
		destruirNodo(eliminado);
	}
}

void insertarInicio(Lista* lista, void* data) {
	Nodo* nodo = crearNodo(data);
	nodo->siguiente = lista->head;
	lista->head = nodo;
	++lista->cantidadNodos;
}

void insertarFinal(Lista* lista, void* data) {
	Nodo* nodo = crearNodo(data);
	Nodo* puntero = lista->head;
	++lista->cantidadNodos;
	if (lista->head == NULL) {
		lista->head = nodo;
	} else {
		while(puntero->siguiente) {
			puntero = puntero->siguiente;
		}
		puntero->siguiente = nodo;
	}
}

void insertarDespues(Lista* lista, void* data, int n) {
	Nodo* nodo = crearNodo(data);
	++lista->cantidadNodos;
	if (lista->head == NULL) {
		lista->head = nodo;
	} else {
		Nodo* puntero = lista->head;
		int c = 0;
		while (c < n && puntero->siguiente) {
			puntero = puntero->siguiente;
			++c;
		}	
		nodo->siguiente = puntero->siguiente;
		puntero->siguiente = nodo;
	}
}

void* obtener(Lista* lista, int n) {
	if (lista->head == NULL) {
		return NULL;
	} 
	Nodo* puntero = lista->head;
	int c = 0;
	while (c < n && puntero->siguiente) {
		puntero = puntero->siguiente;
		++c;
	}
	if (c != n) {
		return NULL;
	}
	return puntero->data;
}

int size(Lista* lista) {
	return lista->cantidadNodos;
}

void eleminarInicio(Lista* lista) {
	if (lista->head) {
		Nodo* eliminado = lista->head;
		lista->head = lista->head->siguiente;
		destruirNodo(eliminado);
		--lista->cantidadNodos;
	}
}

void eliminarFinal(Lista* lista) {
	if (lista->head) {
		--lista->cantidadNodos;
		if (lista->head->siguiente) {
			Nodo* puntero = lista->head;
			while(puntero->siguiente->siguiente) {
				puntero = puntero->siguiente;
			}
			Nodo* eliminado = puntero->siguiente;
			puntero->siguiente = NULL;
			destruirNodo(eliminado);
		} else {
			Nodo* eliminado = lista->head;
			lista->head = NULL;
			destruirNodo(eliminado);
		}
	}
}

void eliminarElemento(Lista* lista, int n) {
	if (lista->head) {
		--lista->cantidadNodos;
		if (n == 0) {
			Nodo* eliminado = lista->head;
			lista->head = lista->head->siguiente;
			destruirNodo(eliminado);
		} else if (n < lista->cantidadNodos) {
			Nodo* puntero = lista->head;
			int c = 0;
			while (c < (n - 1)) {
				puntero = puntero->siguiente;
				++c;
			}
			Nodo* eliminado = puntero->siguiente;
			puntero->siguiente = eliminado->siguiente;
			destruirNodo(eliminado);
			
		}
	}
}

bool estaVacia(Lista* lista) {
	return lista->head == NULL;	
}