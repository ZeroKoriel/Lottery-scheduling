#include "Planificador.h"

void init() {
	listaDeHilos = crearLista();;
	totalTickets = 0;
	/*Unidad de tiempo para el timer, está en milisegundos*/
	timeU = 1000;
	coordenadaX = 400;
	coordenadaY = 50;
	cantidadDeHilos =0;
}

void crearHilo(int terminos, int modo, int id, int tiquetesPorHilo, int quantum, int agujas, int algoritmo, int percent) {
	ethread_t *hilo = calloc(1, sizeof(ethread_t));
	hilo->id = id;
	hilo->i = 0;
	hilo->tiquetes = calloc(tiquetesPorHilo, sizeof(int));

	/*0 expropiativo (asigna quantum) 1 no-expropiativo (asigna porcentaje de trabajo)*/
	hilo->modo = modo;
	hilo->quantum = quantum;
	if (percent != 0 && percent <= 100)
		hilo->porcentajeTrabajo = (double) percent / 100;
	else 
		hilo->porcentajeTrabajo = 0.03; 
	hilo->algoritmo = algoritmo;
	hilo->porcentajeAcumulado = 0.0;
	hilo->cantidadEjecuciones = 0;
	hilo->cantidadTerminos = terminos * 50 * 100;
	hilo->cantidadTiquetes = tiquetesPorHilo;
	hilo->totalAgujas = agujas;
	hilo->totalCortantes = 0;
	hilo->resultado = 0.0;
	hilo->terminado = false;
	hilo->ejecutado = false;

	/*formato para la cantidad de decimales solicitados por el usuario*/
	sprintf(hilo->formato,"%s","%+-#*.");
	char b[100];
	int v = terminos * 50;
	sprintf(b, "%d", v);
	strcat(hilo->formato, b);
	strcat(hilo->formato, "Qe");
	
	hilo->bar = gtk_progress_bar_new ();
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (hilo->bar), 0.0);
	
	insertarFinal(listaDeHilos, hilo);

	totalTickets += tiquetesPorHilo;
}

void calcularPiBuffon(){
	sigsetjmp(hiloEnEjecucion->buffer,1);
	int angulo = 0;
	double valor = 0;
	double recto = sin(180);
	double porcentajeActual = 0.0;
	int iteraciones = hiloEnEjecucion->totalAgujas * hiloEnEjecucion->porcentajeTrabajo;
	hiloEnEjecucion->ejecutado = true;

	sigsetjmp(hiloEnEjecucion->buffer, 1);
	for (hiloEnEjecucion->i = 0; hiloEnEjecucion->i < hiloEnEjecucion->totalAgujas; )
	{
		sigsetjmp(hiloEnEjecucion->buffer, 1);
		iteraciones = (int)hiloEnEjecucion->totalAgujas * hiloEnEjecucion->porcentajeTrabajo;
		hiloEnEjecucion->i++;	
		actualizarBarra((double)hiloEnEjecucion->porcentajeAcumulado);
		gtk_main_iteration();
		angulo = aleatorio(180);
		valor = 2 * sin(angulo);
		
		if (hiloEnEjecucion->i != 0) {
			hiloEnEjecucion->porcentajeAcumulado = (double) hiloEnEjecucion->i / (double) hiloEnEjecucion->totalAgujas;
			hiloEnEjecucion->resultado = (2 *  hiloEnEjecucion->totalAgujas) / hiloEnEjecucion->totalCortantes;
			quadmath_snprintf (hiloEnEjecucion->valorAMostrar, sizeof(hiloEnEjecucion->valorAMostrar), 
			hiloEnEjecucion->formato, 1024, hiloEnEjecucion->resultado);
		}
		
		if (valor > recto) {
			++hiloEnEjecucion->totalCortantes;
		}

		/*Pregunta si está en modo no expropiativo*/
		if (hiloEnEjecucion->modo) {
			if (hiloEnEjecucion->i % iteraciones == 0) {
				actualizarBarra((double)hiloEnEjecucion->porcentajeAcumulado);
				gtk_main_iteration();
				siglongjmp(parent, 1);
			}
		} else {
			int t = currentTimeMillis() - tInicio;
			if (t >= hiloEnEjecucion->quantum) {
				siglongjmp(parent, 1);
			}
		}
	}
	hiloEnEjecucion->terminado = true;
	devolver();
	sigsetjmp(hiloEnEjecucion->buffer, 1);
	siglongjmp(parent, 1);
}

