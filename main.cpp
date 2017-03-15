#include <iostream>              //Incluir la librería iostream
#include <string>                //Incluir la librería string
#include <thread>                //Incluir la librería thread
#include <stdlib.h>              //Incluir la librería stdlib.h
#include <errno.h>               //Incluir la librería errno.h
#include <gtk/gtk.h>             //Incluir la librería gtk
#include <gdk/gdkx.h>            //Incluir la librería ddk
#include <vlc/vlc.h>             //Incluir la librería vlc

#include "include/usb.h"         //Incluir la cabezera usb.h
#include "include/initialize.h"  //Incluir la cabezera initialize.h
#include "include/thread.h"      //Incluir la cabezera thread.h
#include "include/samsung.h"     //Incluir la cabezera samsung.h

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------
void funcion_de_llamada_destruir(GtkWidget *widget, gpointer data);             //Función de llamada para destruir todos los widget cuando la ventana se cierre
void funcion_de_llamada_reproductor_widget(GtkWidget *widget, gpointer data);   //Función de llamada para insertar el reproductor
void funcion_de_llamada_hilo_recepcion();                                       //Función de llamada para comenzar el hilo de recepción de paquetes BTS
void funcion_de_llamada_reproducir_pausar(GtkWidget *widget, gpointer data);    //Función de llamada para reproducir o pausar
void funcion_de_llamada_detener();                                              //Función de llamada para detener_producción
void funcion_de_llamada_volumen(GtkWidget *widget, gpointer data);              //Función de llamada para colocar volúmen
void funcion_reproducir();                                                      //Función para reproducir el archivo .TS
void funcion_unpluged_reproduccion();                                           //Función que ejecuta un hilo para detener el reproductor si el dispositvo es retirado
void funcion_de_llamada_maxima_pantalla();
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//------------------------------------VARIABLES GLOBALES GRAFICAS COMIENZO--------------------------------------
///Borde de los widgets
const int ANCHO_BORDES=6;
///Estructura sesión de libvlc
libvlc_instance_t       *instancia_vlc;
///Estructura reproductor
libvlc_media_player_t   *reproductor;

//Declaración de widgets inicio
GtkWidget *ventana,                                     //Declaración Widget de la ventana
          *reproductor_widget,                          //Declaración Widget del Reproductor
          *rejilla,                                     //Declaración Widget de la rejilla
          *caja_horizontal_botones_control_izquierda,   //Declaración Widget de la caja_horizontal_botones_control_izquierda
          *caja_horizontal_botones_control_derecha,     //Declaración Widget de la caja_horizontal_botones_control_derecha
          *caja_vertical_botones_canales_1,             //Declaración Widget de la caja_vertical_botones_canales_1
          *caja_vertical_botones_canales_2,             //Declaración Widget de la caja_vertical_botones_canales_2
          *boton_reproducir_pausar,
          *boton_detener,                               //Declaración Widget del boton detener
          *volumen,                                     //Declaración Widget del boton  volumen
          *maxima_pantalla;                             //Declaración Widget de maxima_pantalla

GtkAdjustment *ajuste;                                  //Declaración de la variable ajuste

