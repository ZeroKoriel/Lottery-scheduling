#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H 

#include <math.h>
#include <float.h>
#include <quadmath.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#include <assert.h>
#include <unistd.h>
/*Buscar bien cómo funcionan*/
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>

#include "Variables.h"
#include "Lista.h"

Lista *listaDeHilos;
static int totalTickets;

/*Estructura para guardar el avance del scheduler*/
sigjmp_buf parent;
/*
*	Se usa para pasar un método como atributo de otro.
*	Lo uso para el manejador del timer. Para cuando se 
*	acaba el tiempo
*/
void (*call)(int);
int tInicio;
int cantidadDeHilos;
/*Se usa para controlar el timer*/
struct itimerval timer;

/*Unidad de tiempo para el timer*/
int timeU;

int coordenadaX;
int coordenadaY;

typedef struct etrhead_t
{
	int id;
	int i;

	/*0 expropiativo 1 no-expropiativo*/
	int modo;
	
	/*0 Leibniz 1 buffon*/
	int algoritmo;

	int quantum;
	int cantidadEjecuciones;
	int cantidadTerminos;
	int cantidadTiquetes;
	
	/*Vector con los tiquetes asignados para cada hilo*/
	int *tiquetes;

	/*string en donde se va a pegar el resultado de pi*/
	char valorAMostrar[1024];
	char formato[100];
	bool terminado;
	bool ejecutado;
	
	double porcentajeTrabajo;
	double porcentajeAcumulado;
	
	__float128 resultado;
	__float128 totalAgujas;
	__float128 totalCortantes;

	/*Atributos para mostrar el avance del cálculo*/
	GtkWidget *bar;
	GtkWidget *label;

	/*Buscar bien cómo funciona*/
	sigjmp_buf buffer;
}ethread_t;

ethread_t *hiloEnEjecucion;

void init();
/*Terminos, expropiatio o no*/
void crearHilo(int, int, int, int, int, int, int, int);
void calcularPiBuffon();
void calcularPiLeibniz();

/*
*	pasa los tiquetes del proceso que termina 
*	al primero que encuentre que continúne en 
*  	ejecución
*/
void devolver();
void scheduler();
void transferirTiquetes(ethread_t*, ethread_t*);
void equilibrarTiquetes();

/*Devuelven el índice*/
int hiloMasTiquetes();
int hiloMenosTiquetes();

int fastrand(int);
int aleatorio(int);
void swap(int*, int*);
void actualizarBarra(double);
void insertarBarras();
int insertarNuevo();
int64_t currentTimeMillis();

#endif