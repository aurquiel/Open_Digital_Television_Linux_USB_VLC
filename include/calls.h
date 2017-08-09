 /* TDA_LINUX is a USB control rutine and multimedia rutine to the Dongle
    USB of Digital Televesion developed by the CENDIT Fundation.
    Copyright (C) 2017  Edgar Gomez

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef CALLS_H_INCLUDED
#define CALLS_H_INCLUDED

#include "usb.h"         //Incluir la cabezera usb.h
#include "initialize.h"  //Incluir la cabezera initialize.h
#include "thread.h"      //Incluir la cabezera thread.h
#include "samsung.h"     //Incluir la cabezera samsung.h
#include <cstring>       //Incluir la libreria cstring

//Incluir imagenes de los iconos
#include "../imagenes/iconos/icon_imagen.h"
#include "../imagenes/iconos/reproducir_imagen.h"
#include "../imagenes/iconos/pausar_imagen.h"
#include "../imagenes/iconos/detener_imagen.h"
#include "../imagenes/iconos/pantalla_completa_imagen.h"
#include "../imagenes/iconos/usb_conectado_imagen.h"
#include "../imagenes/iconos/usb_desconectado_imagen.h"
#include "../imagenes/iconos/senial_00_imagen.h"
#include "../imagenes/iconos/senial_01_imagen.h"
#include "../imagenes/iconos/senial_02_imagen.h"
#include "../imagenes/iconos/senial_03_imagen.h"
#include "../imagenes/iconos/senial_04_imagen.h"
#include "../imagenes/iconos/senial_05_imagen.h"
#include "../imagenes/iconos/senial_06_imagen.h"
#include "../imagenes/iconos/senial_07_imagen.h"
#include "../imagenes/iconos/senial_08_imagen.h"
#include "../imagenes/iconos/senial_09_imagen.h"
#include "../imagenes/iconos/senial_10_imagen.h"

//Incluir imagenes de los canales
#include "../imagenes/canales/tv123_imagen.h"
#include "../imagenes/canales/alba_imagen.h"
#include "../imagenes/canales/antv_imagen.h"
#include "../imagenes/canales/avilatv_imagen.h"
#include "../imagenes/canales/cctv_imagen.h"
#include "../imagenes/canales/colombeia_imagen.h"
#include "../imagenes/canales/conciencia_imagen.h"
#include "../imagenes/canales/fanbtv_imagen.h"
#include "../imagenes/canales/meridiano_imagen.h"
#include "../imagenes/canales/pdvsatv_imagen.h"
#include "../imagenes/canales/russiatoday_imagen.h"
#include "../imagenes/canales/sibci_imagen.h"
#include "../imagenes/canales/telesur_imagen.h"
#include "../imagenes/canales/televen_imagen.h"
#include "../imagenes/canales/tves_imagen.h"
#include "../imagenes/canales/venevision_imagen.h"
#include "../imagenes/canales/vive_imagen.h"
#include "../imagenes/canales/vtv_imagen.h"

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------
void procedimiento_de_llamada_destruir();                                               //Procedimiento de llamada para cuando la ventana se cierre
void procedimiento_de_llamada_reproductor_widget(GtkWidget *widget);                    //Procedimiento de llamada para insertar el reproductor
int procedimiento_de_llamada_hilo_recepcion(void);                                     //Procedimiento de llamada para comenzar el hilo de recepción de paquetes BTS
void procedimiento_de_llamada_reproducir_pausar(GtkWidget *widget, gpointer data);      //Procedimiento de llamada para reproducir o pausar
void procedimiento_de_llamada_detener(void);                                            //Procedimiento de llamada para detener_producción
void procedimiento_de_llamada_volumen(GtkWidget *widget, gpointer data);                //Procedimiento de llamada para colocar volúmen
void procedimiento_hilo_detener(void);                                                  //Procedimiento de hilo detener
void procedimiento_de_llamada_maxima_pantalla(void);                                    //Procedimiento de llamada para ir a pantalla completa
void procedimiento_de_llamada_tecla_presionada(GtkWidget *widget, GdkEventKey *event, gpointer user_data); //Procedimiento de llamada para las hotkeys
void procedimiento_media_callbacks(void);                                               //Procedimiento procedimiento_media_callbacks.
int abrir_media(void *opaque, void **datap, long unsigned int *sizep);                  //Función de llamada de la función libvlc_media_new_callbacks para apuntar a la posición de los datos y guardar el tamaño del buffer a leer.
long int leer_media(void *opaque, unsigned char *buf,long unsigned int len);            //Función de llamada de la función libvlc_media_new_callbacks para leer los datos
int posicionar_media(void *opaque, uint64_t offset);                                    //Función de llamada de la función libvlc_media_new_callbacks para colocar la nueva posicion en el buffer de datos
int procedimiento_imagenes_caja_horizontal_indicadores(void*);                          //Procedimiento para manegar las imagenes de estado_hotplug: conectado o desconectado, manejar las imagenes de calidad de señal.
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//------------------------------------VARIABLES GLOBALES GRAFICAS COMIENZO--------------------------------------
///Estructura sesión libvlc.
libvlc_instance_t       *instancia_vlc;
///Estructura reproductor libvlc.
libvlc_media_player_t   *reproductor;
///Media del reproductor.
libvlc_media_t *media;
///Variable para controlar el intercambio de estados entre pantalla completa o no pantalla completa.
/*!
Variable controlador_pantalla_completa:
  - Si controlador_pantalla_completa Igual FALSO, en modo pantalla completa.
  - Si controlador_pantalla_completa Igual VERDADERO, en modo no pantalla completa.
*/
bool controlador_pantalla_completa=false;