//Variables para botones de canales comienzo
//Frecuencia de 521000 Hz Comienzo
GtkWidget *boton_canal_521000_57856; //Variable para el botón del canal VTV
GtkWidget *boton_canal_521000_57857; //Variable para el botón del canal 123TV
GtkWidget *boton_canal_521000_57858; //Variable para el botón del canal Colombela
GtkWidget *boton_canal_521000_57859; //Variable para el botón del canal Venevision
GtkWidget *boton_canal_521000_57860; //Variable para el botón del canal AlbaTV
GtkWidget *boton_canal_521000_57880; //Variable para el botón del canal VTVMovil
//Frecuencia de 521000 Hz Fin
//Frecuencia de 533000 Hz Comienzo
GtkWidget *boton_canal_533000_57920; //Variable para el botón del canal TVES
GtkWidget *boton_canal_533000_57921; //Variable para el botón del canal ANTV
GtkWidget *boton_canal_533000_57922; //Variable para el botón del canal SIBCHID
GtkWidget *boton_canal_533000_57944; //Variable para el botón del canal TVESmovil
//Frecuencia de 533000 Hz Fin
//Frecuencia de 539000 Hz Comienzo
GtkWidget *boton_canal_539000_57952; //Variable para el botón del canal FANBTV
GtkWidget *boton_canal_539000_57953; //Variable para el botón del canal CCTV
GtkWidget *boton_canal_539000_57954; //Variable para el botón del canal AvilaTV
GtkWidget *boton_canal_539000_57955; //Variable para el botón del canal PDVSATV
GtkWidget *boton_canal_539000_57956; //Variable para el botón del canal RussiaToday
GtkWidget *boton_canal_539000_57976; //Variable para el botón del canal AVilaTVmovil
//Frecuencia de 539000 Hz Fin
//Frecuencia de 527000 Hz Comienzo
GtkWidget *boton_canal_527000_57888; //Variable para el botón del canal ViveTV
GtkWidget *boton_canal_527000_57889; //Variable para el botón del canal Telesur
GtkWidget *boton_canal_527000_57890; //Variable para el botón del canal Meridiano
GtkWidget *boton_canal_527000_57891; //Variable para el botón del canal Televen
GtkWidget *boton_canal_527000_57892; //Variable para el botón del canal Conciencia
GtkWidget *boton_canal_527000_57912; //Variable para el botón del canal Telesurmovil
//Frecuencia de 527000 Hz Fin
//Variables Buttons of channels End

//------------------------------------VARIABLES GLOBALES GRAFICAS FIN-------------------------------------------