void calcularPiLeibniz() {
	sigsetjmp(hiloEnEjecucion->buffer, 1);
	int iteraciones = (int)hiloEnEjecucion->cantidadTerminos * hiloEnEjecucion->porcentajeTrabajo;
	hiloEnEjecucion->ejecutado = true;
	sigsetjmp(hiloEnEjecucion->buffer, 1);
	for (hiloEnEjecucion->i = 0; hiloEnEjecucion->i < hiloEnEjecucion->cantidadTerminos;)
	{
		sigsetjmp(hiloEnEjecucion->buffer, 1);

		iteraciones = (int)hiloEnEjecucion->cantidadTerminos * hiloEnEjecucion->porcentajeTrabajo;
		
		hiloEnEjecucion->i++;
		hiloEnEjecucion->resultado+= (-4)*pow(-1,hiloEnEjecucion->i)/(2*hiloEnEjecucion->i-1);
		actualizarBarra((double)hiloEnEjecucion->porcentajeAcumulado);
		gtk_main_iteration();
		if (hiloEnEjecucion->i != 0) {
			hiloEnEjecucion->porcentajeAcumulado = (double) hiloEnEjecucion->i / (double) hiloEnEjecucion->cantidadTerminos;
			quadmath_snprintf (hiloEnEjecucion->valorAMostrar, sizeof(hiloEnEjecucion->valorAMostrar), 
			hiloEnEjecucion->formato, 1024, hiloEnEjecucion->resultado);
		}

		if (hiloEnEjecucion->modo) {
			if (hiloEnEjecucion->i % iteraciones == 0) {
				actualizarBarra((double)hiloEnEjecucion->porcentajeAcumulado);
				gtk_main_iteration();
				siglongjmp(parent, 1);
			}
		} else {
			int t = currentTimeMillis() - tInicio;
			if (t >= hiloEnEjecucion->quantum) {
				siglongjmp(parent, 1);
			}
		}
	}
	hiloEnEjecucion->terminado = true;
	devolver();
	sigsetjmp(hiloEnEjecucion->buffer, 1);
	siglongjmp(parent, 1);
}

void devolver() {
	ethread_t *h;

	for (int i = 0; i < listaDeHilos->cantidadNodos; ++i)
	{
		h = obtener(listaDeHilos, i);
		if (!h->terminado) {
			break;
		}
	}
	if (h) {
		int cantidadN = h->cantidadTiquetes + hiloEnEjecucion->cantidadTiquetes;
		int *temp = calloc(cantidadN, sizeof(int));

		/*copia los tiquetes que ya tiene*/
		for (int i = 0; i < h->cantidadTiquetes; ++i)
		{
			temp[i] = h->tiquetes[i];
		}
		int c = 0;
		for (int i = h->cantidadTiquetes; i < cantidadN; ++i)
		{
			temp[i] = hiloEnEjecucion->tiquetes[c++];
		}
		h->tiquetes = temp;
		h->cantidadTiquetes = cantidadN;
		hiloEnEjecucion->cantidadTiquetes = 0;
	}
}

void equilibrarTiquetes() {
	ethread_t *mayor;
	ethread_t *menor;

	mayor = obtener(listaDeHilos, hiloMasTiquetes());
	menor = obtener(listaDeHilos, hiloMenosTiquetes());
	transferirTiquetes(mayor, menor);
}

void transferirTiquetes(ethread_t* s, ethread_t* d) {
	int tamanioS = sizeof(s->tiquetes) / sizeof(s->tiquetes[0]);
	int tamanioD = sizeof(d->tiquetes) / sizeof(d->tiquetes[0]);
	int dif = tamanioS - tamanioD;
	dif /= 2;

	int *dTemp = calloc((tamanioD + dif), sizeof(int));

	/*copia todos los valores de d a dTemp*/
	for (int i = 0; i < tamanioD; ++i)
	{
		dTemp[i] = d->tiquetes[i];
	}
	int c = tamanioS - dif;
	for (int i = tamanioD; i < (tamanioD + dif); ++i)
	{
		dTemp[i] = s->tiquetes[c++];
	}

	d->tiquetes = dTemp;

	int *sTemp = calloc((tamanioS - dif), sizeof(int));
	for (int i = 0; i < (tamanioS - dif); ++i)
	{
		sTemp[i] = s->tiquetes[i];
	}
	s->tiquetes = sTemp;
	d->cantidadTiquetes += dif;
	s->cantidadTiquetes -= dif;
}