GtkWidget
            ///GTK_WIDGET para la ventana.
            *ventana,
            ///GTK_WIDGET rejilla para la distribción de widgets del programa.
            *rejilla,
            ///GTK_WIDGET para el botón reproductor de video.
            *reproductor_widget,
            ///GTK_WIDGET para el botón reproducir y pausar.
            *boton_reproducir_pausar,
            ///GTK_WIDGET para el botón detener.
            *boton_detener,
            ///GTK_WIDGET para el botón volumen.
            *volumen,
            ///GTK_WIDGET para el botón pantalla completa.
            *pantalla_completa,
            ///GTK_WIDGET para la caja de botones verticales.
            *caja_vertical_botones_programacion,
            ///GTK_WIDGET scroll ventana
            *ventana_desplazamiento,
            ///GTK_WIDGET para la caja horizontal que contiene botones de control.
            *caja_horizontal_botones_de_control,
            ///GTK_WIDGET para la caja horizontal que contiene botones de control.
            *caja_horizontal_indicadores,
            ///GTK_WIDGET para la caja vertical que contiene caja_horizontal_reproductor_botones_canales y caja_horizontal_botones_de_control.
            *caja_vertical_mayor,
            ///GTK_WIDGET para etiqueta de estado_usb.
            *etiqueta_estado_usb,
            ///GTK_WIDGET para etiqueta de senial.
            *etiqueta_senial,
            ///GTK_WIDGET para frame estado_usb.
            *frame_estado_usb,
            ///GTK_WIDGET para frame senial.
            *frame_senial,
            ///GTK_WIDGET para boton estado_usb.
            *boton_estado_usb,
            ///GTK_WIDGET para boton senial.
            *boton_senial;

///GTK_ADJUSTMENT para la escala del volumen.
GtkAdjustment *ajuste,
///GTK_ADJUSTMENT para la escala de desplazmiento de la barra del widget ventana_desplazamiento.
              *desplazamiento;

