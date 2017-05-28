#ifndef SAMSUNG_H_INCLUDED
#define SAMSUNG_H_INCLUDED

#include "usb.h"        //Incluir la cabezera usb.h
#include <cmath>        //Incluir la librería cmath
#include <unistd.h>     //Incluir la librería unistd.h

#define OK		0

///Estructura para contener los datos del demodulador: estado de bloqueo, relación señal ruido, pre_BER, post_BER, mode_90527, code_rate, hab_error.
typedef struct _TC90527Data{
   unsigned char bLock;
   double SNR;
   float  pre_BER;
   float  post_BER;
   int    mode_90527;
   int    code_rate;
   unsigned int  hab_error;
}TC90527Data,
///Apuntador a estructura TC90527Data
*lpTC90527Data;

///Estrutura del tipo TC90527Data
TC90527Data datos_demulador;
///Apuntador a estrucutra del tipo TC90527Data
lpTC90527Data apuntador_datos_demulador=&datos_demulador;

//------------------------------------CABEZERAS FUNCIONES COMIENZO----------------------------------------------

// Initialize the TC90527
unsigned short SemcoTC90527Init();
// Soft Reset for TC90527
unsigned short SemcoTC90527SoftReset();
// Register Reset
unsigned short SemcoTC90527RegReset();
// Sleep On
unsigned short SemcoTC90527SleepOn();
// Sleep Off
unsigned short SemcoTC90527SleepOff();
// Check master lock
// return value : 1-lock, 0-unlock
unsigned short SemcoTC90527MasterLock();
// ISDB-T SNR
double SemcoTC90527_ISDBT_SNRResultCheck();
// ISDB-T Signal Quality
unsigned short  SemcoTC90527_GetSignalQuality(lpTC90527Data lpTD);
// Set TS Output
// nTS : 0-serial, 1-parallel
unsigned short  SemcoTC90527_Set_TS_Output(unsigned char nTS);
// Get Test Data
unsigned short  SemcoTC90527_GetDatas(lpTC90527Data lpTD);
/* Get IF AGC Level */
int SemcoTC90527_GetIfAgcLevel();
/* Get SSI */
int SemcoTC90527_GetSSI();
/* Initialize the STV4100 */
unsigned char SemcoSTV4100_Initialize();
/* Set Frequency */
unsigned char SemcoSTV4100_SetFrequency(unsigned long nFrequency_KHz, unsigned char BW);
/* Check lock status */
/* return : 1-pll lock, 0-pll unlock */
unsigned char SemcoSTV4100_GetLockStatus();
/* Wake up */
unsigned char SemcoSTV4100_On();
/* Sleep */
unsigned char SemcoSTV4100_StandBy();
/* STV4110 IIC Write */
unsigned char STV4100_I2C_Write(unsigned char Addr, unsigned char Data);
/* STV4110 IIC Read */
unsigned char STV4100_I2C_Read(unsigned char Addr, unsigned char *lpData);
/* TC90527 IIC Write */
unsigned char TC90527_I2cWrite(unsigned char Addr,unsigned char Data);
/* TC90527 IIC Read  */
unsigned char TC90527_I2cRead (unsigned char Addr);
//Funciono para combinar dos bytes
static unsigned short funcion_combinar_bytes(unsigned char b1, unsigned char b2);
//Porcedimeinto de retardo en milisegundos
void procedimiento_retardo_milisegundos(unsigned int tiempo_ms);
//------------------------------------CABEZERAS FUNCIONES FIN---------------------------------------------------

//-----------------------FUNCIONES DE SAMSUNG PARA EL DEMODULADOR INICIO----------------------------------------

