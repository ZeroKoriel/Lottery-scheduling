#ifndef LISTA_H
#define LISTA_H 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "Nodo.h"

typedef struct Lista 
{
	Nodo* head;
	int cantidadNodos;
}Lista;

Lista* crearLista();
void destruirLista(Lista*);
void insertarInicio(Lista*, void*);
void insertarFinal(Lista*, void*);
void insertarDespues(Lista*, void*, int);
void* obtener(Lista*, int);
int size(Lista*);
void eleminarInicio(Lista*);
void eliminarFinal(Lista*);
void eliminarElemento(Lista*, int);
bool estaVacia(Lista*);
#endif