//Variables para botones de programacion comienzo
//Frecuencia de 521000 Hz Comienzo
///GTK_WIDGET para el botón del programa a la fruencia de 521000, ID=57856, VTV.
GtkWidget *boton_programa_521000_57856;
///GTK_WIDGET para el botón del programa a la fruencia de 521000, ID=57857, 123VTV.
GtkWidget *boton_programa_521000_57857;
///GTK_WIDGET para el botón del programa a la fruencia de 521000, ID=57858, Colombela.
GtkWidget *boton_programa_521000_57858;
///GTK_WIDGET para el botón del programa a la fruencia de 521000, ID=57859, Venevision.
GtkWidget *boton_programa_521000_57859;
///GTK_WIDGET para el botón del programa a la fruencia de 521000, ID=57860, AlbaTV.
GtkWidget *boton_programa_521000_57860;
///GTK_WIDGET para el botón del programa a la fruencia de 521000, ID=57880, VTVMovil.
GtkWidget *boton_programa_521000_57880;
//Frecuencia de 521000 Hz Fin
//Frecuencia de 533000 Hz Comienzo
///GTK_WIDGET para el botón del programa a la fruencia de 533000, ID=57920, TVES.
GtkWidget *boton_programa_533000_57920;
///GTK_WIDGET para el botón del programa a la fruencia de 533000, ID=57921, ANTV.
GtkWidget *boton_programa_533000_57921;
///GTK_WIDGET para el botón del programa a la fruencia de 533000, ID=57922, SIBCHID.
GtkWidget *boton_programa_533000_57922;
///GTK_WIDGET para el botón del programa a la fruencia de 533000, ID=57944, TVESmovil.
GtkWidget *boton_programa_533000_57944;
//Frecuencia de 533000 Hz Fin
//Frecuencia de 539000 Hz Comienzo
///GTK_WIDGET para el botón del programa a la fruencia de 539000, ID=57952, FANBTV.
GtkWidget *boton_programa_539000_57952;
///GTK_WIDGET para el botón del programa a la fruencia de 539000, ID=57953, CCTV.
GtkWidget *boton_programa_539000_57953;
///GTK_WIDGET para el botón del programa a la fruencia de 539000, ID=57954, AvilaTV.
GtkWidget *boton_programa_539000_57954;
///GTK_WIDGET para el botón del programa a la fruencia de 539000, ID=57955, PDVSATV.
GtkWidget *boton_programa_539000_57955;
///GTK_WIDGET para el botón del programa a la fruencia de 539000, ID=57956, RussiaToday.
GtkWidget *boton_programa_539000_57956;
///GTK_WIDGET para el botón del programa a la fruencia de 539000, ID=57976, AVilaTVmovil.
GtkWidget *boton_programa_539000_57976;
//Frecuencia de 539000 Hz Fin
//Frecuencia de 527000 Hz Comienzo
///GTK_WIDGET para el botón del programa a la fruencia de 527000, ID=57888, ViveTV.
GtkWidget *boton_programa_527000_57888;
///GTK_WIDGET para el botón del cprograma a la fruencia de 527000, ID=57889, Telesur.
GtkWidget *boton_programa_527000_57889;
///GTK_WIDGET para el botón del programa a la fruencia de 527000, ID=57890, Meridiano.
GtkWidget *boton_programa_527000_57890;
///GTK_WIDGET para el botón del programa a la fruencia de 527000, ID=57891, Televen.
GtkWidget *boton_programa_527000_57891;
///GTK_WIDGET para el botón del programa a la fruencia de 527000, ID=57892, Conciencia.
GtkWidget *boton_programa_527000_57892;
///GTK_WIDGET para el botón del programa a la fruencia de 527000, ID=57912, Telesurmovil.
GtkWidget *boton_programa_527000_57912;
//Frecuencia de 527000 Hz Fin
//Variables para botones de programacio fin

///GTK_IMAGE variable que maneja los iconos e imagenes de todo el programa
GtkImage  *imagen;

///GTK_IMAGE variable que maneja la informacion de los iconos e imagenes de todo el programa
GdkPixbuf *icon;


