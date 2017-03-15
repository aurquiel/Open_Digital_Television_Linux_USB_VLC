#ifndef USB_H_INCLUDED
#define USB_H_INCLUDED

#include <libusb-1.0/libusb.h>  //Incluir la librería libusb
#include <iostream>             //Incluir la librería iostream
#include <unistd.h>             //Incluir la librería unistd.h

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------
void funcion_conexion_usb();                                //Funcion para identificar, reclamar y abrir el dispositivo
void funcion_desconexion_usb();                             //Funcion para desconectarse del dispositivo
void funcion_hotplug();                                     //Funcion que ejecuta un hilo para el hotplug del dispositivo
int funcion_de_llamada_hotplug(struct libusb_context *ctx, struct libusb_device *dev,libusb_hotplug_event event, void *user_data); //Callback function to handle hotplug
int funcion_leer_estado_hotplug();                          //Funcion para leer el estado de hotplug
void funcion_escribir_estado_hotplug(int nuevo_estado);     //Funcion para escribir el estado de hotplug
bool funcion_leer_estado_frecuencia();                      //Funcion para leer de frecuencia
void funcion_escribir_estado_frecuencia(bool nuevo_estado); //Funcion para escribir el estado de frecuencia
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//------------------------------------VARIABLES GLOBALES GRAFICAS COMIENZO--------------------------------------
libusb_device_handle *manejador_dispositivo_usb;            //Variable libusb para el manejo del dispositivo
libusb_context *contexto_usb=NULL;                          //Variable para la sesion libusb
static int estado_hotplug = 0;                              //Variable para alojar el estado de hotplug del dispositivo
bool estado_frecuencia=false;                               //Variable para saber si la frecuencia del canal fue sintonizada
//------------------------------------VARIABLES GLOBALES GRAFICAS FIN-------------------------------------------

//! Función de llamada para el hotplug.
/*!
  \param *ctx conexto de la sesion de libusb
  \param *dev lista de dispositivo
  \param libusb_hotplug_event
  \param *user_dat
*/
int funcion_de_llamada_hotplug(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data)
{
    static libusb_device_handle *handle = NULL;
    struct libusb_device_descriptor desc;
    int rc;

    (void)libusb_get_device_descriptor(dev, &desc);

    if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event)
    {
        rc = libusb_open(dev, &handle);

        if (LIBUSB_SUCCESS != rc)
        {
            std::cout<<"Error: No pudo abrirse el dispositivo USB"<<std::endl;
        }

    } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event)
    {
        if (handle)
        {
            libusb_close(handle);
            handle = NULL;
        }
    } else std::cout<<"Unhandled event "<<event;

    ++estado_hotplug;                                                                   //iIncrementar estado_hotplug

    if (estado_hotplug==1)                                                              //Si estado_hotplug igual a uno
    {
        funcion_conexion_usb();                                                         //Lamar a la funcion funcion_conexion_usb para conectar y reclamar el dispositivo USB
        if (estado_hotplug==1) std::cout<<"El dispositivo esta conectado"<<std::endl;   //Mostar la siguiente advertencia

    }

    if (estado_hotplug==2) //Si estado_hotplug es igual a dos
    {
        funcion_escribir_estado_frecuencia(false);                  //Escribir estado de frecuencia falso
        std::cout<<"El dispositivo fue desconectado"<<std::endl;    //Mostar la siguiente advertencia
    }

    return 0;
}

//! Funcion que ejecuta un hilo para controlar el estado de hotplug del dispositivo
void funcion_hotplug()
{
    while (1)                                                               //Ciclo infinito para el hilo
    {
        libusb_hotplug_callback_handle manejador_dispositivo_usb_hotplug;   //Variable libusb para el manejo del dispositivo hotplug
        libusb_init(NULL);                                                  //Libusb sesion nula

        int estado_funcion_llamada = libusb_hotplug_register_callback(NULL,                                                                         //Sesion libusb nula
                                                (libusb_hotplug_event) (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),    //events bitwise or of events that will trigger this callback
                                                LIBUSB_HOTPLUG_ENUMERATE,                                                                           //flags hotplug callback flags
                                                1204,                                                                                               //ID vendor del dispositivo usb para hotplug
                                                4100,                                                                                               //ID product del dispositivo usb para hotplug
                                                LIBUSB_HOTPLUG_MATCH_ANY,                                                                           //the device class to match or \ref LIBUSB_HOTPLUG_MATCH_ANY
                                                funcion_de_llamada_hotplug,                                                                         //Función de llamada para el hotplug.
                                                NULL,                                                                                               //Data de usuario para pasar a la función de llamada
                                                &manejador_dispositivo_usb_hotplug);

        if (LIBUSB_SUCCESS != estado_funcion_llamada)                                       //Si LIBUSB_SUCCESS distinto a estado_funcion_llamada
        {
            std::cout<<"Error creating a hotplug callback"<<std::endl;                      //Mostar la siguiente advertencia
            libusb_exit(NULL);                                                              //Salir
        }

        while (estado_hotplug < 2)                                                          //Mientras estado_hotplug menor que dos hacer
        {
            if (estado_hotplug==0) std::cout<<"Por favor conecte el dispositivo"<<std::endl;//Si estado hotplug es igual a cero, mostar la siguiente advertencia

            libusb_handle_events_completed(NULL, NULL);                                     //El programa se queda a la espera de que un evento ocurra
            usleep(10000);                                                                  //Retardo 10000 microsegundos
        }

        estado_hotplug=0;                                                                   //Reinicializacion del estado hotplug
        libusb_hotplug_deregister_callback(NULL, manejador_dispositivo_usb_hotplug);        //deregistar
        libusb_exit(NULL);                                                                  //cerrar libusb
    }
}