//! Función para incializar el demodulador.
/*!
  \return result 0:Exitosa 1:Fallida.
*/
unsigned short SemcoTC90527Init()
{
	unsigned short result = OK ;

	// original
	// No. 0x01 :
	result |= TC90527_I2cWrite(0x01, 0x40); // reset
	result |= TC90527_I2cWrite(0x0D, 0x00); // output enable
	result |= TC90527_I2cWrite(0x0F, 0x01); // Set the parallel TS 20110214 by yooheeyong

	result |= TC90527_I2cWrite(0x11, 0x2D); // for TC90527 : Divider ratio of VCO
	result |= TC90527_I2cWrite(0x12, 0x02); // for TC90527 : Divider ratio of Ref. clock

	result |= TC90527_I2cWrite(0x13, 0x62); // carrier freq. offset
	result |= TC90527_I2cWrite(0x14, 0x60); // carrier freq. offset
	result |= TC90527_I2cWrite(0x15, 0x00); // carrier freq. offset

	result |= TC90527_I2cWrite(0x16, 0x04); // IQ Baseband mode[2]  : Zero IF Selection

	result |= TC90527_I2cWrite(0x1C, 0x20); // output enable(except RFAGC)
	result |= TC90527_I2cWrite(0x1D, 0x00); // output enable
	result |= TC90527_I2cWrite(0x1E, 0x00); // output enable
	result |= TC90527_I2cWrite(0x1F, 0x00); // output enable

	result |= TC90527_I2cWrite(0x20, 0x00); // ifagc mode
	result |= TC90527_I2cWrite(0x22, 0x80); // ifagc mode
	result |= TC90527_I2cWrite(0x23, 0x4C); // depend on tuner
	result |= TC90527_I2cWrite(0x30, 0x28); // for TC90527 : Spectrum polarity inversion [3]


	result |= TC90527_I2cWrite(0x31, 0x00); // zero(0MHz) IF
	result |= TC90527_I2cWrite(0x32, 0x00); // zero (0MHz) IF
	result |= TC90527_I2cWrite(0x34, 0x0F); // for TC90527 : Digital Filter mode(90M, BW 6M), IF_AGC(x1), RF_AGC(x1)
	result |= TC90527_I2cWrite(0x38, 0x01); // for TC90527 : AFC
	result |= TC90527_I2cWrite(0x39, 0x1C); // for TC90527 : AFC

	result |= TC90527_I2cWrite(0x3B, 0x00); // for TC90527 : symds_off[11:8]
	result |= TC90527_I2cWrite(0x3C, 0x00); // for TC90527 : symds_off[7:0]
	result |= TC90527_I2cWrite(0x47, 0x10); // for TC90527 : mdsmd[4]    ==> 1 0x00010000

	result |= TC90527_I2cWrite(0x73, 0x40); // for TC90527 : pvita : 1, psela : 0

	result |= TC90527_I2cWrite(0xD0, 0x1E); // for TC90527 : ADC Ref. level set up 1
	result |= TC90527_I2cWrite(0xD1, 0x0B); // for TC90527 : ADC Ref. level set up 2
	result |= TC90527_I2cWrite(0xEF, 0x01); // recommendation
	result |= TC90527_I2cWrite(0xF9, 0x00); // fbstoff set '0'

	// for SAT
	//-----------------------------------------------
	result |= TC90527_I2cWrite(0x50, 0x00); // recommend by RyuSH for SAT 20110314
	result |= TC90527_I2cWrite(0xF1, 0x80); // recommend by RyuSH for SAT 20110314
	result |= TC90527_I2cWrite(0x2B, 0x20); // recommend by RyuSH for SAT 20110314
	//-----------------------------------------------


	return result;
}


//! Función para realizar un reset suave al demodulador.
/*!
  \return result 0:Exitosa 1:Fallida.
*/
unsigned short SemcoTC90527SoftReset()
{
	unsigned short  result = OK;

	result |= TC90527_I2cWrite(0x01,0x40);

	return result;
}


//! Función para realizar un reset de los registros del demodulador.
/*!
  \return result 0:Exitosa 1:Fallida.
*/
unsigned short SemcoTC90527RegReset()
{
	unsigned short result = OK;

	result |= TC90527_I2cWrite(0x01,0x80);

	return result;
}

//! Función para entrar en el modo de sueño del demodulador.
/*!
  \return result 0:Exitosa 1:Fallida.
*/
unsigned short SemcoTC90527SleepOn()
{
	unsigned short result = OK;

	result |= TC90527_I2cWrite(0x03,0x80);

	return result;
}

//! Función para salir del modo de sueño del demodulador.
/*!
  \return result 0:Exitosa 1:Fallida.
*/
unsigned short SemcoTC90527SleepOff()
{
	unsigned short result = OK;

	result |= TC90527_I2cWrite(0x03,0x00);

	SemcoTC90527SoftReset();

	return result;
}

///! Función para saber el estado del master lock.
/*!
  \return result  1-bloqueado, 0-desbloqueado.
*/
unsigned short SemcoTC90527MasterLock()
{
	unsigned char value;
	unsigned short result;

	value = TC90527_I2cRead(0x96); // change lock register from 0x80 to 0x96 by yooheeyong 20091110

	// add if statement for lock register from 0x80 to 0x96 by yooheeyong 20091110
	if((value&0x80) || (value&0x40) || (value&0x20)){ // rlocka[7], rlockb[6], rlockc[5]
        result = 1;
	}else{
		result = 0;
	}

	return (result);
}

//! Función ISDB-T SNR pa obtener la relación señal/ruido.
/*!
  \return dSn relación señal ruido.
*/
double SemcoTC90527_ISDBT_SNRResultCheck()
{
	double dSnWork;
	double dSn;
	unsigned long ulData=0;
	unsigned char ucTemp;

	//double a,b,c,d,e,f;

  	ucTemp = TC90527_I2cRead(0x8B);

	if(ucTemp < 0)
	{
		return -1;
	}
	else
	{
		ulData = ucTemp;
	}

	ucTemp = TC90527_I2cRead(0x8C);

	if(ucTemp < 0)
	{
		return -1;
	}
	else
	{
		ulData <<= 8;
		ulData |= ucTemp;
	}

	ucTemp = TC90527_I2cRead(0x8D);

	if(ucTemp < 0)
	{
		return -1;
	}
	else
	{
		ulData <<= 8;
		ulData |= ucTemp;
	}

 	 if(ulData == 0.0)
	{
		dSn = 0;
	}

	else
	{
		dSnWork = 10.0*log10(5505024.0 / (double)ulData);
	 	dSn = (0.000024 * pow(dSnWork, 4.0)) - (0.0016 * pow(dSnWork, 3.0)) + (0.0398 * pow(dSnWork, 2.0)) + (0.5491 * dSnWork) + 3.0965;
	}

	return dSn;
}

