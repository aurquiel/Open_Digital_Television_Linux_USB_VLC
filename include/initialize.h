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

#ifndef INITIALIZE_H_INCLUDED
#define INITIALIZE_H_INCLUDED

#include "usb.h"            //Incluir la cabezera usb.h
#include "thread.h"         //Incluir la cabezera thread.h
#include "samsung.h"        //Incluir la cabezera samsung.h
#include <string>           //Incluir la librería string
#include <thread>           //Incluir la librería thread

//------------------------------------CABEZERAS PROCEDMEINTOS Y FUNCIONES COMIENZO------------------------------
void procedimiento_inicializar_demodulador_sintonizador(void);            //Procedimiento para inicializar el demodulador y el sintonizador
void procedimiento_sintonizar_frecuencia(unsigned long frecuencia_Khz);   //Procedimiento para sintonizar una frecuencia en específico
void procedimiento_programa_521000_57856(void);                           //Procedimiento para sintonizar el programa 521000 Hz id 57856
void procedimiento_programa_521000_57857(void);                           //Procedimiento para sintonizar el programa 521000 Hz id 57857
void procedimiento_programa_521000_57858(void);                           //Procedimiento para sintonizar el programa 521000 Hz id 57858
void procedimiento_programa_521000_57859(void);                           //Procedimiento para sintonizar el programa 521000 Hz id 57859
void procedimiento_programa_521000_57860(void);                           //Procedimiento para sintonizar el programa 521000 Hz id 57860
void procedimiento_programa_521000_57880(void);                           //Procedimiento para sintonizar el programa 521000 Hz id 57880
void procedimiento_programa_533000_57920(void);                           //Procedimiento para sintonizar el programa 533000 Hz id 57920
void procedimiento_programa_533000_57921(void);                           //Procedimiento para sintonizar el programa 533000 Hz id 57921
void procedimiento_programa_533000_57922(void);                           //Procedimiento para sintonizar el programa 533000 Hz id 57922
void procedimiento_programa_533000_57944(void);                           //Procedimiento para sintonizar el programa 533000 Hz id 57944
void procedimiento_programa_539000_57952(void);                           //Procedimiento para sintonizar el programa 539000 Hz id 57952
void procedimiento_programa_539000_57953(void);                           //Procedimiento para sintonizar el programa 539000 Hz id 57953
void procedimiento_programa_539000_57954(void);                           //Procedimiento para sintonizar el programa 539000 Hz id 57954
void procedimiento_programa_539000_57955(void);                           //Procedimiento para sintonizar el programa 539000 Hz id 57955
void procedimiento_programa_539000_57956(void);                           //Procedimiento para sintonizar el programa 539000 Hz id 57956
void procedimiento_programa_539000_57976(void);                           //Procedimiento para sintonizar el programa 539000 Hz id 57976
void procedimiento_programa_527000_57888(void);                           //Procedimiento para sintonizar el programa 527000 Hz id 57888
void procedimiento_programa_527000_57889(void);                           //Procedimiento para sintonizar el programa 527000 Hz id 57889
void procedimiento_programa_527000_57890(void);                           //Procedimiento para sintonizar el programa 527000 Hz id 57890
void procedimiento_programa_527000_57891(void);                           //Procedimiento para sintonizar el programa 527000 Hz id 57891
void procedimiento_programa_527000_57892(void);                           //Procedimiento para sintonizar el programa 527000 Hz id 57892
void procedimiento_programa_527000_57912(void);                           //Procedimiento para sintonizar el programa 527000 Hz id 57912
char* funcion_leer_id(void);                                              //Función para leer el ID del programa
//------------------------------------CABEZERAS PROCEDMEINTOS Y FUNCIONES FIN-----------------------------------

//------------------------------------VARIABLES GLOBALES COMIENZO-----------------------------------------------
/*!
  Variable para guardar el ID del programa.
*/
std::string id = "program=00000";                             //Variable para guardar el ID del programa
//------------------------------------VARIABLES GLOBALES FIN----------------------------------------------------

//-----------------------FUNCION PARA LOS CANALES COMIENZO------------------------------------------------------

//! Procedimiento para inicializar el demodulador y el sintonizador.
/*!
  Advertencias que muestra:
  - Si no puede incializarse el dispositivo USB: "Error de Inicializacion".
*/
void procedimiento_inicializar_demodulador_sintonizador(void)
{
    unsigned short resultado = 0;                               //Variable oe control del ciclo while
    unsigned int intentos = 0;                                  //Variable para interrumpir el ciclo while

    do                                                          //Hacer
    {
        resultado = 0;                                          //Asignar 0 a la variable resultado
        procedimiento_retardo_milisegundos(80);
        resultado |= SemcoTC90527Init();                        //Llamada a la función de Samsung para inicializar el demodulador, duevleve un cero exitoso
        procedimiento_retardo_milisegundos(80);
        resultado |= SemcoSTV4100_Initialize();                 //Llamada a la función de Samsung para inicializar el sintonizador, duevleve un cero exitoso
        intentos++;                                             //Incrementa intentos
        if (intentos>100)                                      //Si incrementos es mayor a 100
        {
            std::cout<<"Error de Inicializacion"<<std::endl;    //Mostar la siguiente advertencia
            break;                                              //Sale del ciclo de inicializacion
        }

    }
    while (resultado == 1);                                     //Hacer mientras la variable resultado valga 1
}