//! Procedimiento de llamada para embeber el widget reproductor.
/*!
  \param *widget puntero al widget reproductor.
*/
void procedimiento_de_llamada_reproductor_widget(GtkWidget *widget)
{
   libvlc_media_player_set_xwindow(reproductor, GDK_WINDOW_XID(gtk_widget_get_window(widget))); //Insertar el reproductor en la ventana
}

//! Procedimiento de llamada para comenzar la recepción de paquetes BTS y reproducirlos.
/*!
  @return 0 al finalizar.
  @return -1 en caso de que el dispositivo no este conectado.

  Advertencias que muestra:
  - Si dipositivo está desconectado: "No se puede reproducir: El dispositivo esta desconectado.".
*/
int procedimiento_de_llamada_hilo_recepcion(void)
{
    if ((funcion_leer_estado_hotplug()<1)||(funcion_leer_estado_hotplug()==2))                  //Si el estado de conexión es menor a 1 o igual a 2
    {
        std::cout<<"No se puede reproducir: El dispositivo esta desconectado."<<std::endl;      //Mostar la siguiente advertencia
        return -1;
    }

    procedimiento_de_llamada_detener();                                                         //Llamada a la función procedimeinto_de_llamada_detener

    std::thread hilo_recepcion_BTS (procedimiento_ciclo_recepcion_BTS);                         //Hilo que ejecuta la función de recibir los paquetes BTS y guardarlos en un archivo .TS
    hilo_recepcion_BTS.detach();                                                                //Crea un demonio del hilo hilo_recepcion_BTS

    procedimiento_retardo_milisegundos(2000);                                                   //Esperar 2000ms
    procedimiento_de_llamada_volumen(volumen, NULL);                                            //Se coloca el volumen lo que indica el boton de volumen, evita que se pierda el volumen cundo se cambie de canal
    procedimiento_media_callbacks();                                                            //Funciones de llamada para obtner la media del video

    icon = gdk_pixbuf_new_from_inline (-1, pausar_en_linea, FALSE, NULL);                       //Asignar los datos de la imagen a icon
    imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                        //Asignar a imagen icon
    gtk_button_set_image (GTK_BUTTON(boton_reproducir_pausar), GTK_WIDGET(imagen));             //Añadir la imagen al boton boton_reproducir_pausar
    gtk_button_set_label (GTK_BUTTON(boton_reproducir_pausar), "Pausar");                       //Cambiar etiqueta del reproductor

    return 0;
}

//! Procedimiento de llamada para pausar y reproducir el video.
/*!
  \param *widget puntero al widget del botón reproducir y pausar.
  \param data datos pasados a la función de llamada.
*/
void procedimiento_de_llamada_reproducir_pausar(GtkWidget *widget, gpointer data)
{
    if(!funcion_leer_estado_frecuencia())                                                   //Si ninguna frecuencia fue establecida antes de que el boton_reproducir_pausar fuera presionado
    {
        procedimiento_programa_521000_57856();       				                        //Estable el program VTV como inical
    }

    if(libvlc_media_player_is_playing(reproductor) == 1)                                    //Si el reproductor esta reproduciendo
    {
        libvlc_media_player_pause(reproductor);                                             //Pausar el reproductor

        icon = gdk_pixbuf_new_from_inline (-1, reproducir_en_linea, FALSE, NULL);               //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(widget), GTK_WIDGET(imagen));                      //Añadir la imagen al boton boton_reproducir_pausar
        gtk_button_set_label (GTK_BUTTON(widget), "Reproducir");                       //Cambiar etiqueta del reproductor
    }
    else
        procedimiento_de_llamada_hilo_recepcion();                                          //Comenzar el hilo de la recepción de paquetes BTS y reproducirlos
}