//! Función para determinar el tipo la salida de los paquetes BTS.
/*!
  \param *nTS 0:serial, 1:paralelo.
  \return operación 0:Exitosa 1:Fallida.
*/
unsigned short  SemcoTC90527_Set_TS_Output(unsigned char nTS)
{
    unsigned char  temp;
    unsigned short result = OK;

	if(nTS == 0)
	{
	   temp=TC90527_I2cRead(0x0F);
	   temp &= 0xFC;
	   TC90527_I2cWrite(0x0F, temp);
	}else{
	   temp=TC90527_I2cRead(0x0F);
	   temp |= 0x02;
	   TC90527_I2cWrite(0x0F, temp);
	}

	return result;
}

//! Función para obtener los datos: estado de bloqueo, relación señal ruido, pre_BER, post_BER, mode_90527, code_rate, hab_error.
/*!
  \param lpTC90527Data estructura que aloja los datos del demodulador.
  \return result 0:Exitosa 1:Fallida.
*/
unsigned short  SemcoTC90527_GetDatas(lpTC90527Data lpTD)
{
	unsigned short result;
	//int temp1,temp2,temp3;
	int temp4, temp5, temp6, temp7, temp8,temp9,temp10, perra, pecya;

	unsigned char tempread, tempwrite, a_seg, fecbst, vberorg, perra_2, perra_1, perra_0, pecya_1, pecya_0;

	result = OK;

	lpTD->bLock = SemcoTC90527MasterLock();

	lpTD->SNR=SemcoTC90527_ISDBT_SNRResultCheck();

	lpTD->mode_90527 = TC90527_I2cRead(0xb3);
	lpTD->mode_90527 = lpTD->mode_90527 & 0xe0;
	//KS_data1.modulation_scheme;

	lpTD->code_rate = TC90527_I2cRead(0xb3);
	lpTD->code_rate = lpTD->code_rate & 0x1c;
	//KS_data1.coding_rate;


	//Selecting the measurement cycle mode   - Packet Mode Select
	tempwrite = 0x00 & 0xFD; //0Àž·Î ŒŒÆÃ
	TC90527_I2cWrite(0x77, tempwrite);

	//Setting the measurement cycle
	tempwrite = 0x02 | 0x60;
	TC90527_I2cWrite(0x76, tempwrite);

	//Selecting the BER measurement layer   - Layer A Select
	tempwrite = 0x00;    //0·Î ŒŒÆÃ
	TC90527_I2cWrite(0x71, tempwrite);

	TC90527_I2cWrite(0x73, 0x40); // for TC90527 : pvita : 1, psela : 0


	 //read a_seg[3:0] - B4
	 a_seg = ((TC90527_I2cRead(0xb4)) & 0x78 ) >> 3;

	 //read fecbst - 0xFB
	 fecbst = ((TC90527_I2cRead(0xFB) ) & 0x10) >> 4;

	if(a_seg == 1)
	 {
		   vberorg = 0x02;
		  tempwrite = vberorg & 0x7F; //vberorg Set '0'
		  tempwrite = vberorg | 0x40; //vberbst Set '1'
		  TC90527_I2cWrite(0xF8, tempwrite);
	 }
	 else
	 {
		  if(fecbst == 1)  //fecbst = 1
		  {
			    vberorg = 0x02;
			   tempwrite = vberorg | 0xC0; //vberorg set '1', vberbst Set '1'
			   TC90527_I2cWrite(0xF8, tempwrite);
		  }
		  else    //fecbst = 0
		  {
			   vberorg = 0x02;
			   tempwrite = vberorg & 0x7F;
			   tempwrite = vberorg | 0x40; //vberbst Set '1'
			   TC90527_I2cWrite(0xF8, tempwrite);
		  }
	 }

	perra_2 = TC90527_I2cRead(0x9D);
	perra_1 = TC90527_I2cRead(0x9E);
	perra_0 = TC90527_I2cRead(0x9f);

	pecya_1 = TC90527_I2cRead(0xa6);
	pecya_0 = TC90527_I2cRead(0xa7);

	perra = (perra_2<<16)|(perra_1<<8)|perra_0;  //perra
	pecya = (pecya_1<<8)|pecya_0;   //pecya

	lpTD->pre_BER =  ((float)perra/(float)(pecya*204*8));

	TC90527_I2cWrite(0x73, 0x00); // for TC90527 : pvita : 0, psela : 0

	//Selecting the measurement cycle mode   - Packet Mode Select
	tempwrite = 0x00 & 0xFD; //0Àž·Î ŒŒÆÃ
	TC90527_I2cWrite(0x77, tempwrite);

	//Setting the measurement cycle
	tempwrite = 0x02 | 0x60;
	TC90527_I2cWrite(0x76, tempwrite);

	//Selecting the BER measurement layer   - Layer A Select
	tempwrite = 0x00;    //0·Î ŒŒÆÃ
	TC90527_I2cWrite(0x71, tempwrite);


	perra_2 = TC90527_I2cRead(0x9D);
	perra_1 = TC90527_I2cRead(0x9E);
	perra_0 = TC90527_I2cRead(0x9f);

	pecya_1 = TC90527_I2cRead(0xa6);
	pecya_0 = TC90527_I2cRead(0xa7);

	perra = (perra_2<<16)|(perra_1<<8)|perra_0;  //perra
	pecya = (pecya_1<<8)|pecya_0;   //pecya

	lpTD->post_BER = ( (float)(perra*8*0.5*9) / (float)(pecya*204*8) );

	if(lpTD->post_BER >= 0.0004)  //packet Áß¿¡ ±úÁø Bit°¡ ÀÖÀœ.  - 4 X 10-4
	{
		lpTD->hab_error = lpTD->hab_error + 1;
	}

	return result;
}

