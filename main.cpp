#include <iostream>              //Incluir la librería iostream
#include <string>                //Incluir la librería string
#include <thread>                //Incluir la librería thread
#include <stdlib.h>              //Incluir la librería stdlib.h
#include <gtk/gtk.h>             //Incluir la librería gtk
#include <gdk/gdkx.h>            //Incluir la librería ddk
#include <vlc/vlc.h>             //Incluir la librería vlc
#include <X11/Xlib.h>            //Incluir la librería Xlib

#include "include/calls.h"       //Incluir la cabezera calls.h

//------------------------------------VARIABLES GLOBALES GRAFICAS FIN-------------------------------------------
//! Función main C++.
int main(int argc, char *argv[])
{
    XInitThreads();                                                                 //Inicializar Xlib para programas con hilos

    std::thread hilo_hotplug_USB(procedimiento_hotplug);                            //Hilo que ejecuta el estado de hotplug
    hilo_hotplug_USB.detach();                                                      //Crea un demonio del hilo hilo_hotplug_USB

    std::thread hilo_deterner_reproductor_hotplug(procedimiento_hilo_detener);      //Hilo que ejecuta la función de recibir los paquetes BTS
    hilo_deterner_reproductor_hotplug.detach();                                     //Crea un demonio del hilo hilo_deterner_reproductor_hotplug

    gdk_threads_add_timeout(2,procedimiento_imagenes_caja_horizontal_indicadores,NULL); //Función de gdk dedicada a chequear el estado de hotplug cada dos segundos para realizar el cambio de las imagenes

    // Función que se encargar de comenzar el ciclo infito de GTK3.
    gtk_init(&argc, &argv);

        GtkSettings *default_settings = gtk_settings_get_default();                 //Obtener la configuracion de gtk3
        g_object_set(default_settings, "gtk-button-images", TRUE, NULL);            //Habilitar el uso de imagenes en los botones
        //------------------------------------CREACION DE LOS WIDGETS COMIENZO----------------------------------

        ventana = gtk_window_new(GTK_WINDOW_TOPLEVEL);                                          //Creación una nueva ventana
        gtk_container_set_border_width(GTK_CONTAINER(ventana),0);                               //Establecer los bordes de la ventana
        gtk_window_set_title(GTK_WINDOW(ventana), "TDA Linux Cendit");                          //Colocar tìtulo de la ventana
        icon = gdk_pixbuf_new_from_inline (-1, icon_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        gtk_window_set_icon (GTK_WINDOW(ventana),icon);                                         //Asignar icon al widget

        //Inicio Layaouts
        rejilla = gtk_grid_new();                                                               //Creación rejilla para designar la distribución de los widgets
        caja_vertical_mayor = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);                         //Creación caja horizontal para alojar botones de control
        caja_vertical_botones_canales = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);               //Creación caja vertical para alojar botones de los canales
        gtk_box_set_spacing (GTK_BOX(caja_vertical_botones_canales),1);                         //Ingreseando espaciado entre elementos(widgets)
        caja_horizontal_botones_de_control = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);        //Creación caja horizontal para alojar botones de control
        caja_horizontal_indicadores = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);               //Creación caja horizontal para alojar botones de indicadores
        //Fin Layaouts

        desplazamiento = gtk_adjustment_new(0,0,5,1,0,0);                                       //Creacion del ajuste para la escala del widget ventana_desplazamiento
        ventana_desplazamiento = gtk_scrolled_window_new (NULL,desplazamiento);                 //Añadiendo el ajusto como tipo vertical
        gtk_widget_set_size_request (ventana_desplazamiento, 150, 0);                           //Indicando las diensiones del widget ventana_desplazamiento
        gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (ventana_desplazamiento), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); //Politicas de depslazamiento

        reproductor_widget = gtk_drawing_area_new();                                            //Dibujar el area del reproductor
        gtk_widget_set_size_request (reproductor_widget, 800, 400);                             //Fijar las dimensiones del reproductor
        gtk_widget_set_hexpand (reproductor_widget, TRUE);                                      //Permitir que le reproductor se expanda horizontalmente
        gtk_widget_set_vexpand (reproductor_widget, TRUE);                                      //Permitir que el reproductor se expanda verticalmente
        instancia_vlc = libvlc_new(0, NULL);                                                    //Creación de una cesión de libvlc
        reproductor = libvlc_media_player_new(instancia_vlc);                                   //Creación del reproducctor

        frame_estado_usb = gtk_frame_new (NULL);                                                //Creación nuevo marco
        etiqueta_estado_usb = gtk_label_new ("Conexión USB: ");                                 //Creación etiqueta
        gtk_container_add (GTK_CONTAINER (frame_estado_usb), etiqueta_estado_usb);              //Añadir etiqueta al marco
        gtk_frame_set_shadow_type (GTK_FRAME(frame_estado_usb),GTK_SHADOW_NONE);                //Quitar borde al marco

        frame_senial = gtk_frame_new (NULL);                                                    //Creación nuevo marco
        etiqueta_senial = gtk_label_new ("  Señal: ");                                          //Creación etiqueta
        gtk_container_add (GTK_CONTAINER (frame_senial), etiqueta_senial);                      //Añadir etiqueta al marco
        gtk_frame_set_shadow_type (GTK_FRAME(frame_senial),GTK_SHADOW_NONE);                    //Quitar borde al marco

        boton_estado_usb = gtk_button_new();                                                    //Creación del boton boton_estado_usb
        icon = gdk_pixbuf_new_from_inline (-1, usb_desconectado_en_linea, FALSE, NULL);         //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image(GTK_BUTTON(boton_estado_usb), GTK_WIDGET(imagen));                 //Añadir la imagen al boton boton_reproducir_pausar
        gtk_button_set_relief(GTK_BUTTON (boton_estado_usb), GTK_RELIEF_NONE);                  //Cambiar estilo al boton
        gtk_button_set_focus_on_click(GTK_BUTTON (boton_estado_usb), false);                    //Cambiar estilo al boton

        boton_senial = gtk_button_new();                                                        //Creación del boton boton_senial
        icon = gdk_pixbuf_new_from_inline (-1, senial_00_en_linea, FALSE, NULL);                //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image(GTK_BUTTON(boton_senial), GTK_WIDGET(imagen));                     //Añadir la imagen al boton boton_reproducir_pausar
        gtk_button_set_relief(GTK_BUTTON (boton_senial), GTK_RELIEF_NONE);                      //Cambiar estilo al boton
        gtk_button_set_focus_on_click(GTK_BUTTON (boton_senial), false);                        //Cambiar estilo al boton

        boton_reproducir_pausar = gtk_button_new_with_label("Reproducir");                      //Creación  del boton reproducir y pausar
        icon = gdk_pixbuf_new_from_inline (-1, reproducir_en_linea, FALSE, NULL);               //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_reproducir_pausar), GTK_WIDGET(imagen));         //Añadir la imagen al boton boton_reproducir_pausar

        boton_detener = gtk_button_new_with_label("Detener");                                   //Creación  del boton detener
        icon = gdk_pixbuf_new_from_inline (-1, detener_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_detener), GTK_WIDGET(imagen));                   //Añadir la imagen al boton boton_detener

        volumen = gtk_volume_button_new();                                                      //Creación  del boton volumen
        ajuste = gtk_adjustment_new(0,0,100,1,0,0);                                             //Creacion del ajuste para la escala del volumen: valor incial, inferior, superior, incrmento, incremento pagina, tamaño pagina
        gtk_scale_button_set_adjustment(GTK_SCALE_BUTTON(volumen),ajuste);                      //Añadir la escala al boton de volumen
        gtk_scale_button_set_value(GTK_SCALE_BUTTON(volumen),50);                               //Colocar el boton a 50%

        pantalla_completa = gtk_button_new();                                                   //Creación del boton para maximizar la pantalla
        icon = gdk_pixbuf_new_from_inline (-1, pantalla_completa_en_linea, FALSE, NULL);        //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(pantalla_completa), GTK_WIDGET(imagen));               //Añadir la imagen al boton boton_detener

        //Creación de los botones de los Canales Comienzo
        //Frecuencia de 521000 Hz Comienzo
        boton_canal_521000_57856 = gtk_button_new_with_label("VTV");
        boton_canal_521000_57857 = gtk_button_new_with_label("123 TV");
        boton_canal_521000_57858 = gtk_button_new_with_label("Colombeia");
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

        //Colocar imagenes para botones de canales comienzo
        //Frecuencia de 521000 Hz Comienzo
        icon = gdk_pixbuf_new_from_inline (-1, vtv_en_linea, FALSE, NULL);                      //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_521000_57856), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57856

        icon = gdk_pixbuf_new_from_inline (-1, tv123_en_linea, FALSE, NULL);                    //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_521000_57857), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57857

        icon = gdk_pixbuf_new_from_inline (-1, colombeia_en_linea, FALSE, NULL);                //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_521000_57858), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57858

        icon = gdk_pixbuf_new_from_inline (-1, venevision_en_linea, FALSE, NULL);               //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_521000_57859), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57859

        icon = gdk_pixbuf_new_from_inline (-1, alba_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_521000_57860), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57860

        icon = gdk_pixbuf_new_from_inline (-1, vtv_en_linea, FALSE, NULL);                      //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_521000_57880), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57880
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        icon = gdk_pixbuf_new_from_inline (-1, tves_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_533000_57920), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57920

        icon = gdk_pixbuf_new_from_inline (-1, antv_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_533000_57921), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57921

        icon = gdk_pixbuf_new_from_inline (-1, sibci_en_linea, FALSE, NULL);                    //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_533000_57922), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57922

        icon = gdk_pixbuf_new_from_inline (-1, tves_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_533000_57944), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_521000_57944
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        icon = gdk_pixbuf_new_from_inline (-1, fanbtv_en_linea, FALSE, NULL);                   //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_539000_57952), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_539000_57952

        icon = gdk_pixbuf_new_from_inline (-1, cctv_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_539000_57953), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_539000_57953

        icon = gdk_pixbuf_new_from_inline (-1, avilatv_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_539000_57954), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_539000_57954

        icon = gdk_pixbuf_new_from_inline (-1, pdvsatv_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_539000_57955), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_539000_57955

        icon = gdk_pixbuf_new_from_inline (-1, russiatoday_en_linea, FALSE, NULL);              //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_539000_57956), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_539000_57956

        icon = gdk_pixbuf_new_from_inline (-1, avilatv_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_539000_57976), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_539000_57976
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        icon = gdk_pixbuf_new_from_inline (-1, vive_en_linea, FALSE, NULL);                     //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_527000_57888), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_527000_57888

        icon = gdk_pixbuf_new_from_inline (-1, telesur_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_527000_57889), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_527000_57889

        icon = gdk_pixbuf_new_from_inline (-1, meridiano_en_linea, FALSE, NULL);                //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_527000_57890), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_527000_57890

        icon = gdk_pixbuf_new_from_inline (-1, televen_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_527000_57891), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_527000_57891

        icon = gdk_pixbuf_new_from_inline (-1, conciencia_en_linea, FALSE, NULL);               //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_527000_57892), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_527000_57892

        icon = gdk_pixbuf_new_from_inline (-1, telesur_en_linea, FALSE, NULL);                  //Asignar los datos de la imagen a icon
        imagen = GTK_IMAGE(gtk_image_new_from_pixbuf(icon));                                    //Asignar a imagen icon
        gtk_button_set_image (GTK_BUTTON(boton_canal_527000_57912), GTK_WIDGET(imagen));        //Añadir la imagen al boton del canal boton_canal_527000_57912
        //Frecuencia de 527000 Hz Fin
        //Colocar imagenes para botones de canales fin

        //------------------------------------CREACION DE LOS WIDGETS FIN---------------------------------------

        //------------------------------------EMPAQUETADO DE LOS WIDGETS COMIENZO-------------------------------

        //Insertar en la caja_vertical_botones_canales_1 y en caja_vertical_botones_canales_2 los botones de los canales comienzo
        //Frecuencia de 521000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_521000_57856, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_521000_57857, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_521000_57858, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_521000_57859, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_521000_57860, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_521000_57880, FALSE, FALSE, 0);
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_533000_57920, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_533000_57921, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_533000_57922, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_533000_57944, FALSE, FALSE, 0);
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_539000_57952, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_539000_57953, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_539000_57954, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_539000_57955, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_539000_57956, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_539000_57976, FALSE, FALSE, 0);
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_527000_57888, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_527000_57889, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_527000_57890, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_527000_57891, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_527000_57892, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(caja_vertical_botones_canales), boton_canal_527000_57912, FALSE, FALSE, 0);
        //Frecuencia de 527000 Hz Fin
        //Insertar en la caja_vertical_botones_canales_1 y en caja_vertical_botones_canales_2 los botones de los canales fin

        gtk_container_add(GTK_CONTAINER(ventana_desplazamiento),GTK_WIDGET(caja_vertical_botones_canales));                     //Insertar en l ventana_desplazamiento la caja_vertical_botones_canales

        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_de_control), boton_reproducir_pausar, FALSE, FALSE, 0);              //Insertar en la caja_horizontal_botones_de_control el boton_reproducir_pausar
        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_de_control), boton_detener, FALSE, FALSE, 0);                        //Insertar en la caja_horizontal_botones_de_control el boton_detener
        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_de_control), volumen, FALSE, FALSE, 0);                              //Insertar en la caja_horizontal_botones_de_control el boton volumen
        gtk_box_pack_start(GTK_BOX(caja_horizontal_botones_de_control), pantalla_completa, FALSE, FALSE, 0);                    //Insertar en la caja_horizontal_botones_de_controlel boton maxima_pantalla

        gtk_box_pack_start(GTK_BOX(caja_horizontal_indicadores), frame_estado_usb, FALSE, FALSE, 0);                            //Insertar en la caja_horizontal_indicadores frame_estado_usb
        gtk_box_pack_start(GTK_BOX(caja_horizontal_indicadores), boton_estado_usb, FALSE, FALSE, 0);                            //Insertar en la caja_horizontal_indicadores boton_estado_usb
        gtk_box_pack_start(GTK_BOX(caja_horizontal_indicadores), frame_senial, FALSE, FALSE, 0);                                //Insertar en la caja_horizontal_indicadores frame_senial
        gtk_box_pack_start(GTK_BOX(caja_horizontal_indicadores), boton_senial, FALSE, FALSE, 0);                                //Insertar en la caja_horizontal_indicadores boton_senial

        gtk_grid_attach(GTK_GRID(rejilla),reproductor_widget,0,0,50,1);                                                         //Insertar en la rejilla el reproductor_widget
        gtk_grid_attach(GTK_GRID(rejilla),ventana_desplazamiento,51,0,1,2);                                                     //Insertar en la rejilla la ventana_desplazamiento
        gtk_grid_attach(GTK_GRID(rejilla),caja_horizontal_botones_de_control,0,1,1,1);                                          //Insertar en la rejilla la vcaja_horizontal_botones_de_control
        gtk_grid_attach(GTK_GRID(rejilla),caja_horizontal_indicadores,49,1,1,1);                                                //Insertar en la rejilla la vcaja_horizontal_indicadores

        gtk_box_pack_start(GTK_BOX(caja_vertical_mayor), rejilla,TRUE,TRUE,0);                                                  //Insertar rejilla en la caja_vertical_mayor
        gtk_container_add(GTK_CONTAINER(ventana),caja_vertical_mayor);                                                          //Añadir a la ventana la caja_vertical_mayor
        //------------------------------------EMPAQUETADO DE LOS WIDGETS FIN------------------------------------

        //------------------------------------SEÑALES Y PROCEDIMEINTOS DE LLAMADA COMIENZO----------------------
        g_signal_connect(ventana, "destroy", G_CALLBACK(procedimiento_de_llamada_destruir), NULL);                              //Procedimiento de llamada para destruir todos los widget cuando la ventana se cierre

        //Señales y funciones de llamada para los botones de los canales Comienzo
        //Frecuencia de 521000 Hz Comienzo
        g_signal_connect(boton_canal_521000_57856, "clicked", G_CALLBACK(procedimiento_canal_521000_57856), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57857, "clicked", G_CALLBACK(procedimiento_canal_521000_57857), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57858, "clicked", G_CALLBACK(procedimiento_canal_521000_57858), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57859, "clicked", G_CALLBACK(procedimiento_canal_521000_57859), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57860, "clicked", G_CALLBACK(procedimiento_canal_521000_57860), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_521000_57880, "clicked", G_CALLBACK(procedimiento_canal_521000_57880), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        g_signal_connect(boton_canal_533000_57920, "clicked", G_CALLBACK(procedimiento_canal_533000_57920), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_533000_57921, "clicked", G_CALLBACK(procedimiento_canal_533000_57921), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_533000_57922, "clicked", G_CALLBACK(procedimiento_canal_533000_57922), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_533000_57944, "clicked", G_CALLBACK(procedimiento_canal_533000_57944), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        g_signal_connect(boton_canal_539000_57952, "clicked", G_CALLBACK(procedimiento_canal_539000_57952), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57953, "clicked", G_CALLBACK(procedimiento_canal_539000_57953), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57954, "clicked", G_CALLBACK(procedimiento_canal_539000_57954), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57955, "clicked", G_CALLBACK(procedimiento_canal_539000_57955), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57956, "clicked", G_CALLBACK(procedimiento_canal_539000_57956), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_539000_57976, "clicked", G_CALLBACK(procedimiento_canal_539000_57976), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        g_signal_connect(boton_canal_527000_57888, "clicked", G_CALLBACK(procedimiento_canal_527000_57888), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57889, "clicked", G_CALLBACK(procedimiento_canal_527000_57889), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57890, "clicked", G_CALLBACK(procedimiento_canal_527000_57890), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57891, "clicked", G_CALLBACK(procedimiento_canal_527000_57891), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57892, "clicked", G_CALLBACK(procedimiento_canal_527000_57892), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        g_signal_connect(boton_canal_527000_57912, "clicked", G_CALLBACK(procedimiento_canal_527000_57912), NULL);              //Procedimiento de llamada cuando el boton del canal se presione para inicializar el sintonizador, demodulador, colocar la frecuencia y el id del canal
        //Frecuencia de 527000 Hz Fin
        //Señales y funciones de llamada para los botones de los canales Fin

        //Señales y funciones de llamada para los botones de los canales Comienzo
        //Frecuencia de 521000 Hz Comienzo
        g_signal_connect(boton_canal_521000_57856, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57857, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57858, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57859, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57860, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_521000_57880, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 521000 Hz Fin
        //Frecuencia de 533000 Hz Comienzo
        g_signal_connect(boton_canal_533000_57920, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_533000_57921, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_533000_57922, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_533000_57944, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 533000 Hz Fin
        //Frecuencia de 539000 Hz Comienzo
        g_signal_connect(boton_canal_539000_57952, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57953, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57954, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57955, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57956, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_539000_57976, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 539000 Hz Fin
        //Frecuencia de 527000 Hz Comienzo
        g_signal_connect(boton_canal_527000_57888, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57889, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57890, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57891, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57892, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        g_signal_connect(boton_canal_527000_57912, "clicked", G_CALLBACK(procedimiento_de_llamada_hilo_recepcion), NULL);         //Procedimiento_de_llamada_hilo_recepcion cuando el boton del canal se presione
        //Frecuencia de 527000 Hz Fin
        //Señales y funciones de llamada para los botones de los canales Fin

        g_signal_connect(boton_reproducir_pausar, "clicked", G_CALLBACK(procedimiento_de_llamada_reproducir_pausar), NULL);       //Procedimiento_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione
        g_signal_connect(boton_detener, "clicked", G_CALLBACK(procedimiento_de_llamada_detener), NULL);                           //Procedimiento_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione
        g_signal_connect(volumen, "value_changed",G_CALLBACK(procedimiento_de_llamada_volumen), NULL);                            //Procedimiento_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione
        g_signal_connect(pantalla_completa, "clicked", G_CALLBACK(procedimiento_de_llamada_maxima_pantalla), NULL);               //Procedimiento_de_llamada_reproductor_widget cuando el boton pantalla_completa se presione
        g_signal_connect (G_OBJECT(ventana), "key_press_event", G_CALLBACK (procedimiento_de_llamada_tecla_presionada), NULL);    ////Procedimiento_de_llamada_tecla_presionada cuando una tecla del teclado es presionada
        g_signal_connect(G_OBJECT(reproductor_widget), "realize", G_CALLBACK(procedimiento_de_llamada_reproductor_widget), NULL); //Procedimiento_de_llamada_reproducir_pausar cuando el boton boton_reproducir_pausar se presione

        //------------------------------------SEÑALES Y PROCEDIMIENTOS DE LLAMADA FIN---------------------------

        gtk_widget_show_all(ventana);               //Mostar todo los Widgets

    gtk_main();                                     //Fin del ciclo infinito de GTK3

    return 0;                                       //Retorna 0
}