//! Procedimiento de llamada para detener el video.
void procedimiento_de_llamada_detener(void)
{
    libvlc_media_player_stop(reproductor);                                                  //Detener el reproductor

    icon = gdk_pixbuf_new_from_inline (-1, reproducir_en_linea, FALSE, NULL);               //Asignar los datos de la imagen a icon
    imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
    gtk_button_set_image (GTK_BUTTON(boton_reproducir_pausar), GTK_WIDGET(imagen));         //Añadir la imagen al boton boton_reproducir_pausar
    gtk_button_set_label (GTK_BUTTON(boton_reproducir_pausar), "Reproducir");               //Cambiar etiqueta del reproductor
    procedimiento_retardo_milisegundos(300);                                                //Retardo

}

//! Procedimiento ejecutado por un hilo para detener el reproductor si el dispositivo USB es desconectado.
void procedimiento_hilo_detener(void)
{
    while(1)                                                //Ciclo infinito de ejecución del hilo
    {
        if(funcion_leer_estado_detener())                   //Si estado_deteer es verdadero
        {
            procedimiento_de_llamada_detener();             //Realizar el procedimeinto de llamada detener
            procedimiento_escribir_estado_detener(false);   //Escribir estado_detener falso
        }
        procedimiento_retardo_milisegundos(1);              //Retardo de 2 ms para evitar uso exsecivo del CPU
    }
}

//! Procedimiento de llamada para cambiar el volumen del reproductor cuando el valor del botón volumen cambie.
/*!
  \param *widget puntero al widget del botón volumen.
  \param data datos pasados a la función de llamada.
*/
void procedimiento_de_llamada_volumen(GtkWidget *widget, gpointer data)
{
    libvlc_audio_set_volume(reproductor, int(gtk_scale_button_get_value(GTK_SCALE_BUTTON(widget)))); //Obtener la escala del boton volumen y colocarlar como volumen del reproductor
}

//! Procedimiento de llamada para pantalla completa.
/*!
  Variable controlador_pantalla_completa:
  - Si controlador_pantalla_completa Igual FALSO, en modo pantalla completa.
  - Si controlador_pantalla_completa Igual VERDADERO, en modo no pantalla completa.
*/
void procedimiento_de_llamada_maxima_pantalla(void)
{
    if(!controlador_pantalla_completa)                          //Si controlador_pantalla_completa es falso, entonces
    {
        gtk_widget_hide(caja_horizontal_botones_de_control);    //Esconder la caja_horizontal_botones_de_control
        gtk_widget_hide(caja_horizontal_indicadores);           //Esconder la caja_horizontal_indicadores
        gtk_widget_hide(ventana_desplazamiento);                //Esconder ventana desplazamiento
        gtk_window_fullscreen(GTK_WINDOW(ventana));             //Colocar el widget ventana en pantalla completa
    }
    else                                                        //Sino
    {
        gtk_widget_show(caja_horizontal_botones_de_control);    //Mostrar la caja_horizontal_botones_de_control
        gtk_widget_show(caja_horizontal_indicadores);           //Mostrar la caja_horizontal_indicadores
        gtk_widget_show(ventana_desplazamiento);                //Mostrar ventana desplazamiento
        gtk_window_unfullscreen(GTK_WINDOW(ventana));           //Salir de de modo pantalla completa del widget ventana
        controlador_pantalla_completa = false;                  //Asignar controlador_pantalla_completa falso
    }

}