int main(int argc, char *argv[])
{
    std::thread hilo_hotplugin (funcion_hotplug);                           //Hilo que ejecuta la función hotplug para dectectar el dispostivo, reclamarlo y abrirlo
    hilo_hotplugin.detach();                                                //Crea un demonio del hilo hilo_hotplugin
    std::thread hilo_unpluged_reproduccion (funcion_unpluged_reproduccion); //Hilo que ejecuta la función unpluged_reproduccion detner producción en caso de retirar el dispositivo
    hilo_unpluged_reproduccion.detach();                                    //Crea un demonio del hilo hilo_unpluged_reproduccion



    //! Función que se encargar de comenzar el ciclo infito de GTK3.
    gtk_init(&argc, &argv);

        //------------------------------------CREACION DE LOS WIDGETS COMIENZO----------------------------------
        rejilla= gtk_grid_new();                                                                            //Creación de la rejilla para alojar los widgets

        ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);                                                      //Creación una nueva ventana
        gtk_window_set_title(GTK_WINDOW(ventana), "TDA Linux Cendit");                                      //Colocar tìtulo de la ventana
        gtk_window_set_resizable (GTK_WINDOW(ventana),false);                                               //No redimensionar ventana

        caja_vertical_botones_canales_1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);                         //Creación caja vertical para alojar botones de los canales
        caja_vertical_botones_canales_2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);                         //Creación caja vertical para alojar botones de los canales
        caja_horizontal_botones_control_izquierda = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, ANCHO_BORDES);  //Creación caja horizontal para alojar botones de control
        caja_horizontal_botones_control_derecha = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, ANCHO_BORDES);    //Creación caja horizontal para alojar botones de control

        reproductor_widget = gtk_drawing_area_new();                                                        //Dibujar el area del reproductor
        gtk_widget_set_size_request (reproductor_widget, 800, 100);                                         //Fijar las dimensiones del reproductor
        instancia_vlc = libvlc_new(0, NULL);                                                                //Creación de una cesión de libvlc
        reproductor = libvlc_media_player_new(instancia_vlc);                                               //Creación del reproducctor

        boton_reproducir_pausar = gtk_button_new_with_label("Reproducir");                                  //Creación  del boton reproducir y pausar
        boton_detener = gtk_button_new_with_label("Detener");                                               //Creación  del boton detener
        volumen = gtk_volume_button_new();                                                                  //Creación  del boton volumen
        ajuste = gtk_adjustment_new(50,0,100,1,0,0);                                                        //Creacion del ajuste para la escala del volumen: valor incial, inferior, superior, incrmento, incremento pagina, tamaño pagina
        gtk_scale_button_set_adjustment(GTK_SCALE_BUTTON(volumen),ajuste);                                  //Añadir la escala al boton de volumen
        maxima_pantalla = gtk_button_new_with_label("M");                                                   //Creación del boton para maximizar la pantalla

        //Creación de los botones de los Canales Comienzo
        //Frecuencia de 521000 Hz Comienzo
        boton_canal_521000_57856 = gtk_button_new_with_label("VTV");
        boton_canal_521000_57857 = gtk_button_new_with_label("123 TV");
        boton_canal_521000_57858 = gtk_button_new_with_label("Colombella");
        boton_canal_521000_57859 = gtk_button_new_with_label("Venevision");
        boton_canal_521000_57860 = gtk_button_new_with_label("AlbaTV");
        boton_canal_521000_57880 = gtk_button_new_with_label("VTVmovil");
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        boton_canal_533000_57920 = gtk_button_new_with_label("TVES");
        boton_canal_533000_57921 = gtk_button_new_with_label("ANTV");
        boton_canal_533000_57922 = gtk_button_new_with_label("SIBCHID");
        boton_canal_533000_57944 = gtk_button_new_with_label("TVESmovil");
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        boton_canal_539000_57952 = gtk_button_new_with_label("FANBTV");
        boton_canal_539000_57953 = gtk_button_new_with_label("CCTV");
        boton_canal_539000_57954 = gtk_button_new_with_label("AvilaTV");
        boton_canal_539000_57955 = gtk_button_new_with_label("PDVSATV");
        boton_canal_539000_57956 = gtk_button_new_with_label("RussiaToday");
        boton_canal_539000_57976 = gtk_button_new_with_label("AVilaTVmovil");
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        boton_canal_527000_57888 = gtk_button_new_with_label("ViveTV");
        boton_canal_527000_57889 = gtk_button_new_with_label("Telesur");
        boton_canal_527000_57890 = gtk_button_new_with_label("Meridiano");
        boton_canal_527000_57891 = gtk_button_new_with_label("Televen");
        boton_canal_527000_57892 = gtk_button_new_with_label("Conciencia");
        boton_canal_527000_57912 = gtk_button_new_with_label("TelesurMovil");
        //Frecuencia de 527000 Hz Fin
        //Creación de los botones de los Canales Fin

        //------------------------------------CREACION DE LOS WIDGETS FIN---------------------------------------

        //------------------------------------EMPAQUETADO DE LOS WIDGETS COMIENZO-------------------------------
        gtk_container_add (GTK_CONTAINER (ventana), rejilla);                   //Insertar la ventana en el contenedor rejilla

        //Insertar en la caja_vertical_botones_canales_1 y en caja_vertical_botones_canales_2 los botones de los canales comienzo
        //Frecuencia de 521000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_521000_57856, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_521000_57857, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_521000_57858, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_521000_57859, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_521000_57860, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_521000_57880, FALSE, FALSE, 0);
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_533000_57920, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_533000_57921, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_533000_57922, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_533000_57944, FALSE, FALSE, 0);
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_539000_57952, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_539000_57953, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_539000_57954, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_539000_57955, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_539000_57956, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_1), boton_canal_539000_57976, FALSE, FALSE, 0);
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_2), boton_canal_527000_57888, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_2), boton_canal_527000_57889, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_2), boton_canal_527000_57890, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_2), boton_canal_527000_57891, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_2), boton_canal_527000_57892, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales_2), boton_canal_527000_57912, FALSE, FALSE, 0);
        //Frecuencia de 527000 Hz Fin
        //Insertar en la caja_vertical_botones_canales_1 y en caja_vertical_botones_canales_2 los botones de los canales fin

        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_control_izquierda), boton_reproducir_pausar, FALSE, FALSE, 0);   //Insertar en la caja_horizontal_botones_control_izquierda el boton_reproducir_pausar
        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_control_izquierda), boton_detener, FALSE, FALSE, 0);             //Insertar en la caja_horizontal_botones_control_izquierda el boton_detener
        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_control_derecha), maxima_pantalla, FALSE, FALSE, 0);             //Insertar en la caja_horizontal_botones_control_derecha el boton maxima_pantalla
        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_control_derecha), volumen, FALSE, FALSE, 0);                     //Insertar en la caja_horizontal_botones_control_derecha el boton volumen

        //Inserta las cajas en la rejilla comienzo
        gtk_grid_attach (GTK_GRID (rejilla),                //Insertar en la rejilla
                 reproductor_widget,                        //Inserta el reproductor_widget en la rejilla
                 0,                                         //El widget comienza en el cuadrante cero del numero de columna
                 0,                                         //El widget comienza en el cuadrante cero del numero de fila
                 20,                                        //Ocupa veinte filas
                 20);                                       //Ocupa veinte columnas

        gtk_grid_attach (GTK_GRID (rejilla),                //Insertar en la rejilla
                 caja_vertical_botones_canales_1,           //Insertar la caja_vertical_botones_canales_1
                 21,                                        //El widget comienza en el cuadrante veintiuno del numero de columna
                 0,                                         //El widget comienza en el cuadrante cero del numero de fila
                 1,                                         //Ocupa una fila
                 1);                                        //Ocupa una columna

        gtk_grid_attach (GTK_GRID (rejilla),                //Insertar en la rejilla
                 caja_vertical_botones_canales_2,           //Insertar la caja_vertical_botones_canales_2
                 22,                                        //El widget comienza en el cuadrante veintidos del numero de columna
                 0,                                         //El widget comienza en el cuadrante cero del numero de fila
                 1,                                         //Ocupa una fila
                 1);                                        //Ocupa una columna

        gtk_grid_attach (GTK_GRID (rejilla),                //Insertar en la rejilla
                 caja_horizontal_botones_control_izquierda, //Insertar la caja_horizontal_botones_control_izquierda
                 0,                                         //El widget comienza en el cuadrante cero del numero de columna
                 20,                                        //El widget comienza en el cuadrante veinte del numero de fila
                 1,                                         //Ocupa una fila
                 1);                                        //Ocupa una columna

        gtk_grid_attach (GTK_GRID (rejilla),                //Insertar en la rejilla
                 caja_horizontal_botones_control_derecha,   //Insertar la caja_horizontal_botones_control_derecha
                 19,                                        //El widget comienza en el cuadrante diecinueve del numero de columna
                 20,                                        //El widget comienza en el cuadrante veinte del numero de fila
                 1,                                         //Ocupa dos filas
                 1);                                        //Ocupa una columna
        //Inserta las cajas en la rejilla fin
        //------------------------------------EMPAQUETADO DE LOS WIDGETS FIN------------------------------------

        //------------------------------------SEÑALES Y FUNCIONES DE LLAMADA COMIENZO---------------------------
        g_signal_connect(ventana, "destroy", G_CALLBACK(funcion_de_llamada_destruir), NULL);                                //Función de llamada para destruir todos los widget cuando la ventana se cierre

        //Señales y funciones de llamada para los botones de los canales Comienzo
        //Frecuencia de 521000 Hz Comienzo
        g_signal_connect(boton_canal_521000_57856, "clicked", G_CALLBACK(funcion_canal_521000_57856), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57857, "clicked", G_CALLBACK(funcion_canal_521000_57857), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57858, "clicked", G_CALLBACK(funcion_canal_521000_57858), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57859, "clicked", G_CALLBACK(funcion_canal_521000_57859), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57860, "clicked", G_CALLBACK(funcion_canal_521000_57860), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57880, "clicked", G_CALLBACK(funcion_canal_521000_57880), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        g_signal_connect(boton_canal_533000_57920, "clicked", G_CALLBACK(funcion_canal_533000_57920), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_533000_57921, "clicked", G_CALLBACK(funcion_canal_533000_57921), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_533000_57922, "clicked", G_CALLBACK(funcion_canal_533000_57922), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_533000_57944, "clicked", G_CALLBACK(funcion_canal_533000_57944), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        g_signal_connect(boton_canal_539000_57952, "clicked", G_CALLBACK(funcion_canal_539000_57952), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57953, "clicked", G_CALLBACK(funcion_canal_539000_57953), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57954, "clicked", G_CALLBACK(funcion_canal_539000_57954), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57955, "clicked", G_CALLBACK(funcion_canal_539000_57955), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57956, "clicked", G_CALLBACK(funcion_canal_539000_57956), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57976, "clicked", G_CALLBACK(funcion_canal_539000_57976), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        g_signal_connect(boton_canal_527000_57888, "clicked", G_CALLBACK(funcion_canal_527000_57888), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57889, "clicked", G_CALLBACK(funcion_canal_527000_57889), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57890, "clicked", G_CALLBACK(funcion_canal_527000_57890), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57891, "clicked", G_CALLBACK(funcion_canal_527000_57891), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57892, "clicked", G_CALLBACK(funcion_canal_527000_57892), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57912, "clicked", G_CALLBACK(funcion_canal_527000_57912), NULL);                //Función de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 527000 Hz Fin
        //Señales y funciones de llamada para los botones de los canales Fin

        //Señales y funciones de llamada para los botones de los canales Comienzo
        //Frecuencia de 521000 Hz Comienzo
        g_signal_connect(boton_canal_521000_57856, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57857, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57858, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57859, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57860, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57880, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        g_signal_connect(boton_canal_533000_57920, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_533000_57921, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_533000_57922, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_533000_57944, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        g_signal_connect(boton_canal_539000_57952, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57953, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57954, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57955, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57956, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57976, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        g_signal_connect(boton_canal_527000_57888, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57889, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57890, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57891, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57892, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57912, "clicked", G_CALLBACK(funcion_de_llamada_hilo_recepcion), NULL);         //Función de llamada funcion_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 527000 Hz Fin
        //Señales y funciones de llamada para los botones de los canales Fin

        g_signal_connect(boton_reproducir_pausar, "clicked", G_CALLBACK(funcion_de_llamada_reproducir_pausar), NULL);       //Función de llamada funcion_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione
        g_signal_connect(boton_detener, "clicked", G_CALLBACK(funcion_de_llamada_detener), NULL);                           //Función de llamada funcion_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione
        g_signal_connect(volumen, "value_changed",G_CALLBACK(funcion_de_llamada_volumen), NULL);                            //Función de llamada funcion_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione
        g_signal_connect(maxima_pantalla, "clicked", G_CALLBACK(funcion_de_llamada_maxima_pantalla), NULL);
        g_signal_connect(G_OBJECT(reproductor_widget), "realize", G_CALLBACK(funcion_de_llamada_reproductor_widget), NULL); //Función de llamada funcion_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione

        //------------------------------------SEÑALES Y FUNCIONES DE LLAMADA FIN---------------------------.....

        gtk_widget_show_all(ventana);               //Mostar todo los Widgets

    gtk_main();                                     //Fin del ciclo infinito de GTK3

    //Cuando el programa finaliza se realizan las siguientes operaciones

    libvlc_media_player_release(reproductor);       //Liberar el Reproductor de la sesión libvlc
    libvlc_release(instancia_vlc);                  //Cerrar la sesión de libvlc
    funcion_reinicializar_archivo();                //Reincia el archivo
    funcion_desconexion_usb();                      //Desconectar el dispositivo USB
    std::terminate();                               //Terminar todos los hilos y los demonios

    return 0;                                       //Retorna 0
}
//! Función de llamada para destruir todos los widget cuando la ventana se cierre, la señal emitida es de tipo "destroy" cuando la aplicacion se cierra.
/*!
  \param *widget puntero al widget ventana.
  \param data datos pasados a la función de llamada.
*/
void funcion_de_llamada_destruir(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();                                //Finalizar la ventana de aplicación
}

//! Función de llamada para insertar el widget de reproductot en la ventana, la señal emitida es de tipo "realize".
/*!
  \param *widget puntero al widget reproductor.
  \param data datos pasados a la función de llamada.
*/
void funcion_de_llamada_reproductor_widget(GtkWidget *widget, gpointer data)
{
   libvlc_media_player_set_xwindow(reproductor, GDK_WINDOW_XID(gtk_widget_get_window(widget))); //Insertar el reproductor en la ventana
}

//! Función de llamada para comenzar la recepción de paquetes BTS y reproducirlos, la señal emitida es de tipo "clicked" cuando el boton de un canal es presionado.
void funcion_de_llamada_hilo_recepcion()
{
    funcion_reinicializar_archivo();                                                        //Reinicia el archivo
    std::thread hilo_recepcion_BTS (funcion_ciclo_recepcion_BTS);                           //Hilo que ejecuta la función de recibir los paquetes BTS y guardarlos en un archivo .TS
    hilo_recepcion_BTS.detach();                                                            //Crea un demonio del hilo hilo_recepcion_BTS

	libvlc_audio_set_volume(reproductor,50);                                                //Colocar el volumen en 50%

    if ((funcion_leer_estado_hotplug()<1)||(funcion_leer_estado_hotplug()==2))              //Si el estado de conexión es menor a 1 o igual a 2
    {
        std::cout<<"No se puede reproducir: El dispositivo esta desconectado."<<std::endl;  //Mostar la siguiente advertencia
    }else  funcion_reproducir();                                                            //Sino llamar a la función encargada de hacer la reproducción
}

//! Función reproducir
void funcion_reproducir()
{
    SemcoSleep(1500);                                                               //Esperar 1,5 segundo para llenar un poco el archivo .TS

    //Obteniendo el directorio donde se encuentra el archivo a reproducir Comienzo
    char directorio_actual_de_trabajo[200000];                                      //Tamaño del buffer donde guardar el directorio
    getcwd(directorio_actual_de_trabajo, sizeof(directorio_actual_de_trabajo));     //Rellena el buffer con el directorio actual de trabajo
    std::string direccion_archivo_vlc=directorio_actual_de_trabajo;                 //Se guarda en un string
    direccion_archivo_vlc="file://"+direccion_archivo_vlc+"/data.ts";               //Se modifica el string para hacerlo compatible con la direccion tomada por libvlc
    //Obteniendo el directorio donde se encuentra el archivo a reproducir Fin

    libvlc_media_t *media;                                                          //Se crea la variable media
    media = libvlc_media_new_location(instancia_vlc, &direccion_archivo_vlc[0u]);   //fill media
    libvlc_media_add_option(media,funcion_leer_id());                               //Se coloca el Id del canal deseado a reproducir
    libvlc_media_player_set_media(reproductor, media);                              //Se coloca el medio media en el reporductor
    libvlc_media_player_play(reproductor);                                          //Reproducir
    gtk_button_set_label(GTK_BUTTON(boton_reproducir_pausar), "Pausar");            //Se coloca la etiqueta del boton_reproducir_pausar en Pausar
}

//! Función de llamada para la señal emitida de tipo "clicked" por el boton pausar y reproducir.
/*!
  \param *widget puntero al widget del botón de play y pausa.
  \param data datos pasados a la función de llamada.
*/
void funcion_de_llamada_reproducir_pausar(GtkWidget *widget, gpointer data)
{
    if(!funcion_leer_estado_frecuencia())                       //Si ninguna frecuencia fue establecida antes de que el boton_reproducir_pausar fuera presionado
    {
        funcion_canal_521000_57856();       				    //Estable el canal VTV como inical
    }

    if(libvlc_media_player_is_playing(reproductor) == 1)        //Si el reproductor esta reproduciendo
    {
        libvlc_media_player_pause(reproductor);                 //Pausar el reproductor
        gtk_button_set_label(GTK_BUTTON(widget), "Reproducir"); //Se coloca la etiqueta del boton_reproducir_pausar en Reproducir
    }
    else funcion_de_llamada_hilo_recepcion();                   //Comenzar el hilo de la recepción de paquetes BTS y reproducirlos
}

//! Función de llamada para la señal emitida de tipo "clicked" por el boton detener.
void funcion_de_llamada_detener()
{
    libvlc_media_player_stop(reproductor);                                          //Detener el reproductor

    if(libvlc_media_player_is_playing(reproductor) == 1)                            //Si el reproductor está reproduciendo
    {
        gtk_button_set_label(GTK_BUTTON(boton_reproducir_pausar), "Pausar");        //Se coloca la etiqueta del boton_reproducir_pausar en Pausar
    }else gtk_button_set_label(GTK_BUTTON(boton_reproducir_pausar), "Reproducir");  //Se coloca la etiqueta del boton_reproducir_pausar en Reproducir

    SemcoSleep(300); //Retardo

}

//! Función de llamada para la señal emitida de tipo "value-changed" por el boton volumen.
/*!
  \param *widget puntero al widget del botón volumen.
  \param data datos pasados a la función de llamada.
*/
void funcion_de_llamada_volumen(GtkWidget *widget, gpointer data)
{
    libvlc_audio_set_volume(reproductor,int(gtk_scale_button_get_value(GTK_SCALE_BUTTON(widget)))); //Obtener la escala del boton volumen y colocarlar como volumen del reproductor
}

//! Función de llamada para maxima pantalla.
void funcion_de_llamada_maxima_pantalla()
{


}


//! Función para detner el rproductor si el dispositivo es desconectado.
void funcion_unpluged_reproduccion()
{
    while(1) //Ciclo infnito que eejecuta el hilo
    {
        if (funcion_leer_estado_hotplug()>1) funcion_de_llamada_detener(); //Si el estatdo de hotplug mayor a uno detener el reproductor
    }
}