int hiloMasTiquetes() {
	/*indice mayor*/
	int indiceMayor;
	int numeroMayor = 0;
	ethread_t *temp;
	for (int i = 0; i < listaDeHilos->cantidadNodos; ++i)
	{
		for (int j = 0; j < listaDeHilos->cantidadNodos; ++j)
		{
			temp = obtener(listaDeHilos, j);
			if (temp->cantidadEjecuciones > numeroMayor) {
				numeroMayor = temp->cantidadEjecuciones;
				indiceMayor = j;
			}
		}
	}
	return indiceMayor;
}

int hiloMenosTiquetes() {
	/*indice mayor*/
	int indiceMenor;
	int numeroMenor = 100000000;
	ethread_t *temp;
	for (int i = 0; i < listaDeHilos->cantidadNodos; ++i)
	{
		for (int j = 0; j < listaDeHilos->cantidadNodos; ++j)
		{
			temp = obtener(listaDeHilos, j);
			if (temp->cantidadEjecuciones < numeroMenor) {
				numeroMenor = temp->cantidadEjecuciones;
				indiceMenor = j;
			}
		}
	}
	return indiceMenor;
}

void actualizarBarra(double avance) {
	gdouble fraction = avance;
  	gtk_label_set_text((GtkLabel*)hiloEnEjecucion->label, hiloEnEjecucion->valorAMostrar);
  	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (hiloEnEjecucion->bar), fraction);
}

void insertarBarras() {
	ethread_t *temp;
	char bufferN[30];
	for (int i = 0; i < listaDeHilos->cantidadNodos; ++i)
	{
		temp = obtener(listaDeHilos, i);
		sprintf(bufferN, "Proceso %d", temp->id);

		
		temp->label = gtk_label_new(bufferN);
		gtk_fixed_put(GTK_FIXED(fixed), temp->label, coordenadaX, coordenadaY-35);
		gtk_widget_set_size_request(temp->label, 75, 30);

		temp->bar = gtk_progress_bar_new();
		gtk_fixed_put(GTK_FIXED(fixed), temp->bar, coordenadaX, coordenadaY);
		coordenadaY += 50;
		++cantidadDeHilos;
		gtk_main_iteration();
	}
	gtk_widget_show_all(window);
}

int insertarNuevo() {
	int valor = listaDeHilos->cantidadNodos - cantidadDeHilos;
	if (valor > 0) {
		ethread_t *temp;
		char bufferN[30];
		for (int i = 0; i < valor; ++i)
		{
			printf("%s\n", "Hilo añadido");
			temp = obtener(listaDeHilos, listaDeHilos->cantidadNodos - valor + i);
			sprintf(bufferN, "Proceso %d", temp->id);
			int c = 0;

			for (int i = totalTickets; i < (totalTickets + temp->cantidadTiquetes); ++i)
			{
				temp->tiquetes[c++] = i;
			}

			totalTickets += temp->cantidadTiquetes;
			temp->label = gtk_label_new(bufferN);
			gtk_fixed_put(GTK_FIXED(fixed), temp->label, coordenadaX, coordenadaY-35);
			gtk_widget_set_size_request(temp->label, 75, 30);

			temp->bar = gtk_progress_bar_new();
			gtk_fixed_put(GTK_FIXED(fixed), temp->bar, coordenadaX, coordenadaY);
			coordenadaY += 50;

			gtk_main_iteration();
			gtk_widget_show_all(window);
		}
		cantidadDeHilos += valor;
		return 1;
	}
	return 0;
}

