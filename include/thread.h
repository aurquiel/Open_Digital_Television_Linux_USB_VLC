#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include "samsung.h"    //Incluir la cabezera samsung.h
#include "usb.h"        //Incluir la cabezera usb.h
#include <vlc/vlc.h>    //Incluir la librería vlc
#include <iostream>     //Incluir la librería iostream
#include <cstring>      //Incluir la librería cstring

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------
static void procedimiento_de_llamada_leer_transferencia_usb(struct libusb_transfer *transferencia_usb_2);  //Procedimiento de llamada de la funcion funcion_crear_transferencia_usb, con la cual se obtienen los paquetes BTS y se realiza una nueva transferencia
static struct libusb_transfer *funcion_crear_transferencia_usb(libusb_device_handle *manejador_dispositivo_usb,int tamanio_buffer_MPEG_TS); //Funcion que crear la transferencia y devuelve una estructura de transferencia LIBUSB
int procedimiento_ciclo_recepcion_BTS(void);                                                //Procedimiento ejecutada por un hilo para realizar la cola de transferencia y manejar eventos de LIBUSB
bool funcion_leer_estado_detener(void);                                                     //Funcion para leer la varibale estado_detener
void procedimiento_escribir_estado_detener(bool nuevo_estado);                              //Procedimiento para escribir la varibale estado_detener
int funcion_leer_calidad_senial(void);
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//------------------------------------VARIABLES GLOBALES THREAD COMIENZO----------------------------------------                                              //Archivo donde se escriben los paquetes BTS

//!Constante entera tamaño del buffer = 245760.
const int tamanio_buffer_MPEG_TS = 245760;                                                              //Tamaño del buffer de recepción usb de recepccion de BTS 245760 bytes

//!Buffer para alojar los paquetes BTS.
static unsigned char MPEG2_TS[tamanio_buffer_MPEG_TS];                                                  //Creación del buffer de recepcion

//!Constante entera tamaño del buffer = 4915200.
const int tamanio_buffer = 4915200;                                                                     //Tamaño del buffer del buffer rotativo para alojar paquetes BTS 4915200 bytes

//!Buffer rotativo para alojar los paquetes BTS.
static unsigned char buffer[tamanio_buffer];                                                            //Buffer rotativo


//!Variable para incrementar el apuntor del buffer y posicionarse en varias posiciones de memoria dentro del buffer. Posiciones={0,245760,491520,737280,983040,1228800,1474560,1720320,1966080,2211840,2457600,2703360,2949120,3194880,3440640,3686400,3932160,4177920,4423680,4669440}.
static int posicion = 0;                                                                                   //Variable para incrementar el apuntor buffer

//!Variable para evitar labasura de las primeras transmisiones.
static int contador_evitar_basura = 0;                                                                  //Contador para evitar la basura en las primeras transmisiones


//!Variable estado_detener para detectar si el reproductor esta detenido.
/*!
  Variable estado_detener:
  - Si estado_detener igual FALSO, reproductor no detenido.
  - Si estado_detener igual VERDADERO, reproductor detenido.
*/
static bool estado_detener = false;                                                                     //Variable para saber si el boton detener fue presionado

//!Varibale que apunta al buffer de transferencias USB
unsigned char *apuntador_al_buffer_recepcion_BTS=buffer;                                                //Variable que apunta al buffer de transferencias USB

//!Variable mostrar mensaje de recepción
static bool mensasje_recepcion = true;                                                                    //Variable para mostrar estado de recpcion
//------------------------------------VARIABLES GLOBALES THREAD FIN---------------------------------------------

//! Procedimiento procedimiento_de_llamada_leer_transferencia_usb recibe la data BTS y la escribe al buffer rotativo luego realiza otra transferencia.
/*!
  \param *transferencia_usb_2 Estructura LIBUSB de transferencia.

    Advertencias que muestra:
  - Si transferencias completada: "Recibiendo paquetes BTS".
  - Si nueva transferencia realizada sin exitos: Error nueva transferencia".
*/
static void procedimiento_de_llamada_leer_transferencia_usb(struct libusb_transfer *transferencia_usb_2)
{
    if(transferencia_usb_2->status == LIBUSB_TRANSFER_COMPLETED)                //Si es estado de la transferencia es igual a LIBUSB_TRANSFER_COMPLETED==0
    {
        if(mensasje_recepcion)                                                  //si mensaje de recepción es verdadero
            std::cout<<"Recibiendo paquetes BTS"<<std::endl;                    //Mostar la siguiente advertencia
        mensasje_recepcion=false;                                               //mensaje_rececpcion igual false
    }
    else                                                                        //Sino
        std::cout<<"Error: "<<transferencia_usb_2->status<<std::endl;           //Mostar la siguiente advertencia, con el numero del status de transferencia LIBUSB

    if(contador_evitar_basura <= 4)                                               //Si contador de basura es menor o igual a cuatro
        contador_evitar_basura++;                                               //Incrementar contador_evitar_basura

    if (contador_evitar_basura > 4)                                             //Si contador_evitar_basura mayor a 6 enonces
    {
        std::memcpy((buffer+posicion),transferencia_usb_2->buffer,tamanio_buffer_MPEG_TS);  //Copiar el los datos del buffer MPEG2_TS alojados en la estructura LIBUSB al buffer rotativo
        posicion+=tamanio_buffer_MPEG_TS;                                                   //Incrementar la posicion despues de copiar los datos
        if(posicion>4669440)                                                                //Si se copió la ultima posición
            posicion=0;                                                                     //Comienza en la posición cero
    }

    int respuesta_transferencia_llamada = libusb_submit_transfer(transferencia_usb_2);  //Realizar una nueva transferencia, devulve cero si fue exitosa

    if(respuesta_transferencia_llamada != 0)                                            //Si respuesta_transferencia_llamada es diferente de cero
        std::cout<<"Error nueva transferencia: "<<respuesta_transferencia_llamada<<std::endl; //Mostar la siguiente advertencia

}