//! Procedimiento de llamada cuando una tecla del teclado es presionada.
/*!
  \param *widget puntero para controlar los widgets.
  \param *event puntero para los eventos cuando una tecla es presionada.
  \param user_data datos pasados a la función de llamada.

  Hotkeys:
  <ol>
    <li>KEY_PLUS: Incrementar volumen.</li>
    <li>KEY_MINUS: Decrementar volumen.</li>
    <li>KEY_A: Reproducir o pausar video.</li>
    <li>KEY_a: Reproducir o pausar video.</li>
    <li>KEY_D: Detener video.</li>
    <li>KEY_d: Detener video.</li>
    <li>KEY_S: Entrar en modo pantalla completa.</li>
    <li>KEY_s: Entrar en modo pantalla completa.</li>
    <li>KEY_ESCAPE: Salir del modo pantalla completa.</li>
  </ol>
*/
void procedimiento_de_llamada_tecla_presionada(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    switch (event->keyval)                                                                          //Conmutador recive el valor de la tecla presionada
    {
        case GDK_KEY_Escape:                                                                        //Caso de la tecla escape
            controlador_pantalla_completa = true;                                                   //hacer controlador_pantalla_completa verdadero
            procedimiento_de_llamada_maxima_pantalla();                                             //Llamar procedimiento_de_llamada_maxima_pantalla
        break;                                                                                      //Romper
        case GDK_KEY_plus:                                                                          //Caso de la tecla mas
        {
            int incrementar_volumen = int(gtk_scale_button_get_value(GTK_SCALE_BUTTON(volumen)));   //Hacer igual Incrementar_volumen al valor del boton de volumen
            incrementar_volumen = incrementar_volumen+5;                                            //Incrementar la variable incrementar_volumen en cinco
            if (incrementar_volumen > 100) incrementar_volumen = 100;                               //Si incrementar_volumen mayor a cien entonces hacer incrementar_volumen igual a cien
            gtk_scale_button_set_value(GTK_SCALE_BUTTON(volumen), incrementar_volumen);             //Asignar el valor de incrementar_volumen al boton de volumen
            libvlc_audio_set_volume(reproductor, incrementar_volumen);                              //Asignar el valor de incrementar_volumen al reproductor
        }
        break;                                                                                      //Romper
        case GDK_KEY_minus:                                                                         //Caso de la tecla menos
        {
            int decrementar_volumen = int(gtk_scale_button_get_value(GTK_SCALE_BUTTON(volumen)));   //Hacer igual Decrementar_volumen al valor del boton de volumen
            decrementar_volumen = decrementar_volumen-5;                                            //Decrementar la variable decrementar_volumen en cinco
            if (decrementar_volumen < 0) decrementar_volumen = 0;                                   //Si decrementar_volumen menor a cero entonces hacer decrementar_volumen igual a cer
            gtk_scale_button_set_value(GTK_SCALE_BUTTON(volumen), decrementar_volumen);             //Asignar el valor de decrementar_volumen al boton de volumen
            libvlc_audio_set_volume(reproductor, decrementar_volumen);                              //Asignar el valor de decrementar_volumen al reproductor
        }
        break;                                                                                      //Romper
        case GDK_KEY_a:                                                                             //Caso de la tecla a
        {
            if (event->state & GDK_CONTROL_MASK)                                                    //Si a y tecla control, entonces
            {
                procedimiento_de_llamada_reproducir_pausar(boton_reproducir_pausar, user_data);     //Llamar procedimiento_de_llamada_reproducir_pausar
            }
        }
        break;                                                                                      //Romper
        case GDK_KEY_d:                                                                             //Caso de la tecla d
        {
            if (event->state & GDK_CONTROL_MASK)                                                    //Si d y tecla control, entonces
            {
                procedimiento_de_llamada_detener();                                                 //Llamar procedimiento_de_llamada_detener
            }
        }
        break;                                                                                      //Romper
        case GDK_KEY_s:                                                                             //Caso de la tecla s
        {
            if (event->state & GDK_CONTROL_MASK)                                                    //Si s y tecla control, entonces
            {
                controlador_pantalla_completa = false;                                              //Asignar controlador_pantalla_completa falso
                procedimiento_de_llamada_maxima_pantalla();                                         //Llamar procedimiento_de_llamada_maxima_pantalla
            }
        }
        break;                                                                                      //Romper
        case GDK_KEY_A:                                                                             //Caso de la tecla A
        {
            if (event->state & GDK_CONTROL_MASK)                                                    //Si A y tecla control, entonces
            {
                procedimiento_de_llamada_reproducir_pausar(boton_reproducir_pausar, user_data);     //Llamar procedimiento_de_llamada_reproducir_pausar
            }
        }
        break;                                                                                      //Romper
        case GDK_KEY_D:                                                                             //Caso de la tecla D
        {
            if (event->state & GDK_CONTROL_MASK)                                                    //Si D y tecla control, entonces
            {
                procedimiento_de_llamada_detener();                                                 //Llamar procedimiento_de_llamada_detener
            }
        }
        break;
        case GDK_KEY_S:                                                                             //Caso de la tecla S
        {
            if (event->state & GDK_CONTROL_MASK)                                                    //Si S y tecla control, entonces
            {
                controlador_pantalla_completa = false;                                              //Asignar controlador_pantalla_completa falso
                procedimiento_de_llamada_maxima_pantalla();                                         //Llamar procedimiento_de_llamada_maxima_pantalla
            }
        }
        break;                                                                                      //Romper
        //MI caso



        //****ESTA TECLA ES PARA PROBRAR LAS FUNCIONES DE SAMSUNG****//////
        case GDK_KEY_p:                                                                             //Caso de la tecla S
        {
            SemcoTC90527_GetDatas(apuntador_datos_demulador);
            std::cout<<"SNR: "<<apuntador_datos_demulador->SNR<<std::endl;
            std::cout<<"pre_BER: "<<apuntador_datos_demulador->pre_BER<<std::endl;
            std::cout<<"post_BER: "<<apuntador_datos_demulador->post_BER<<std::endl;
            std::cout<<"Code Rate: "<<apuntador_datos_demulador->code_rate<<std::endl;
            std::cout<<"Mode: "<<apuntador_datos_demulador->mode_90527<<std::endl;
            std::cout<<"block: "<<apuntador_datos_demulador->bLock<<std::endl;
        }
        break;                                                                                      //Romper
    }
}