void scheduler() {
	do
	{
		while (gtk_events_pending ()) {
  			gtk_main_iteration ();
		}
	} while (listaDeHilos->cantidadNodos < 5);
	insertarBarras();
	printf("linea 323\n");
	int *gameTickets = calloc(totalTickets, sizeof(int));

	/*Guarda los tiquetes del 1 en adelante*/
	for (int i = 0; i < totalTickets; ++i)
	{
		gameTickets[i] = i + 1;
	}

	/*Asigna la cantidad correspondiente de tiquetes para cada hilo*/
	ethread_t *temp;
	int inicio = 0;
	int fin = 0;
	for (int i = 0; i < listaDeHilos->cantidadNodos; ++i)
	{
		temp = obtener(listaDeHilos, i);
		/*Se usa para moverse en la cantidad correspondiente los índices*/
		inicio = fin;
		fin += temp->cantidadTiquetes;
		for (int j = 0; j < temp->cantidadTiquetes; ++j)
		{
			temp->tiquetes[j] = gameTickets[inicio++];
		}
	}

	/*Revuelve los tiquetes*/
	for (int i = totalTickets - 1; i >= 0; i--)
	{
		/*Genera un número aleatorio para revolver los tiquetes*/
		int k = aleatorio(i + 1);
		swap(&gameTickets[i], &gameTickets[k]);
	}
ciclo:
	/*Hay que seleccionar los tiquetes de manera aleatoria*/
	for (int k = 0; k < totalTickets; ++k)
	{	
		sigsetjmp(parent, 1);
		gtk_main_iteration();
		k++;
		if (k == totalTickets) {
			k = 0;
			for (int i = totalTickets - 1; i >= 0; i--)
			{
				/*Genera un número aleatorio para revolver los tiquetes*/
				int p = aleatorio(i + 1);
				swap(&gameTickets[i], &gameTickets[p]);
			}
		}

		if (insertarNuevo()) {
			int *tGame = calloc(totalTickets, sizeof(int));
			
			/*Aumenta los tiquetes*/
			for (int i = 0; i < totalTickets; ++i)
			{
				tGame[i] = i + 1;
			}
			gameTickets = tGame;
			for (int i = totalTickets - 1; i >= 0; i--)
			{
				/*Genera un número aleatorio para revolver los tiquetes*/
				int p = aleatorio(i + 1);
				swap(&gameTickets[i], &gameTickets[p]);
			}
		}

		//printf("Antes de escoger un tiquete i: %d\n", k);
		int tiqueteEscogido = gameTickets[k];
		int idHilo;
		int encontrado = 0;
		ethread_t *hiloTemp;

		for (idHilo = 0; idHilo < listaDeHilos->cantidadNodos; ++idHilo) 
		{
			hiloTemp = obtener(listaDeHilos, idHilo);
			for (int i = 0; i < hiloTemp->cantidadTiquetes; ++i)
			{
				if (hiloTemp->tiquetes[i] == tiqueteEscogido) {
					encontrado = 1;
					break;
				}
			}
			if (encontrado) {
				break;
			}
		}

		hiloEnEjecucion = hiloTemp;
		if (!hiloEnEjecucion->terminado) {
			/*Validad si ya se ejecutó y si no lo ha hecho, calcular pi*/
			hiloEnEjecucion->cantidadEjecuciones++;
			equilibrarTiquetes();
			tInicio = currentTimeMillis();
			if (hiloEnEjecucion->ejecutado) {
				siglongjmp(hiloEnEjecucion->buffer, 1);
			} else {
				if (hiloEnEjecucion->algoritmo) {
					calcularPiBuffon();
				} else {
					calcularPiLeibniz();
				}
				
			}
		}
		while (gtk_events_pending()) {
			gtk_main_iteration();
		}
	}
	ethread_t* t;
	for (int i = 0; i < listaDeHilos->cantidadNodos; ++i)
	{
		t = obtener(listaDeHilos, i);
		if (!t->terminado) {
			equilibrarTiquetes();
			goto ciclo;
		}
	}
	printf("Saliendo del método scheduler\n");
}	

int fastrand(int s) {
    s = (214013 * s + 2531011);
    return (s >> (rand() % 19) + 1) & (32767 + (rand() % 100000)); 
} 

int aleatorio(int valor) {
    return (fastrand(time(NULL)) % valor) + 1;
}

/*Intercambia los valores por si se necesita revolver los tiquetes*/
void swap (int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int64_t currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);
  int64_t s1 = (int64_t)(time.tv_sec) * 1000;
  int64_t s2 = (time.tv_usec / 1000);
  return s1 + s2;
}