//! Procedimiento para sintonizar una frecuencia en específico.
/*!
  \param frecuencia_Khz frecuencia en Kilohertz a sintonizar.
*/
void procedimiento_sintonizar_frecuencia(unsigned long frecuencia_Khz)
{
    for(int i=0;i<2;i++)
    {
        SemcoSTV4100_SetFrequency(frecuencia_Khz, 6);   //Llamada a la función de Samsung para colocar una frecuencia y el ancho de banda en Venezuela es 6 Megahertz.
        procedimiento_retardo_milisegundos(80);        //Retardo de of 100 milisegundos
        SemcoTC90527SoftReset();                        //Llamada a la función de Samsung para un Soft Reset del dispositivo
        procedimiento_retardo_milisegundos(80);        //Retardo de of 100 milisegundos
    }
}

//------------------------------------FUNCIONES DE LOS CANALES COMIENZO-----------------------------------------

//! Procedimiento para sintonizar el programa a la frecuencia de 521000 Hertz con el ID=57856.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_521000_57856(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(521000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57856";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 521000 Hertz con el ID=57857.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_521000_57857(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(521000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57857";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 521000 Hertz con el ID=57858.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_521000_57858(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(521000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57858";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 521000 Hertz con el ID=57859.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_521000_57859(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(521000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57859";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 521000 Hertz con el ID=57860.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_521000_57860(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(521000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57860";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 521000 Hertz con el ID=57880.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_521000_57880(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(521000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57880";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 533000 Hertz con el ID=57920.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_533000_57920(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(533000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57920";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 533000 Hertz con el ID=57921.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_533000_57921(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(533000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57921";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 533000 Hertz con el ID=57922.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_533000_57922(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(533000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57922";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 533000 Hertz con el ID=57944.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_533000_57944(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(533000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57944";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 539000 Hertz con el ID=57952.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_539000_57952(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(539000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57952";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 539000 Hertz con el ID=57953.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_539000_57953(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(539000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57953";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 539000 Hertz con el ID=57954.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_539000_57954(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(539000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57954";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 539000 Hertz con el ID=57955.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_539000_57955(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(539000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57955";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 539000 Hertz con el ID=57956.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_539000_57956(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(539000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57956";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 539000 Hertz con el ID=57976.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_539000_57976(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(539000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57976";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 527000 Hertz con el ID=57888.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_527000_57888(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(527000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57888";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 527000 Hertz con el ID=57889.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_527000_57889(void)    //For channel 527000 id 57889
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(527000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57889";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el cprograma a la frecuencia de 527000 Hertz con el ID=57890.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_527000_57890(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(527000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57890";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 527000 Hertz con el ID=57891.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_527000_57891(void)    //For channel 527000 id 57891
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(527000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57891";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 527000 Hertz con el ID=57892.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_527000_57892(void)
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(527000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57892";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//! Procedimiento para sintonizar el programa a la frecuencia de 527000 Hertz con el ID=57912.
/*!
  Advertencias que muestra:
  - Si dispositivo USB no conectado: "No inicialización del demodulador, sintonizador, frecuencia no establecida.".
  - Si dispositivo USB desconectado: "Error dispositivo desconectado.".
  - Otros caso de estado de conexión indefinido: "Error conexión del dispositivo.".
*/
void procedimiento_programa_527000_57912(void)    //For channel 527000 id 57912
{
    switch (funcion_leer_estado_hotplug()) //Estructura Switch para manejar el estado de hotplug del dispostivo
    {
        case 0:                                                     //Caso 0: El dispositivo no esta conectado
            std::cout<<"Error dispositivo no conectado: No inicialización del demodulador, sintonizador, frecuencia no establecida."<<std::endl;//Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        case 1:                                                     //Caso 1: El dispositivo esta conectado
            procedimiento_inicializar_demodulador_sintonizador();   //Llamada al procedimiento para inicializar el demodulador y el sintonizador
            procedimiento_sintonizar_frecuencia(527000);            //Llamada al procedimiento para sintonizar una frecuencia en específico
            procedimiento_escribir_estado_frecuencia(true);         //Establecer en un estado de frecuencia para indicar que se sintonizo una frecuencia
            id = "program=57912";                                   //Colocar el ID del programa
        break;                                                      //Romper Switch

        case 2:                                                     //Caso 2: El dispositivo esta desconectado
             std::cout<<"Error dispositivo desconectado."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch

        default:                                                    //Otros casos
            std::cout<<"Error conexión del dispositivo."<<std::endl; //Mostar la siguiente advertencia
        break;                                                      //Romper Switch
    }
}

//------------------------------------FUNCIONES DE LOS CANALES FIN----------------------------------------------

//! Función para obtener el ID del programa sintonizado.
/*!
  \return Cadena de caracteres con el ID del programa.
*/
char* funcion_leer_id(void)
{
    char *char_id = &id[0u];            //Convierte el string a char
    return char_id;                     //Devolver el char_id

}

#endif // INITIALIZE_H_INCLUDED