//! Variable para indicar la posicion del buffer de 5,1 Mb a copiar a la data media del reproductor.
/*!
     Posiciones={0,245760,491520,737280,983040,1228800,1474560,1720320,1966080,2211840,2457600,2703360,2949120,3194880,3440640,3686400,3932160,4177920,4423680,4669440}.
*/
static int posicion_media_buffer=0;

//! Función de llamada de la función libvlc_media_new_callbacks para apuntar a la posición de los datos y guardar el tamaño del buffer a leer.
/*!
    \param *opaque apuntador al buffer rotativo.
    \param **datap espacio de alamacenamiento para un apuntador de datos.
    \param *sizep apuntador al tamaño del buffer de datos.
    \return 0 operación exitosa.
*/
int abrir_media(void *opaque, void **datap, long unsigned int *sizep)
{
    *sizep = tamanio_buffer;                                                //Asigna el tamaño de los datos
    *datap = opaque;                                                        //Coloca como data el apuntador al buffer rotativo
    return 0;                                                               //Devuelve cero
}

//! Función de llamada de la función libvlc_media_new_callbacks para leer los datos.
/*!
    \param *opaque apuntador al buffer rotativo.
    \param *buf apuntador al buffer de lectura.
    \param len longuitud de los datos a leer.
    \return devuelve la cantidad de datos leidos.
*/
long int leer_media(void *opaque, unsigned char *buffer_de_llamada,long unsigned int longuitud_de_llamada)
{
    longuitud_de_llamada = tamanio_buffer_MPEG_TS;                                                 //Longuitud de datos a leer igual al tamanio_buffer_MPEG_TS
    std::memcpy(buffer_de_llamada,((unsigned char*)opaque+posicion_media_buffer), longuitud_de_llamada);       //copiar los dtoas del buffer rotativo al buffer de lectura
    procedimiento_retardo_milisegundos(2);                                      //Retardo par evitar uso excesivo del CPU
    posicion_media_buffer+=tamanio_buffer_MPEG_TS;                              //Aumentar posición_media_buffer con los datos ya leidos

    if(posicion_media_buffer>4669440||funcion_leer_estado_hotplug()!=1)         //Si posicion_media_buffer mayor a 4669440 ó dispositivo desconectado
        posicion_media_buffer=0;                                                //posicion_media_buffer igual a cero

    return tamanio_buffer_MPEG_TS;                                              //Devuelve la cantidad de datos leidos
}