//! Función para obtener la calidad de la señal.
/*!
  \return Escala de la calidad de la señal: 1, 2, 3 , 4 , 5, 6, 7, 8, 9, 10. Mayor escala mejor calidad de la señal.
  \return 0 en caso de que el estado de bloqueo sea cero.
*/
unsigned short  SemcoTC90527_GetSignalQuality(lpTC90527Data lpTD)
{
	unsigned char lock_state = 0;

	lock_state = SemcoTC90527MasterLock();  //locking check!!

	if(lock_state == 0)   //unlock
	{
		return 0;
	}
	else   // lock
	{
		lpTD->SNR=SemcoTC90527_ISDBT_SNRResultCheck();

		if(lpTD->SNR < 21.5)
		{
			return 1;
		}
		else if((lpTD->SNR >= 21.5)&&(lpTD->SNR < 22.7))
		{
			return 2;
		}
		else if((lpTD->SNR >= 22.7)&&(lpTD->SNR < 23.9))
		{
			return 3;
		}
		else if((lpTD->SNR >= 23.9)&&(lpTD->SNR < 25.1))
		{
			return 4;
		}
		else if((lpTD->SNR >= 25.1)&&(lpTD->SNR < 26.3))
		{
			return 5;
		}
		else if((lpTD->SNR >= 26.3)&&(lpTD->SNR < 27.5))
		{
			return 6;
		}
		else if((lpTD->SNR >= 27.5)&&(lpTD->SNR < 28.7))
		{
			return 7;
		}
		else if((lpTD->SNR >= 28.7)&&(lpTD->SNR < 29.9))
		{
			return 8;
		}
		else if((lpTD->SNR >= 29.9)&&(lpTD->SNR < 31.1))
		{
			return 9;
		}
		else if(lpTD->SNR >= 31.1)
		{
			return 10;
		}
	}
	return 0;
}

//! Función obtener el nivel de la intensidad de la señal.
/*!
  \return nLevel
*/
int SemcoTC90527_GetIfAgcLevel()
{
	int nLevel;

	nLevel = TC90527_I2cRead(0x82);  /* ifagc_dt(0x82), follow the SB format */

	return nLevel;
}


//! Función obtener la intensidad de la señal.
/*!
  \return devuelve 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 dependiento del nivel de intensidad de la señal, mayor intensiadad mayor el nivel retornado.
  \return 0 en caso de que el estado de bloqueo sea cero.
*/
int SemcoTC90527_GetSSI()
{
	int nLevel;
	int Sig_strength;

    nLevel = SemcoTC90527_GetIfAgcLevel();

	if(SemcoTC90527MasterLock() == 1){

		if(nLevel < 106)
			Sig_strength = 10;
		else if(nLevel >= 106 && nLevel < 112)
			Sig_strength = 9;
		else if(nLevel >= 112 && nLevel < 120)
			Sig_strength = 8;
		else if(nLevel >= 120 && nLevel < 124)
			Sig_strength = 7;
		else if(nLevel >= 124 && nLevel < 128)
			Sig_strength = 6;
		else if(nLevel >= 128 && nLevel < 134)
			Sig_strength = 5;
		else if(nLevel >= 134 && nLevel < 141)
			Sig_strength = 4;
		else if(nLevel >= 141 && nLevel < 153)
			Sig_strength = 3;
		else if(nLevel >= 153 && nLevel < 169)
			Sig_strength = 2;
		else
			Sig_strength = 1;

	}else{
		Sig_strength = 0;
	}

	return Sig_strength;
}

//-----------------------FUNCIONES DE SAMSUNG PARA EL DEMODULADOR FIN-------------------------------------------

//-----------------------FUNCIONES DE SAMSUNG PARA EL SINTONIZADOR INICIO---------------------------------------

///Arreglo de registros del sintonizador
static unsigned char STV4100_Reg[15];

