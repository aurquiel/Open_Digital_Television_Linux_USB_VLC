#ifndef INITIALIZE_H_INCLUDED
#define INITIALIZE_H_INCLUDED

#include "usb.h"            //Incluir la cabezera usb.h
#include "thread.h"         //Incluir la cabezera thread.h
#include "samsung.h"        //Incluir la cabezera samsung.h
#include <string>           //Incluir la librería string
#include <thread>           //Incluir la librería thread

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------
void funcion_inicializar_demodulador_sintonizador();                //Función para inicializar el demodulador y el sintonizador
void funcion_sintonizar_frecuencia(unsigned long frecuencia_Khz);   //Función para sintonizar una frecuencia en específico
void funcion_canal_521000_57856();                                  //Función para sintonizar el canal 521000 Hz id 57856
void funcion_canal_521000_57857();                                  //Función para sintonizar el canal 521000 Hz id 57857
void funcion_canal_521000_57858();                                  //Función para sintonizar el canal 521000 Hz id 57858
void funcion_canal_521000_57859();                                  //Función para sintonizar el canal 521000 Hz id 57859
void funcion_canal_521000_57860();                                  //Función para sintonizar el canal 521000 Hz id 57860
void funcion_canal_521000_57880();                                  //Función para sintonizar el canal 521000 Hz id 57880
void funcion_canal_533000_57920();                                  //Función para sintonizar el canal 533000 Hz id 57920
void funcion_canal_533000_57921();                                  //Función para sintonizar el canal 533000 Hz id 57921
void funcion_canal_533000_57922();                                  //Función para sintonizar el canal 533000 Hz id 57922
void funcion_canal_533000_57944();                                  //Función para sintonizar el canal 533000 Hz id 57944
void funcion_canal_539000_57952();                                  //Función para sintonizar el canal 539000 Hz id 57952
void funcion_canal_539000_57953();                                  //Función para sintonizar el canal 539000 Hz id 57953
void funcion_canal_539000_57954();                                  //Función para sintonizar el canal 539000 Hz id 57954
void funcion_canal_539000_57955();                                  //Función para sintonizar el canal 539000 Hz id 57955
void funcion_canal_539000_57956();                                  //Función para sintonizar el canal 539000 Hz id 57956
void funcion_canal_539000_57976();                                  //Función para sintonizar el canal 539000 Hz id 57976
void funcion_canal_527000_57888();                                  //Función para sintonizar el canal 527000 Hz id 57888
void funcion_canal_527000_57889();                                  //Función para sintonizar el canal 527000 Hz id 57889
void funcion_canal_527000_57890();                                  //Función para sintonizar el canal 527000 Hz id 57890
void funcion_canal_527000_57891();                                  //Función para sintonizar el canal 527000 Hz id 57891
void funcion_canal_527000_57892();                                  //Función para sintonizar el canal 527000 Hz id 57892
void funcion_canal_527000_57912();                                  //Función para sintonizar el canal 527000 Hz id 57912
char* funcion_leer_id();                                            //Función para leer el ID del canal
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//------------------------------------VARIABLES GLOBALES GRAFICAS COMIENZO--------------------------------------
std::string id="program=00000";                             //Variable para guardar el ID del canal
//------------------------------------VARIABLES GLOBALES GRAFICAS FIN-------------------------------------------

//-----------------------FUNCION PARA LOS CANALES COMIENZO------------------------------------------------------

//! Función para inicializar el demodulador y el sintonizador.
void funcion_inicializar_demodulador_sintonizador()
{
    unsigned short resultado = 0;                               //Variable oe control del ciclo while
    unsigned int intentos=0;                                    //Variable para interrumpir el ciclo while

    do                                                          //Hacer
    {
        resultado = 0;                                          //Asignar 0 a la variable resultado
        resultado |= SemcoTC90527Init();                        //Llamada a la función de Samsung para inicializar el demodulador, duevleve un cero exitoso
        resultado |= SemcoSTV4100_Initialize();                 //Llamada a la función de Samsung para inicializar el sintonizador, duevleve un cero exitoso
        intentos++;                                             //Incrementa intentos
        if (intentos>100)                                       //ASi incrementos es mayor a 100
        {
            std::cout<<"Error de Inicializacion"<<std::endl;    //Mostar la siguiente advertencia
            break;                                              //Sale del ciclo de inicializacion
        }

    }
    while (resultado == 1);                                     //Hacer mientras la variable resultado valga 1
}