//! Funcion de llamada de la función libvlc_media_new_callbacks para colocar la nueva posición en el buffer de datos.
/*!
    \param *opaque apuntador al buffer rotativo.
    \param offset posicion de nuevos datos leidos.
    \return 0 operación exitosa.
*/
int posicionar_media(void *opaque, uint64_t offset)
{
    offset=0;   //offset igual a cero
    return 0;   //retorna cero
}

//! Procedimiento procedimiento_media_callbacks.
/*!
    Procedimeinto para obtener la data media y luego mandarlar a reproducir con el reproductor.
*/
void procedimiento_media_callbacks(void)
{
    posicion_media_buffer=0;

    media = libvlc_media_new_callbacks(     //media igual a los datos BTS que llegan la puerto USB su llenado es asincrónico
                         instancia_vlc,     //Instancia vlc
                           abrir_media,     //función de llamada abrir_media()
                            leer_media,     //función de llamada leer_media()
                      posicionar_media,     //función de llamada posicionar_media()
                                  NULL,     //NULL cerrar media
     apuntador_al_buffer_recepcion_BTS);    //Apuntador al buffer roatativo

    libvlc_media_add_option(media, funcion_leer_id());                                      //Se coloca el ID del programa deseado a reproducir
    libvlc_media_player_set_media(reproductor, media);                                      //Se coloca el medio media en el reporductor
    libvlc_media_player_play(reproductor);                                                  //Reproducir
}

/// Variable para manejar las imagenes del layaout caja_horizontal_indicadores
static int estado=1;

//! Procedimiento procedimiento_imagenes_caja_horizontal_indicadores.
/*!
    Procedimiento para manejar las imagenes de estado_hotplug: conectado o desconectado, manejar las imagenes de intensidad de señal.
*/
int procedimiento_imagenes_caja_horizontal_indicadores(void* a)
{
        if(funcion_leer_estado_hotplug()==1&&estado)
        {
            icon = gdk_pixbuf_new_from_inline (-1, usb_conectado_en_linea, FALSE, NULL);           //Asignar los datos de la imagen a icon
            imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                //Asignar a imagen icon
            gtk_button_set_image (GTK_BUTTON(boton_estado_usb), GTK_WIDGET(imagen));                      //Añadir la imagen al boton boton_reproducir_pausar
            estado=0;
        }

        if(!funcion_leer_estado_hotplug()==1&&!estado)
        {
            icon = gdk_pixbuf_new_from_inline (-1, usb_desconectado_en_linea, FALSE, NULL);           //Asignar los datos de la imagen a icon
            imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                //Asignar a imagen icon
            gtk_button_set_image (GTK_BUTTON(boton_estado_usb), GTK_WIDGET(imagen));                      //Añadir la imagen al boton boton_reproducir_pausar
            estado=1;
        }

        return true;                                                                               //Devuelve verdadero
}

//! Procedimiento de llamada para destruir todos los widget cuando la ventana se cierre.
void procedimiento_de_llamada_destruir()
{
    libusb_close(manejador_dispositivo_usb);        //Cerrar el manejador del dispositivo
    libvlc_media_player_release(reproductor);       //Liberar el Reproductor de la sesión libvlc
    libvlc_release(instancia_vlc);                  //Cerrar la sesión de libvlc
    gtk_main_quit();                                //Finalizar la ventana de aplicación
    std::terminate();                               //Terminar todos los hilos y los demonios
}

#endif // CALLS_H_INCLUDED