//! Función para inicializar el sintonizador.
/*!
  \return operacion 0:Exitosa 1:Fallida.
*/
unsigned char SemcoSTV4100_Initialize()
{
   unsigned char ret;
   unsigned char uBuf;
   int i;

   ret = 0;

   // XTal = 16MHz
   STV4100_Reg[0] = 0x01; //{ RSTV4100_ID,		0x00},
   STV4100_Reg[1] = 0xd0; //{ RSTV4100_STATUS,  0x40},
   STV4100_Reg[2] = 0x00; //{ RSTV4100_PLL1,	0x97},
   STV4100_Reg[3] = 0x34; //{ RSTV4100_PLL2,    0x7a},
   STV4100_Reg[4] = 0x00; //{ RSTV4100_PLL3,	0x4f},
   STV4100_Reg[5] = 0x00; //{ RSTV4100_PLL4,	0xB8},
   STV4100_Reg[6] = 0x8; //{ RSTV4100_PLL5,	    0x22},
   STV4100_Reg[7] = 0xad; //{ RSTV4100_VCO,		0x9F},
   STV4100_Reg[8] = 0x83; //{ RSTV4100_LNA1,	0x83},
   STV4100_Reg[9] = 0xa4; //{ RSTV4100_LNA2,	0xA4},
   STV4100_Reg[10] = 0x1c; //{ RSTV4100_CAL1,	0x13},
   STV4100_Reg[11] = 0x19; //{ RSTV4100_CAL2,	0x50},
   STV4100_Reg[12] = 0x0f; //{ RSTV4100_CAL3,	0x0F},
   STV4100_Reg[13] = 0x00; //{ RSTV4100_CONFIG,	0x00},
   STV4100_Reg[14] = 0x01; //{ RSTV4100_TST,	0x01}

   for(i=1; i<15; i++)
   {
	   ret |= STV4100_I2C_Write(i,STV4100_Reg[i]);
   }

   /*4100 power up done once (or done by config file)*/
   ret |= STV4100_I2C_Read(0x06, &uBuf);

   if(uBuf&0x80)
   {
  	  STV4100_Reg[3]&=~0x30;
	    STV4100_Reg[3]|=0x20;
	    ret |=STV4100_I2C_Write(0x03,STV4100_Reg[0x03]);

	    STV4100_Reg[6]&=0x1f;
	    STV4100_Reg[6]|=0x20;
	    ret |= STV4100_I2C_Write(0x06,STV4100_Reg[0x06]);

	    procedimiento_retardo_milisegundos(20); /*startup time*/
	 }

   return ret;
}