//! Función para crear una transferencia USB.
/*!
  \param *manejador_dispositivo_usb variable LIBUSB para manejar el dispositivo.
  \param tamanio_buffer constante tamaño del bufer de recepción de los paquetes BTS.
  \return Estructura LIBUSB de transferencia.
*/
static struct libusb_transfer *funcion_crear_transferencia_usb (libusb_device_handle *manejador_dispositivo_usb,int tamanio_buffer)
{
    struct libusb_transfer *transferencia_usb_2;        //Estructura LIBUSB para manejar la transferencia

    transferencia_usb_2 = libusb_alloc_transfer(0);     //Alojar la transferencia, para el conjunto de datos no isócrono colocar cero

    libusb_fill_interrupt_transfer(transferencia_usb_2, //Estructura LIBUSB para manejar la transferencia
                             manejador_dispositivo_usb, //Manejador del dispositivo
                                                  0x82, //Endpoint
                                              MPEG2_TS, //Buffer
                                        tamanio_buffer, //Tamanio del buffer
       procedimiento_de_llamada_leer_transferencia_usb, //Procedimiento de llamada
                                                  NULL, //NULL
                                                 20000);//Tiempo de espera para esperar la transferencia, cero indefinido



    return transferencia_usb_2;                         //Devuelve la transferencia
}

//! Procedimiento que ejecuta un hilo para crear una cola de buffers y recibir los paquetes BTS, manejar los eventos LIBUSB y liberar las transferencias si el dispositivo USB es desconectado.
/*!
  @return 0 al finalizar el hilo.

  Advertencias que muestra:
  - Si errores en eventos de transferncias: "handle_events()error: número de error".
  - Si dipositivo desconectado: "El dispsotivo fue desconectado".
  - Si dipositivo está desconectado: "El dispositivo no esta conectado: No se puedo incializar el proceso para recibir los paquetes BTS".
*/
int procedimiento_ciclo_recepcion_BTS(void)
{
    mensasje_recepcion = true;                                                                                    //mensaje_recepcion verdadero
    posicion = 0;
    contador_evitar_basura = 0;                                                                                 //Inicializar contador_evitar_basura
    struct libusb_transfer *transferencia_usb;                                                                  //Estructura LIBUSB para manejar la transferencia

    if (funcion_leer_estado_hotplug() == 1)                                                                     //Si el estado de hotplug es igual a uno
    {
        for(int i=0; i<64; i++)                                                                                 //Poner en cola 64 buffers para la recepción de paquetes
        {
            transferencia_usb = funcion_crear_transferencia_usb(manejador_dispositivo_usb, tamanio_buffer_MPEG_TS);     //Funcion que llena y recibe la estructura de transferencia_usb , recibe el manejador del dispositivo y el tamaño del buffer
            libusb_submit_transfer(transferencia_usb);                                                          //Realizar la transferencia
        }

        /*Manejar eventos LIBUSB*/
        while(1)                                                                                                //Ciclo infinito para el hilo
        {
            int respuesta_eventos = libusb_handle_events(contexto_usb);                                         //Para manejar los eventos de LIBUSB mientras se hacen las transferencias

            if(respuesta_eventos < 0) std::cout<<"handle_events()error: "<<respuesta_eventos<<std::endl;        //Si resuestas_eventos menor a cero entonces, mostrar la siguiente advertencia

            if (funcion_leer_estado_hotplug() > 1)                                                              //Si el estado dehotplug es mayor a uno
            {
                libusb_cancel_transfer(transferencia_usb);                                                      //Cancelar transferencias usb
                procedimiento_escribir_estado_detener(true);                                                    //Escribir la variable estado_detener=verdadero, esto inicia el stop del video
                posicion=0;
                std::cout<<"El dispsotivo fue desconectado"<<std::endl;                                         //Mostrar la siguiente advertencia
                break;                                                                                          //Romper
            }

            procedimiento_retardo_milisegundos(2);                                                              //Porcedimeinto de retardo prar evitar uso excesivo del CPU
        }

    }
    else                                                                                                        //Sino
        std::cout<<"El dispositivo no esta conectado: No se puedo incializar el proceso para recibir los paquetes BTS"<<std::endl; //mostar la siguiente advertencia

    return 0;                                                                                                   //Devolver cero
}

//! Función para leer la variable estado_detener.
/*!
  \return estado_detener.

  Variable estado_detener:
  - Si estado_detener FALSO, reproductor no detenido.
  - Si estado_detener VERDADERO, reproductor detenido.
*/
bool funcion_leer_estado_detener(void)
{
    return estado_detener;                              //Devolver el estado de detener
}

//! Procedimiento para escribir la variable estado_detener.
/*!
  \param nuevo_estado

  Variable estado_detener:
  - Si estado_detener FALSO, reproductor no detenido.
  - Si estado_detener VERDADERO, reproductor detenido.
*/
void procedimiento_escribir_estado_detener(bool nuevo_estado)
{
    estado_detener = nuevo_estado;                     //Asignar el nuevo estado de detener
}

#endif // THREAD_H_INCLUDED
