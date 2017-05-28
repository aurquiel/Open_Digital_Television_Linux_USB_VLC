#ifndef USB_H_INCLUDED
#define USB_H_INCLUDED

#include <libusb-1.0/libusb.h>  //Incluir la librería libusb
#include <iostream>             //Incluir la librería iostream
#include <unistd.h>             //Incluir la librería unistd.h

//------------------------------------CABEZERAS PROCEDMEINTOS Y FUNCIONES COMIENZO------------------------------
void procedimiento_hotplug(void);                                   //Procedimiento que ejecuta un hilo para el hotplug del dispositivo
int funcion_de_llamada_hotplug(struct libusb_context *ctx, struct libusb_device *dev,libusb_hotplug_event event, void *user_data); //Funcion de llamada para manejar el hotplug
int funcion_leer_estado_hotplug(void);                              //Funcion para leer el estado de hotplug
void procedimiento_escribir_estado_hotplug(int nuevo_estado);       //Procedimiento para escribir el estado de hotplug
bool funcion_leer_estado_frecuencia(void);                          //Funcion para leer de frecuencia
void procedimiento_escribir_estado_frecuencia(bool nuevo_estado);   //Procedimiento para escribir el estado de frecuencia
//------------------------------------CABEZERAS PROCEDMEINTOS Y FUNCIONES FIN-----------------------------------

//------------------------------------VARIABLES GLOBALES USB COMIENZO-------------------------------------------
/*!
  Variable estructura LIBUSB para manejar sesión de la librería.
*/
static libusb_context *contexto_usb = NULL;                 //Variable para la sesion LIBUSB
/*!
  Variable estructura LIBUSB para manejar el dispositivo USB.
*/
libusb_device_handle *manejador_dispositivo_usb;            //Variable LIBUSB para el manejo del dispositivo
/*!
  Variable estado_hotplug:
  - Si estado_hotplug Igual 0, dispositivo no conectado.
  - Si estado_hotplug Igual 1, dispositivo conectado.
  - Si estado_hotplug Igual 2, dispositivo desconectado.
*/
static int estado_hotplug = 0;                              //Variable para alojar el estado de hotplug del dispositivo
/*!
  Variable estado_frecuencia:
  - Si estado_frecuencia FALSO, frecuencia no sintonizada.
  - Si estado_frecuencia VERDADERO, frecuencia sintonizada.
*/
static bool estado_frecuencia = false;                        //Variable para saber si la frecuencia del canal fue sintonizada
//------------------------------------VARIABLES GLOBALES USB FIN------------------------------------------------

//! Función de llamada para el hotplug.
/*!
  \param *contexto_usb Contexto de la sesión de LIBUSB.
  \param *dispositivo_usb_hotplug_funcion_llamada Lista de dispositivos USB.
  \param evento_usb_hotplug_funcion_llamada libusb_hotplug_event Eventos de hotplug LIUSB.
  \param *data_usuario_hotplug_funcion_llamada Datos a pasar al cuerpo de la función.
  \return 0.

  Advertencias que muestra:
  - Si no puede abrirse el dispositivo USB: "Error: No pudo abrirse el dispositivo USB".
  - Si ocurre un evento inesperado: "Error: Evento LIBUSB inesperado, no se puede manejar".
  - Si el dispositivo USB se conecta: "El dispositivo esta conectado".
  - Si el dispositivo USB se desconecta: "El dispositivo fue desconectado".
*/
int funcion_de_llamada_hotplug(struct libusb_context *contexto_usb, struct libusb_device *dispositivo_usb_hotplug_funcion_llamada,
                     libusb_hotplug_event evento_usb_hotplug_funcion_llamada, void *data_usuario_hotplug_funcion_llamada)
{
    libusb_set_debug(contexto_usb, 0);

    if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == evento_usb_hotplug_funcion_llamada)                                                  //Si LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED es igual a evento_usb_hotplug_funcion_llamada
    {

        int respuesta = libusb_open(dispositivo_usb_hotplug_funcion_llamada, &manejador_dispositivo_usb);   //Tratar de abrir el dispositivo USB

        if (LIBUSB_SUCCESS != respuesta)
            std::cout<<"Error: No pudo abrirse el dispositivo USB"<<std::endl;                              //Si LIBUSB_SUCCESS distinto a respuesta, mostarar la siguiente advertencia

    } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == evento_usb_hotplug_funcion_llamada)                      //Sino, si LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT es igual a evento_usb_hotplug_funcion_llamada
    {
        if (manejador_dispositivo_usb)                                                                      //Si manejador_dispositivo_usb_hotplug_funcion_llamada es verdadero
        {
            libusb_close(manejador_dispositivo_usb);                                                        //Cerrar el manejador del dispositivo
            manejador_dispositivo_usb  = NULL;                                                              //Asignar NULL al manejador del dispositivo
        }
    }
    else                                                                                                    //Sino
        std::cout<<"Error: Evento LIBUSB inesperado, no se puede manejar"<<evento_usb_hotplug_funcion_llamada<<std::endl; //Mostrar la soguiente advertencia

    ++estado_hotplug;                                                                   //Incrementar estado_hotplug

    if (estado_hotplug == 1)                                                            //Si estado_hotplug igual a uno
    {
        std::cout<<"El dispositivo esta conectado"<<std::endl;                          //Mostar la siguiente advertencia
    }

    if (estado_hotplug == 2)                                                            //Si estado_hotplug es igual a dos
    {
        procedimiento_escribir_estado_frecuencia(false);                                //Escribir estado de frecuencia falso
        std::cout<<"El dispositivo fue desconectado"<<std::endl;                        //Mostar la siguiente advertencia
    }

    return 0;                                                                           //Retorna cero
}