//! Función para sintonizar una freuencia deseada.
/*!
  \param nFrequency_KHz frecuencia en kilohertz.
  \param BW Ancho de banda.
  \return operacion 0:Exitosa 1:Fallida.

  Nota para Venezuela el ancho de banda es seis Megahertz, BW=6.
*/
unsigned char SemcoSTV4100_SetFrequency(unsigned long nFrequency_KHz, unsigned char BW)
{
	unsigned char ret;
	int i;
	unsigned char uBuf,CalValue,CLPF_CalValue;
  unsigned long InternalClock;
	unsigned long STV4100_Fxtal,result;
	unsigned char cpma;
	unsigned long N = 0, F = 0;

	STV4100_Fxtal = 16000; // XTal = 16 MHz
	InternalClock = 0;
	result = 0;
	cpma = 0;

	ret = 0;

	/*cal BB filter each time */
	ret |= STV4100_I2C_Read(0x01, &uBuf);
	uBuf |= 0x04;
	ret |= STV4100_I2C_Write(0x01, uBuf);

	procedimiento_retardo_milisegundos(1);

	ret |= STV4100_I2C_Write(0x01,STV4100_Reg[1]);

	ret |= STV4100_I2C_Read(0x0C, &uBuf);

	uBuf &= ~0x08;
	ret |= STV4100_I2C_Write(0x0C,uBuf);
	procedimiento_retardo_milisegundos(1);
	ret |= STV4100_I2C_Write(0x0C,STV4100_Reg[12]);

	/*store cpma value each frequency change*/
	ret |= STV4100_I2C_Read(0x0C, &uBuf);
	CalValue=uBuf&0x07;

	ret |= STV4100_I2C_Read(0x0B, &uBuf);
	uBuf&=~0x20;
	ret |= STV4100_I2C_Write(0x0B, uBuf);

	procedimiento_retardo_milisegundos(1);
	ret |= STV4100_I2C_Write(0x0B,STV4100_Reg[11]);

	/*store cpma value each frequency change*/
	ret |= STV4100_I2C_Read(0x0B, &uBuf);
	CLPF_CalValue=uBuf&0x1F;

	ret |= STV4100_I2C_Write(0x0b,STV4100_Reg[11]);

 if(BW==8){
		STV4100_Reg[11]|=0x40;
	}
	else if((BW==7)||(BW==6)){
		STV4100_Reg[11] &= ~0x40;
	}

  STV4100_Reg[11]&=~0x40;
	STV4100_Reg[8]&=~0x80;
	STV4100_Reg[3]&=~0xc0;
	STV4100_Reg[6]&=~0x0c;

	if(nFrequency_KHz<=325000)
	{
		result=nFrequency_KHz *16;
		STV4100_Reg[8]&=~0x80;
		STV4100_Reg[3]|=0x80;
		STV4100_Reg[6]|=0x04;
	}
	else if(nFrequency_KHz<=650000)
	{
		result=nFrequency_KHz *8 ;
		STV4100_Reg[8]|=0x80;
		STV4100_Reg[3]|=0x40;
		STV4100_Reg[6]|=0x08;
	}
	else if(nFrequency_KHz<=875000)
	{
		result= nFrequency_KHz *4 ;
		STV4100_Reg[8]|=0x80;
		STV4100_Reg[3]&=~0xc0;
		STV4100_Reg[6]|=0x08;
	}

    /* fill  CP current*/
	STV4100_Reg[3]&= 0xF0;

	if ((int)(result/1000)<2700)
		STV4100_Reg[3]|=0;
	else if ((int)(result/1000)<2950)
		STV4100_Reg[3]|=1<<1;
	else if ((int)(result/1000)<3300)
		STV4100_Reg[3]|=2<<1;
	else if ((int)(result/1000)<3700)
		STV4100_Reg[3]|=3<<1;
	else if ((int)(result/1000)<4200)
		STV4100_Reg[3]|=5<<1;
	else if ((int)(result/1000)<4800)
		STV4100_Reg[3]|=6<<1;
	else /*if ((int)(result/1000)<5200)*/
		STV4100_Reg[3]|=7<<1;

 	/* set internal clock*/
	if (STV4100_Fxtal>25000)
	{
		InternalClock= STV4100_Fxtal/2;
	}
	else
	{
		InternalClock=STV4100_Fxtal;
	}

	/*write N value*/
	N= (result/InternalClock);
	STV4100_Reg[2]=N&0xff;
	STV4100_Reg[3]&=~0x01;
	STV4100_Reg[3]|=(N>>8)&0x01;

	if (result<= 4294967 )
	{
		result=(unsigned long)((1000*result/InternalClock));
		F= (result-1000*N)*(262144-1)/1000;
	}
	else
	{
		result=(unsigned long)((100*result/InternalClock));
		F= (result-100*N)*(262144-1)/100;
	}

	/*write F value*/
	STV4100_Reg[4]=F&0xFF;
	STV4100_Reg[5]=(F>>8)&0xFF;
	STV4100_Reg[6]&=~0x03;
	STV4100_Reg[6]|=(F>>16)&0x3;

    /* don't use sigma delta if not needed*/
	if(F==0)
		 STV4100_Reg[6]&=~0x20;
	else
		 STV4100_Reg[6]|=0x20;

	/* calibrate vco for each frequency change*/
	STV4100_Reg[6]|=0x10;

	/*cpma compensation*/
	if(CalValue==0)
		cpma=1;
	else if(CalValue<4)
	{
		if(BW==8)
			cpma=2*CalValue;
		else
			cpma=(2*CalValue)+1;
	}
	else
		cpma=7;

	STV4100_Reg[0x0c]&=~0x07;
	STV4100_Reg[0x0c]|=cpma;
	STV4100_Reg[0x0b]|=0x80;
	STV4100_Reg[0x0c]|=0x08;

	/*set RFAGC to def value*/

    /******write 14 rw registers********/
	for(i=0x01;i<15;i++)
		ret |= STV4100_I2C_Write(i,STV4100_Reg[i]);

	/*check tuner lock but does not affect error*/
	i=0;
	do{
		ret |= STV4100_I2C_Read(0x01, &uBuf);
		if(uBuf&0x40)
			break;
		procedimiento_retardo_milisegundos(10);
		i++;
	}while(i<6);

	STV4100_Reg[11]&=~0x1F;

    //ning 2010.3.9
	CLPF_CalValue=31;
	STV4100_Reg[11]|=CLPF_CalValue;//31 ==> 24
	STV4100_Reg[11]|=0x20;
	ret |= STV4100_I2C_Write(0x0B,STV4100_Reg[11]);

	STV4100_Reg[9]&=~0x07;
	STV4100_Reg[9]|=0x01;
	ret |= STV4100_I2C_Write(0x09,STV4100_Reg[9]);

  return ret;
}

//! Función para saber el estado de bloqueo del sintonizador.
/*!
  \return 1-pll bloqueado, 0-pll desbloqueado.
*/
unsigned char SemcoSTV4100_GetLockStatus()
{
   unsigned char uBuf;

   uBuf = 0x00;

   STV4100_I2C_Read(0x01, &uBuf);

   return((uBuf&0x40)>>6);
}

//! Función para salir del modo de sueño del sintonizador.
/*!
  \return operación 0:Exitosa 1:Fallida.
*/
unsigned char SemcoSTV4100_On()
{
   return STV4100_I2C_Write(0x06, 0x20);
}

