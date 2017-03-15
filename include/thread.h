#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include "usb.h"     //Incluir la cabezera usb.h
#include <iostream>  //Incluir la librería iostream
#include <fstream>   //Incluir la librería fstream

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------
static void funcion_de_llamda_leer_transferencia_usb(struct libusb_transfer *transfer);     //Callback function to resubmit the transfer
static struct libusb_transfer *funcion_realizar_transferencia_usb(libusb_device_handle *handle,size_t length); //Function that receives the transfer and return it
void funcion_ciclo_recepcion_BTS();                                                         //loop for receive the BTS files
void funcion_reinicializar_archivo();                                                       //Eliminate and clean the file
bool funcion_leer_estado_detener();                                                         //Function to know the status of stop
void funcion_escribir_estado_detener(bool nuevo_estado);                                    //Function to change the status of stop
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//------------------------------------VARIABLES GLOBALES GRAFICAS COMIENZO--------------------------------------
std::ofstream variable_archivo_TS ("data.ts");                                                          //File to write the BTS frame
const int tamanio_buffer=1024;                                                              //Size of the buffer 240Kb 245760
unsigned char MPEG2_TS[tamanio_buffer];                                                     //Buffer to receive
int contador_evitar_basura=0;                                                                       //Global counter, this is for avoid trash in the firts transmissions
bool estado_detener=false;                                                                  //variable to know if stop button was pressed
//------------------------------------VARIABLES GLOBALES GRAFICAS FIN-------------------------------------------

//! Funcion de llamada de la funcion funcion_realizar_transferencia_usb, escribe en el archivo y llama a otra transferencia
/*!
  \param *transferencia_usb_2  struct libusb_transfer
*/
static void funcion_de_llamda_leer_transferencia_usb(struct libusb_transfer *transferencia_usb_2)
{
    int res;
    if(transferencia_usb_2->status ==LIBUSB_TRANSFER_COMPLETED)
    {
        std::cout<<"Transferencia completa"<<std::endl; //Mostar la siguiente advertencia
    }
    else
    {
        std::cout<<"Error: "<<transferencia_usb_2->status<<std::endl;   //Mostar la siguiente advertencia
    }

    contador_evitar_basura++;  //Incrementar contador_evitar_basura

    //If glbal_counter > 2 write to a file, this to avoid the trash in the first transmissions
    if (contador_evitar_basura>2)  variable_archivo_TS.write( (char *)transferencia_usb_2->buffer, tamanio_buffer);

    res = libusb_submit_transfer(transferencia_usb_2); //Realizar unanueva transferencia

    if(res !=0)
    {
        std::cout<<"submitting. error code: "<<res<<std::endl;  //Mostar la siguiente advertencia
    }
}

//! Funcion crear transferencia
/*!
  \param *manejador_dispositivo_usb
  \param tamanio_buffer

*/
static struct libusb_transfer *funcion_realizar_transferencia_usb (libusb_device_handle *manejador_dispositivo_usb,size_t tamanio_buffer)
{
    struct libusb_transfer *transferencia_usb_2;                  //transfer structure

    transferencia_usb_2 = libusb_alloc_transfer(0);     //Allocate the transfer for non isochronous set zero

    libusb_fill_interrupt_transfer(transferencia_usb_2, //Transfer structure
                  manejador_dispositivo_usb, //deveice handle
                                       0x82, //Endpoint
                                   MPEG2_TS, //Buffer
                             tamanio_buffer, //length of buffer
   funcion_de_llamda_leer_transferencia_usb, //callback function
                                       NULL, //user NULL
                                          0  //Waiting time
                                          );


    return transferencia_usb_2;             //Devuelve la transferencia
}

void funcion_ciclo_recepcion_BTS()
{
    contador_evitar_basura=0;   //Reset to zero for a new transmission
    struct libusb_transfer *transferencia_usb;

    if (funcion_leer_estado_hotplug()>0)
    {
        for(int i=0;i<1000;i++)   //Queue 68 transfers
        {
            transferencia_usb = funcion_realizar_transferencia_usb(manejador_dispositivo_usb, tamanio_buffer); //Function that fills and returns the transfer structure, receive the deveice handle and the buffer size
            libusb_submit_transfer(transferencia_usb); //submit the transfer
        }

        int res;
        /* Handle Events */
        while(1)
        {
            res =libusb_handle_events(contexto_usb);

            if(res <0)
            {
                std::cout<<"handle_events()error: "<<res<<std::endl;
            }

            if (funcion_leer_estado_hotplug()>1)
            {
                libusb_free_transfer(transferencia_usb);  //free the transfer
                funcion_reinicializar_archivo();  //functio to eleminate and clean
                funcion_escribir_estado_hotplug(0); //change hotplug status to zero
                 //create a empty one
                std::cout<<"El dispsotivo fue desconectado"<<std::endl;
                break;
            }
        }

    }else {
        std::cout<<"El dispositivo no esta conectado: No se puedo incializar el proceso para recibir los paquetes BTS"<<std::endl;
    }

}

void funcion_reinicializar_archivo()
{
    variable_archivo_TS.seekp(variable_archivo_TS.beg); //Colocarse al principio del archivo
    std::ofstream variable_archivo_TS("data.ts");       //Reescribir el archivo
    contador_evitar_basura=0;                           //Reinicializa en cero el contador_evitar_basura en las primeras transmisiones
}

//Function to know the status of stop
bool funcion_leer_estado_detener()
{
    return estado_detener;                              //Devolver el estado de detener
}

//Function to change the status of stop
void funcion_escribir_estado_detener(bool nuevo_estado)
{
    estado_detener=nuevo_estado;                        //Asignar el nuevo estado de detener
}

#endif // THREAD_H_INCLUDED