//! Funcion para identificar, reclamar y abrir el dispositivo USB
void funcion_conexion_usb()
{
    libusb_device **lista_de_dispositivos=NULL;                                                                                         //Variable para guardar la lista de dispositivos, declarada NULL
    ssize_t numero_de_dispositivos;                                                                                                     //Varibale que aloja el numero de dispotivo de la lista de dispositivos

    int r = libusb_init(&contexto_usb);                                                                                                 //Iniciar una sesion de libusb

    if(r < 0) std::cout<<"Error: LIBSUB no se pudeo inciar una sesión"<<std::endl;                                                      //Si r menor que cero, mostar la siguiente advertencia                                                     //Si r menor que cero, mostar la siguiente advertencia

    libusb_set_debug(contexto_usb, 3);                                                                                                  //Esta funcion establece el nivel de debug en una sesion de libusb
    numero_de_dispositivos = libusb_get_device_list(contexto_usb,&lista_de_dispositivos);                                               //Generar una lista de dispositivos USB devolviendo el número de dispositivos
    manejador_dispositivo_usb=libusb_open_device_with_vid_pid(contexto_usb,1204,4100);                                                  //Abrir el dispositivo USB con el idvendor=1204 e idproduct=4100

    if((manejador_dispositivo_usb == NULL)||(numero_de_dispositivos==0))                                                                //Si la lista de dispositivos esta vacia o si el numero de dispositivos es cero
    {
        libusb_free_device_list(lista_de_dispositivos,1);                                                                               //Liberar la lista de dispositivos
        std::cout<<"Error: No se pudo abrir el dispositivo"<<std::endl;                                                                 //Mostar la siguiente advertencia
    }else                                                                                                                               //Sino
    {
        libusb_free_device_list(lista_de_dispositivos,1);                                                                               //Liberar la lista de dispositivos

        if(libusb_kernel_driver_active(manejador_dispositivo_usb, 0) == 1) libusb_detach_kernel_driver(manejador_dispositivo_usb,1);    //Si un modulo del kernel reclamo el dispsitivo, liberar el dispositivo del modulo del kernel

        int reclamar_usb_estado = libusb_claim_interface(manejador_dispositivo_usb, 0);                                                 //Reclamar el dispositivo y usar la interfaz cero

        if(reclamar_usb_estado<0) std::cout<<"Error: No se pudo reclarmar el dispositivo USB"<<std::endl;                               //Si reclamar_usb_estado menor que cero, mostar la siguiente advertenci
    }
}

//! Funcion para desconectar el dispositivo USB
void funcion_desconexion_usb()
{
    int liberacion_usb_estado=libusb_release_interface(manejador_dispositivo_usb, 0);               //Liberar el dispositivo usb

    if (liberacion_usb_estado!=0) std::cout<<"Error: No se pudo liberar el dispositivo"<<std::endl; //Si iberacion_usb_estado distinto de cero, motrar la siguiente advertencia

    libusb_close(manejador_dispositivo_usb);                                                        //Cerrar el manejador_dispositivo_usb
    libusb_exit(contexto_usb);                                                                      //Salir
}

//! Funcion para leer el esatdo de hotplug del dispositivo
int funcion_leer_estado_hotplug()
{
    return estado_hotplug;          //Devolver el estado de hotplug
}

//! Funcion para escribir el esatdo de hotplug del dispositivo
/*!
  \param nuevo_estado
*/
void funcion_escribir_estado_hotplug(int nuevo_estado)
{
    estado_hotplug=nuevo_estado;    //Asignar el nuevo estado de hotplug
}

//! Funcion para leer el esatdo de frecuencia
bool funcion_leer_estado_frecuencia()
{
    return estado_frecuencia;       //Devolver el estado de frecuencia

}

//! Funcion para escribir el esatdo de frecuencia
/*!
  \param nuevo_estado
*/
void funcion_escribir_estado_frecuencia(bool nuevo_estado)
{
    estado_frecuencia=nuevo_estado; //Asignar el nuevo estado de frecuencia
}

#endif // USB_H_INCLUDED