//! Función para entrar en modo de sueño del sintonizador.
/*!
  \return operación 0:Exitosa 1:Fallida.
*/
unsigned char SemcoSTV4100_StandBy()
{
   return STV4100_I2C_Write(0x06, 0xc0);
}

//-----------------------FUNCIONES DE SAMSUNG PARA EL SINTONIZADOR FIN------------------------------------------

//-----------------------FUNCION PARA ESCRITURA TRANSFERENCIA DE CONTROL AL DEMODULADOR INICIO------------------

//! Función transferencia USB de Control para escribir al Demodulador usando el bus I2C.
/*!
  \param Addr Direccion del registro.
  \param Data Dato a escribir.
  \return Regresa el estado de la transferencia de control 0:Exitosa 1:Fallida.
*/
unsigned char TC90527_I2cWrite(unsigned char Addr,unsigned char Data)
{
	unsigned short wValue=funcion_combinar_bytes(Data, Addr);         		//Crea un bloque de 2 bytes con dos bloques de 1 byte
    static unsigned char buffer[1]={0};                                            //Buffer para alojar recibir la respuesta de la transferencia

    //Función de la libreia LIBUSB para realizar la transferencia de control
    unsigned int respuesta = libusb_control_transfer(manejador_dispositivo_usb,     //Variable para manejar el dispsositvo usb
                                                                          0x40,     //bmRequestType SETPTUDATA[0] leer=0xC0 escribir=0x40
                                                                          0xA2,     //bRequest SETPTUDATA[1] en el firmware para hacer el caso TC90527_I2cWrite_US
                                                                        wValue,     //wValue SETPTUDATA[2 y 3] el valor a pasar
                                                                             0,     //wIndex SETPTUDATA[4 y 5], indica la dirección si es desde el afrintión al dispositivo, el numero de EndPoint.
                                                                        buffer,     //Recive un apuntador a los datos a leer o escribir
                                                                             1,     //wLength SETPTUDATA[6 y 7], numero de bytes a escribir o leer
                                                                             20);    //Tiempo para esperar la respuesta

    int resultado=buffer[0];                                                        //Asignar a resultado el valor del buffer
    if ((resultado!=0)||(respuesta!=1))                                             //Si resultado es distinto de cero o respuesta es distinto de uno
        std::cout<<"Error en escritura transferencia de control al demodulador TC90527_I2cWrite_USB"<<std::endl;//Mostrar la siguiente advertencia

    return buffer[0];   //Regresa el estado de la transferencia de control 0:Exitosa 1:Fallida
}
//-----------------------FUNCION PARA ESCRITURA TRANSFERENCIA DE CONTROL AL DEMODULADOR FIN---------------------

//-----------------------FUNCION PARA LECTURA TRANSFERENCIA DE CONTROL AL DEMODULADOR INICIO--------------------

//! Función transferencia USB de Control para leer del Demodulador usando el bus I2C.
/*!
  \param Addr Direccion del registro.
  \return Regresa el dato leido.
*/
unsigned char TC90527_I2cRead (unsigned char Addr)
{
	static unsigned char buffer[2]={0,0};                                          //Buffer para alojar recibir la respuesta de la transferencia

    //Función de la libreia LIBUSB para realizar la transferencia de control
    unsigned int respuesta = libusb_control_transfer(manejador_dispositivo_usb,     //Variable para manejar el dispsositvo usb
                                                                          0xC0,     //bmRequestType SETPTUDATA[0] leer=0xC0 escribir=0x40
                                                                          0xA3,     //bRequest SETPTUDATA[1] en el firmware para hacer el caso TC90527_I2cWrite_US
                                                                          Addr,     //wValue SETPTUDATA[2 y 3] el valor a pasar
                                                                             0,     //wIndex SETPTUDATA[4 y 5], indica la dirección si es desde el afrintión al dispositivo, el numero de EndPoint.
                                                                        buffer,     //Recive un apuntador a los datos a leer o escribir
                                                                             2,     //wLength SETPTUDATA[6 y 7], numero de bytes a escribir o leer
                                                                             20);    //Tiempo para esperar la respuesta


    int resultado=buffer[0];                                                        //Asignar a resultado el valor del buffer[0]
    if ((resultado!=0)||(respuesta!=2))                                             //Si resultado es distinto de cero o respuesta es distinto de dos
        std::cout<<"Error en escritura transferencia de control al demodulador TC90527_I2c_Write_USB"<<std::endl;//Mostrar la siguiente advertencia

    return buffer[1];   //Regresa el valor de la variable leida
}
//-----------------------FUNCION PARA LECTURA TRANSFERENCIA DE CONTROL AL DEMODULADOR FIN-----------------------

//-----------------------FUNCION PARA ESCRITURA TRANSFERENCIA DE CONTROL AL SINTONIZADOR INICIO-----------------