//! Procedimiento que ejecuta un hilo para controlar el estado de hotplug del dispositivo USB.
/*!
  Variable estado_hotplug:
  - Si estado_hotplug Igual 0, dispositivo no conectado.
  - Si estado_hotplug Igual 1, dispositivo conectado.
  - Si estado_hotplug Igual 2, dispositivo desconectado.

  Advertencias que muestra:
  - Si no puede realizarse el hotplug con el dispositivo USB "Error: creando funcion de llamda para el hotplug" <b>*Depende de la versión del Kernel, si este acepta hotplug </b>.
  - Si el dispositivo USB no esta conectado: "Por favor conecte el dispositivo".
*/
void procedimiento_hotplug(void)
{
    while (1)                                                               //Ciclo infinito para el hilo
    {
        libusb_hotplug_callback_handle manejador_dispositivo_usb_hotplug;   //Variable libusb para el manejo del dispositivo hotplug
        libusb_init(&contexto_usb);                                                  //Libusb sesion nula

        int estado_funcion_llamada = libusb_hotplug_register_callback(contexto_usb,                                                                 //Sesion libusb
                                                (libusb_hotplug_event) (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),    //events bitwise or of events that will trigger this callback
                                                LIBUSB_HOTPLUG_ENUMERATE,                                                                           //flags hotplug callback flags
                                                1204,                                                                                               //ID vendor del dispositivo usb para hotplug
                                                4100,                                                                                               //ID product del dispositivo usb para hotplug
                                                LIBUSB_HOTPLUG_MATCH_ANY,                                                                           //the device class to match or \ref LIBUSB_HOTPLUG_MATCH_ANY
                                                funcion_de_llamada_hotplug,                                                                         //Función de llamada para el hotplug.
                                                NULL,                                                                                               //Data de usuario para pasar a la función de llamada
                                                &manejador_dispositivo_usb_hotplug);

        if (LIBUSB_SUCCESS != estado_funcion_llamada)                                       //Si LIBUSB_SUCCESS==0 distinto a estado_funcion_llamada
        {
            std::cout<<"Error: creando funcion de llamda para el hotplug"<<std::endl;       //Mostar la siguiente advertencia
            libusb_exit(contexto_usb);                                                      //Salir
        }

        while (estado_hotplug < 2)                                                          //Mientras estado_hotplug menor que dos hacer
        {
            if (estado_hotplug == 0)                                                        //Si estado hotplug es igual a cero
                std::cout<<"Por favor conecte el dispositivo"<<std::endl;                   //Mostar la siguiente advertencia

            libusb_handle_events_completed(contexto_usb, NULL);                                     //El programa se queda a la espera de que un evento ocurra
            usleep(10000);                                                                  //Retardo 10000 microsegundos
        }

        estado_hotplug = 0;                                                                 //Reinicializacion del estado hotplug
        libusb_hotplug_deregister_callback(contexto_usb, manejador_dispositivo_usb_hotplug);        //deregistar
        libusb_exit(contexto_usb);                                                                  //cerrar libusb
    }
}

//! Función para leer el esatdo de hotplug del dispositivo USB.
/*!
 @return estado_hotplug.

  Variable estado_hotplug:
  - Si estado_hotplug Igual 0, dispositivo no conectado.
  - Si estado_hotplug Igual 1, dispositivo conectado.
  - Si estado_hotplug Igual 2, dispositivo desconectado.
*/
int funcion_leer_estado_hotplug(void)
{
    return estado_hotplug;          //Devolver el estado de hotplug
}

//! Procedimiento para escribir el estado de hotplug del dispositivo USB.
/*!
  \param nuevo_estado

  Variable nuevo_estado:
  - Si nuevo_estado Igual 0 se escribe estado_hotplug Igual 0, dispositivo no conectado.
  - Si nuevo_estado Igual 1 se escribe estado_hotplug Igual 1, dispositivo conectado.
  - Si nuevo_estado Igual 2 se escribe estado_hotplug Igual 2, dispositivo desconectado.


*/
void procedimiento_escribir_estado_hotplug(int nuevo_estado)
{
    estado_hotplug = nuevo_estado;  //Asignar el nuevo estado de hotplug
}

//! Función para leer el estado si una frecuencia fue sintonizada.
/*!
  @return estado_frecuencia.

  Variable estado_frecuencia:
  - Si estado_frecuencia FALSO, frecuencia no sintonizada.
  - Si estado_frecuencia VERDAADERO, frecuencia sintonizada.
*/
bool funcion_leer_estado_frecuencia(void)
{
    return estado_frecuencia;       //Devolver el estado de frecuencia

}

//! Procedimiento para escribir el estado si una frecuencia fue sintonizada.
/*!
  \param nuevo_estado

  Variable nuevo_estado:
  - Si nuevo_estado FALSO, se escribe estado_frecuencia FALSO, frecuencia no sintonizada.
  - Si nuevo_estado VERDADERO, se escribe estado_frecuencia VERDADERO, frecuencia sintonizada.

*/
void procedimiento_escribir_estado_frecuencia(bool nuevo_estado)
{
    estado_frecuencia = nuevo_estado; //Asignar el nuevo estado de frecuencia
}

#endif // USB_H_INCLUDED
