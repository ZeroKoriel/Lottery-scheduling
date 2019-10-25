#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Planificador.h"

#define FPS 6
#define WIDTH 800
#define HEIGTH 620

static gint64 last_tick = 0;
static gint64 tick_cb = 0;
static gint64 sizeL = 32;

int modo;
int algoritmo;

gint delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data ){
    g_print ("Cerrando la ventana\n");
    exit(0);
    return true;
}

static gboolean on_tick (gpointer user_data) {
    gint64 current = g_get_real_time ();
    gboolean changed = FALSE;
    if ((current - last_tick) < (1000/ FPS)) {
        last_tick = current;
        return G_SOURCE_CONTINUE;
    }

    gtk_widget_queue_draw_area(drawing, 0, 0, WIDTH, HEIGTH);

    last_tick = current;
    return G_SOURCE_CONTINUE;
}

static void on_draw (GtkWidget *widget, cairo_t *cr, gpointer user_data) {

}

static void on_changedModo (GtkComboBox *widget, gpointer user_data) {
  GtkComboBox *combo_box = widget;

  int opcion = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));

  switch(opcion) {
    case 1: ///Expropiativo/
        modo = 0;
        break;
    case 2://No-Expropiativo/
        modo = 1;
        break;
    default: printf("Modo no soportado\n");
        break;
  } 
}

static void on_changedAlgoritmo(GtkComboBox *widget, gpointer user_data) {
    GtkComboBox *combo_box = widget;

    int opcion = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));

    switch(opcion) {
        case 1: /*Leibniz*/
            algoritmo = 0;
            break;
        case 2:  /*Buffon*/
            algoritmo = 1;
            break;
        default: printf("Algoritmo no soportado\n");
    } 
}

static void crear (GtkWidget *widget, gpointer   user_data)
{
    int terminos = atoi(gtk_entry_get_text((GtkEntry *)entryCantidadTrabajo));
    int tiquetesPorHilo = atoi(gtk_entry_get_text((GtkEntry *)entryCantidadTiquetes));
    int quantum = atoi(gtk_entry_get_text((GtkEntry *)entryTamanioQuantum));
    int id = atoi(gtk_entry_get_text((GtkEntry *)entryId));
    int agujas = atoi(gtk_entry_get_text((GtkEntry *)entryAgujas));
    int porcentaje = atoi(gtk_entry_get_text((GtkEntry*)entryPorcentaje));

    //crearHilo(terminos, modo, id, tiquetesPorHilo, quantum);
    crearHilo(terminos, modo, id, tiquetesPorHilo, quantum, agujas, algoritmo, porcentaje);
    gtk_entry_set_text((GtkEntry *)entryTamanioQuantum, " ");
    gtk_entry_set_text((GtkEntry *)entryCantidadTiquetes, " ");
    gtk_entry_set_text((GtkEntry *)entryCantidadTrabajo, " ");
    gtk_entry_set_text((GtkEntry *)entryId, " ");
    gtk_entry_set_text((GtkEntry *)entryAgujas, " ");
    gtk_entry_set_text((GtkEntry *)entryPorcentaje, " ");
}