//! Función transferencia USB de Control para escribir al Sintonizador usando el bus I2C.
/*!
  \param Addr Direccion del registro.
  \param Data Dato a escribir.
  \return Regresa el estado de la transferencia de control 0:Exitosa 1:Fallida.
*/
unsigned char STV4100_I2C_Write(unsigned char Addr, unsigned char Data)
{

    unsigned short wValue=funcion_combinar_bytes(Data, Addr);         //Crea un bloque de 2 bytes con dos bloques de 1 byte
    static unsigned char buffer[1]={0};                                            //Buffer para alojar recibir la respuesta de la transferencia

    //Función de la libreia LIBUSB para realizar la transferencia de control
    unsigned int respuesta = libusb_control_transfer(manejador_dispositivo_usb,     //Variable para manejar el dispsositvo usb
                                                                          0x40,     //bmRequestType SETPTUDATA[0] leer=0xC0 escribir=0x40
                                                                          0xA4,     //bRequest SETPTUDATA[1] en el firmware para hacer el caso TC90527_I2cWrite_US
                                                                        wValue,     //wValue SETPTUDATA[2 y 3] el valor a pasar
                                                                             0,     //wIndex SETPTUDATA[4 y 5], indica la dirección si es desde el afrintión al dispositivo, el numero de EndPoint.
                                                                        buffer,     //Recive un apuntador a los datos a leer o escribir
                                                                             1,     //wLength SETPTUDATA[6 y 7], numero de bytes a escribir o leer
                                                                             20);    //Tiempo para esperar la respuesta

    int resultado=buffer[0];                                                        //Asignar a resultado el valor del buffer
    if ((resultado!=0)||(respuesta!=1))                                             //Si resultado es distinto de cero o respuesta es distinto de uno
        std::cout<<"Error en escritura transferencia de control al sintonizador STV4100_I2C_Write_USB"<<std::endl;//Mostrar la siguiente advertencia

    return buffer[0];   //Regresa el estado de la transferencia de control 0:Exitosa 1:Fallida

}
//-----------------------FUNCION PARA ESCRITURA TRANSFERENCIA DE CONTROL AL SINTONIZADOR FIN--------------------

//-----------------------FUNCION PARA LECTURA TRANSFERENCIA DE CONTROL AL SINTONIZADOR INICIO-------------------

//! Función transferencia USB de Control para leer del Sintonizador usando el bus I2C.
/*!
  \param Addr Direccion del registro.
  \param *lpData Apuntador para regresar la data leida.
  \return Regresa el estado de la transferencia de control 0:Exitosa 1:Fallida.
*/
unsigned char STV4100_I2C_Read(unsigned char Addr, unsigned char *lpData)
{
	static unsigned char buffer[2]={0,0};                                          //Buffer para alojar recibir la respuesta de la transferencia

    //Función de la libreia LIBUSB para realizar la transferencia de control
    unsigned int respuesta = libusb_control_transfer(manejador_dispositivo_usb,     //Variable para manejar el dispsositvo usb
                                                                          0xC0,     //bmRequestType SETPTUDATA[0] leer=0xC0 escribir=0x40
                                                                          0xA5,     //bRequest SETPTUDATA[1] en el firmware para hacer el caso TC90527_I2cWrite_US
                                                                          Addr,     //wValue SETPTUDATA[2 y 3] el valor a pasar
                                                                             0,     //wIndex SETPTUDATA[4 y 5], indica la dirección si es desde el afrintión al dispositivo, el numero de EndPoint.
                                                                        buffer,     //Recive un apuntador a los datos a leer o escribir
                                                                             2,     //wLength SETPTUDATA[6 y 7], numero de bytes a escribir o leer
                                                                             20);    //Tiempo para esperar la respuesta

    int resultado = buffer[0];                                                       //Asignar a resultado el valor del buffer[0]
    if ((resultado!=0)||(respuesta!=2))                                             //Si resultado es distinto de cero o respuesta es distinto de dos
        std::cout<<"Error en lectura transferencia de control al demodulador STV4100_I2C_Read_US"<<std::endl; //Mostrar la siguiente advertencia
	*lpData=buffer[1];
    return buffer[0];   //Regresa el estado de la transferencia de control 0:Exitosa 1:Fallida
}

//-----------------------FUNCION PARA LECTURA TRANSFERENCIA DE CONTROL AL SINTONIZADOR FIN----------------------

//! Función que crea un variable de 2 bytes con dos variables de 1 byte usando operaciones lógicas.
/*!
  \param b1 variable de 1 byte.
  \param b2 variable de 1 byte.
  \return variable de 2 bytes.
*/
static unsigned short funcion_combinar_bytes(unsigned char b1, unsigned char b2)
{
    int combinados = b1 << 8 | b2;          //Mueve b1 8 bits hacia la izquierda, luego operacion logicar or con b2
    return (unsigned short)combinados;      //Regresa la variable combinada de dos bytes
}

//! Procedimiento para realizar retardo en milisegundos.
/*!
  \param tiempo_ms tiempo en milisegundos.
*/
void procedimiento_retardo_milisegundos(unsigned int tiempo_ms)
{
    tiempo_ms=tiempo_ms*1000;               //Multplica por mil
    usleep(tiempo_ms);                      //Funcion usleep de la librería unistd.h realiza retardo en microsegundos
}
#endif // SAMSUNG_H_INCLUDED