//! Función para sintonizar una frecuencia en específico.
/*!
  \param frecuencia_Khz la frecuencia en Kilohertz deseada.
*/
void funcion_sintonizar_frecuencia(unsigned long frecuencia_Khz)
{
    SemcoSTV4100_SetFrequency(frecuencia_Khz, 6);   //Llamada a la función de Samsung para colocar una frecuencia y el ancho de banda en Venezuela es 6 Megahertz.
    SemcoSleep(100);                                //Retardo de 100 milisegundos
    SemcoTC90527SoftReset();                        //Llamada a la función de Samsung para un Soft Reset del dispositivo
    SemcoSleep(100);                                //Retardo de of 100 milisegundos
}

//------------------------------------FUNCIONES DE LOS CANALES COMIENZO-----------------------------------------

//! Función para sintonizar el canal a la frecuencia de 521000 Hertz con el ID=57856.
void funcion_canal_521000_57856()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(521000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57856";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 521000 Hertz con el ID=57857.
void funcion_canal_521000_57857()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(521000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57857";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 521000 Hertz con el ID=57858.
void funcion_canal_521000_57858()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(521000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57858";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 521000 Hertz con el ID=57859.
void funcion_canal_521000_57859()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(521000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57859";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 521000 Hertz con el ID=57860.
void funcion_canal_521000_57860()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(521000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57860";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 521000 Hertz con el ID=57880.
void funcion_canal_521000_57880()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(521000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57860";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 533000 Hertz con el ID=57920.
void funcion_canal_533000_57920()    //For channel 533000 id 57920
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(533000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57920";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 533000 Hertz con el ID=57921.
void funcion_canal_533000_57921()    //For channel 533000 id 57921
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(533000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57921";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 533000 Hertz con el ID=57922.
void funcion_canal_533000_57922()    //For channel 533000 id 57922
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(533000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57922";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 533000 Hertz con el ID=57944.
void funcion_canal_533000_57944()    //For channel 533000 id 57944
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(533000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57944";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 539000 Hertz con el ID=57952.
void funcion_canal_539000_57952()    //For channel 539000 id 57952
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(539000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57952";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 539000 Hertz con el ID=57953.
void funcion_canal_539000_57953()    //For channel 539000 id 57953
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(539000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57953";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 539000 Hertz con el ID=57954.
void funcion_canal_539000_57954()    //For channel 539000 id 57954
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(539000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57954";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 539000 Hertz con el ID=57955.
void funcion_canal_539000_57955()    //For channel 539000 id 57955
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(539000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57955";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 539000 Hertz con el ID=57956.
void funcion_canal_539000_57956()    //For channel 539000 id 57956
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(539000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57956";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 539000 Hertz con el ID=57976.
void funcion_canal_539000_57976()    //For channel 539000 id 57976
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(539000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57976";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 527000 Hertz con el ID=57888.
void funcion_canal_527000_57888()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(527000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57888";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 527000 Hertz con el ID=57889.
void funcion_canal_527000_57889()    //For channel 527000 id 57889
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(527000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57889";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 527000 Hertz con el ID=57890.
void funcion_canal_527000_57890()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(527000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57890";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 527000 Hertz con el ID=57891.
void funcion_canal_527000_57891()    //For channel 527000 id 57891
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(527000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57891";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 527000 Hertz con el ID=57892.
void funcion_canal_527000_57892()
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(527000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57892";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Función para sintonizar el canal a la frecuencia de 527000 Hertz con el ID=57912.
void funcion_canal_527000_57912()    //For channel 527000 id 57912
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicilaización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            funcion_inicializar_demodulador_sintonizador();         //Llamada a la función para inicializar el demodulador y el sintonizador
            funcion_sintonizar_frecuencia(527000);                  //Llamada a la función para sintonizar una frecuencia en específico
            funcion_escribir_estado_frecuencia(true);               //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id="program=57912";                                     //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo deconectado"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo"<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//------------------------------------FUNCIONES DE LOS CANALES FIN----------------------------------------------

//! Función para obtner el ID utilizado.
char* funcion_leer_id()
{
    char *char_id = &id[0u];            //Convierte el string a char
    return char_id;                     //Devolver el char

}

#endif // INITIALIZE_H_INCLUDED