void initInterfaz() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK (delete_event), NULL);
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGTH);
    gtk_window_set_title(GTK_WINDOW(window), "Lotery shedulling");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    drawing = gtk_drawing_area_new();
    gtk_fixed_put(GTK_FIXED(fixed), drawing, 0, 0);
    gtk_widget_set_size_request(drawing, WIDTH, HEIGTH); 
    g_signal_connect(drawing, "draw", G_CALLBACK(on_draw), NULL);

    ///Funcionalidad/
    comboAlgoritmo = gtk_combo_box_text_new ();
    const char *al[] = {"Algoritmo", "Leibniz", "Buffon"};
    for (int i = 0; i < G_N_ELEMENTS (al); i++){
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (comboAlgoritmo), al[i]);
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (comboAlgoritmo), 0);
    g_signal_connect (comboAlgoritmo, "changed", G_CALLBACK (on_changedAlgoritmo), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), comboAlgoritmo, 50, 50);
    gtk_widget_set_size_request(comboAlgoritmo, 80, 30);

    comboModo = gtk_combo_box_text_new ();
    const char *algoritmos[] = {"Modo", "Expropiativo", "No-Expropiativo"};
    for (int i = 0; i < G_N_ELEMENTS (algoritmos); i++){
        gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (comboModo), algoritmos[i]);
    }
    gtk_combo_box_set_active (GTK_COMBO_BOX (comboModo), 0);
    g_signal_connect (comboModo, "changed", G_CALLBACK (on_changedModo), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), comboModo, 50, 100);
    gtk_widget_set_size_request(comboModo, 80, 30);

    labelCantidadTiquetes = gtk_label_new("Tiquetes");
    gtk_fixed_put(GTK_FIXED(fixed), labelCantidadTiquetes, 50, 150);
    gtk_widget_set_size_request(labelCantidadTiquetes, 75, 30);

    entryCantidadTiquetes = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entryCantidadTiquetes, 150, 150);
    gtk_widget_set_size_request(entryCantidadTiquetes, 75, 30); 

    labelCantidadTrabajo = gtk_label_new("Trabajo");
    gtk_fixed_put(GTK_FIXED(fixed), labelCantidadTrabajo, 50, 200);
    gtk_widget_set_size_request(labelCantidadTrabajo, 75, 30);

    entryCantidadTrabajo = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entryCantidadTrabajo, 150, 200);
    gtk_widget_set_size_request(entryCantidadTrabajo, 75, 30);

    labelTamanioQuantum = gtk_label_new("Quantum");
    gtk_fixed_put(GTK_FIXED(fixed), labelTamanioQuantum, 50, 250);
    gtk_widget_set_size_request(labelTamanioQuantum, 75, 30);

    entryTamanioQuantum = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entryTamanioQuantum, 150, 250);
    gtk_widget_set_size_request(entryTamanioQuantum, 75, 30);

    labelId = gtk_label_new("Id");
    gtk_fixed_put(GTK_FIXED(fixed), labelId, 50, 300);
    gtk_widget_set_size_request(labelId, 75, 30);

    entryId = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entryId, 150, 300);
    gtk_widget_set_size_request(entryId, 75, 30);

    labelAgujas = gtk_label_new("Agujas");
    gtk_fixed_put(GTK_FIXED(fixed), labelAgujas, 50, 350);
    gtk_widget_set_size_request(labelAgujas, 75, 30);

    entryAgujas = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entryAgujas, 150, 350);
    gtk_widget_set_size_request(entryAgujas, 75,30);

    bottonCrear = gtk_button_new_with_label("Crear hilo");
    g_signal_connect(bottonCrear, "clicked", G_CALLBACK(crear), NULL);
    gtk_widget_set_size_request(bottonCrear, 75, 30);
    gtk_fixed_put(GTK_FIXED(fixed), bottonCrear, 150, 450);

    char *mensaje = calloc(1,sizeof(char));
    mensaje =    "Nota:\n** Para la función de Leibniz el trabajo se\nconvierte a iteraciones: trabajo * 50 * 1000.\n** Para la función de Buffon las agujas\nrepresentan las iteraciones.";

    labelMensaje = gtk_label_new(mensaje);
    gtk_fixed_put(GTK_FIXED(fixed), labelMensaje, 50, 500);
    gtk_widget_set_size_request(labelMensaje, 100, 100);

    labelPorcentaje = gtk_label_new("Porcentaje");
    gtk_fixed_put(GTK_FIXED(fixed), labelPorcentaje, 50, 400);
    gtk_widget_set_size_request(labelPorcentaje, 75, 30);

    entryPorcentaje = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entryPorcentaje, 150, 400);
    gtk_widget_set_size_request(entryPorcentaje, 75, 30);

    //fin funcionalidad/
    gtk_widget_show_all(window);
    tick_cb = g_timeout_add(1000 / FPS / 2, (GSourceFunc) on_tick, GINT_TO_POINTER(sizeL)); 
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    initInterfaz();
    init();
    scheduler();
    while (1) {
        gtk_main_iteration();
    }
    return 0;
}