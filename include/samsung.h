#ifndef SAMSUNG_H_INCLUDED
#define SAMSUNG_H_INCLUDED
#include <cmath>
#include <unistd.h>
#include "usb.h"     //For device_handle

//------------------------------------HEADERS FUNCTION BEGIN------------------------------
unsigned char SemcoTC90527Init();
unsigned char SemcoTC90527SoftReset();
unsigned char SemcoTC90527RegReset();
unsigned char SemcoTC90527SleepOn();
unsigned char SemcoTC90527SleepOff();
unsigned char SemcoTC90527MasterLock();
double SemcoTC90527_ISDBT_SNRResultCheck();
unsigned char SemcoTC90527_Set_TS_Output(unsigned char nTS);
unsigned char SemcoTC90527_GetDatas();
unsigned char SemcoTC90527_GetSignalQuality();
unsigned char SemcoTC90527_GetIfAgcLevel();
unsigned char SemcoTC90527_GetSSI();
unsigned char SemcoSTV4100_Initialize();
unsigned char SemcoSTV4100_SetFrequency(unsigned long nFrequency_KHz, unsigned char BW);
unsigned char SemcoSTV4100_GetLockStatus();
unsigned char SemcoSTV4100_On();
unsigned char SemcoSTV4100_StandBy();
unsigned char TC90527_I2cWrite_USB(unsigned char REGADDR,unsigned char DATAI2C);
unsigned char TC90527_I2cRead_USB(unsigned char REGADDR);
unsigned char STV4100_I2C_Write_USB(unsigned char REGADDR, unsigned char DATAI2C);
unsigned char STV4100_I2C_Read_USB(unsigned char REGADDR);
unsigned short combine(unsigned char b1, unsigned char b2);
void SemcoSleep(unsigned int nSleepTims_ms);
//------------------------------------HEADERS FUNCTION END--------------------------------

//------------------------------------VARIABLES BEGIN-------------------------------------
//---------------For Samsung comunication Begin------------
long int DataReadDemo=0;               // Store the data read from the demodulator and tuner
unsigned char DataReadTuner = 0;      // esta es utilizada por la funcion de leer data del TUNER devolviendo la data leida
unsigned char result_func_SNR = 0;    //variable de estado de la funcion
unsigned char result_func_GSQ = 0;    //variable de estado de la funcion "SemcoTC90527_GetSignalQuality"
// begin
unsigned char bLock;
double SNR;
float pre_BER;
float post_BER;
int mode_90527;
int code_rate;
unsigned int hab_error;
// end
unsigned char STV4100_Reg[15];  //This variable modifies register add initialization
//---------------For Samsung comunication End------------
//------------------------------------VARIABLES END---------------------------------------

//Declarations
unsigned char TC90527_I2cWrite_USB(unsigned char REGADDR,unsigned char DATAI2C);
unsigned char TC90527_I2cRead_USB(unsigned char REGADDR);
unsigned char STV4100_I2C_Write_USB(unsigned char REGADDR, unsigned char DATAI2C);
unsigned char STV4100_I2C_Read_USB(unsigned char REGADDR);
unsigned short combine(unsigned char b1, unsigned char b2);
void SemcoSleep(unsigned int nSleepTims_ms);



//-----------------------FUNCTIONS FOR THE DEMULATOR SAMSUNG BEGIN------------------------

unsigned char SemcoTC90527Init()
{
    unsigned char result = 0;//inicializa con 0 para indicar que no ha ocurrido un problema

    result |= TC90527_I2cWrite_USB(0x01, 0x40); // reset

    result |= TC90527_I2cWrite_USB(0x0D, 0x00); // output enable
    result |= TC90527_I2cWrite_USB(0x0F, 0x01); // Set the parallel TS 20110214 by yooheeyong

    result |= TC90527_I2cWrite_USB(0x11, 0x2D); // for TC90527 : Divider ratio of VCO
    result |= TC90527_I2cWrite_USB(0x12, 0x02); // for TC90527 : Divider ratio of Ref. clock

    result |= TC90527_I2cWrite_USB(0x13, 0x62); // carrier freq. offsetk
    result |= TC90527_I2cWrite_USB(0x14, 0x60); // carrier freq. offsetk
    result |= TC90527_I2cWrite_USB(0x15, 0x00); // carrier freq. offsetk

    result |= TC90527_I2cWrite_USB(0x16, 0x04); // IQ Baseband mode[2]  : Zero IF Selection

    result |= TC90527_I2cWrite_USB(0x1C, 0x20); // output enable(except RFAGC)
    result |= TC90527_I2cWrite_USB(0x1D, 0x00); // output enable
    result |= TC90527_I2cWrite_USB(0x1E, 0x00); // output enable
    result |= TC90527_I2cWrite_USB(0x1F, 0x00); // output enable

    result |= TC90527_I2cWrite_USB(0x20, 0x00); // ifagc mode
    result |= TC90527_I2cWrite_USB(0x22, 0x80); // ifagc mode
    result |= TC90527_I2cWrite_USB(0x23, 0x4C); // depend on tuner
    result |= TC90527_I2cWrite_USB(0x30, 0x28); // for TC90527 : Spectrum polarity inversion [3]

    result |= TC90527_I2cWrite_USB(0x31, 0x00); // zero(0MHz) IF
    result |= TC90527_I2cWrite_USB(0x32, 0x00); // zero(0MHz) IF
    result |= TC90527_I2cWrite_USB(0x34, 0x0F); // for TC90527 : Digital Filter mode(90M, BW 6M), IF_AGC(x1), RF_AGC(x1)
    result |= TC90527_I2cWrite_USB(0x38, 0x01); // for TC90527 : AFC
    result |= TC90527_I2cWrite_USB(0x39, 0x1C); // for TC90527 : AFC

    result |= TC90527_I2cWrite_USB(0x3B, 0x00); // for TC90527 : symds_off[11:8]
    result |= TC90527_I2cWrite_USB(0x3C, 0x00); // for TC90527 : symds_off[7:0]
    result |= TC90527_I2cWrite_USB(0x47, 0x10); // for TC90527 : mdsmd[4]    ==> 1 0x00010000

    result |= TC90527_I2cWrite_USB(0x73, 0x40); // for TC90527 : pvita : 1, psela : 0

    result |= TC90527_I2cWrite_USB(0xD0, 0x1E); // for TC90527 : ADC Ref. level set up 1
    result |= TC90527_I2cWrite_USB(0xD1, 0x0B); // for TC90527 : ADC Ref. level set up 2
    result |= TC90527_I2cWrite_USB(0xEF, 0x01); // recommendation
    result |= TC90527_I2cWrite_USB(0xF9, 0x00); // fbstoff set '0'

    // for SAT
    //----------------------------------------
    result |= TC90527_I2cWrite_USB(0x50, 0x00); // recommend by RyuSH for SAT 20110314
    result |= TC90527_I2cWrite_USB(0xF1, 0x80); // recommend by RyuSH for SAT 20110314
    result |= TC90527_I2cWrite_USB(0x2B, 0x20); // recommend by RyuSH for SAT 20110314
    //----------------------------------------

    return result;

}


// Soft Reset for TC90527
unsigned char SemcoTC90527SoftReset()
{
    unsigned char result = 0;//inicializa con 0 para indicar que no ha ocurrido un problema
    result |= TC90527_I2cWrite_USB(0x01, 0x40);
    return result;
}

// Register Reset
unsigned char SemcoTC90527RegReset()
{
    unsigned char result = 0;//inicializa con 0 para indicar que no ha ocurrido un problema
    result |= TC90527_I2cWrite_USB(0x01, 0x80);
    return result;
}

// Sleep On
unsigned char SemcoTC90527SleepOn()
{
    unsigned char result = 0;//inicializa con 0 para indicar que no ha ocurrido un problema
    result |= TC90527_I2cWrite_USB(0x03, 0x80);
    return result;
}

// Sleep Off
unsigned char SemcoTC90527SleepOff()
{
    unsigned char result = 0;//inicializa con 0 para indicar que no ha ocurrido un problema
    result |= TC90527_I2cWrite_USB(0x03, 0x00);
    return result;
}

// Check master lock
// retorna en "DataReadDemo"  : 1-lock, 0-unlock
unsigned char SemcoTC90527MasterLock()
{
    unsigned char result = 0;//inicializa con 0 para indicar que no ha ocurrido un problema
    result |= TC90527_I2cRead_USB(0x96); // change lock register from 0x80 to 0x96 by yooheeyong 20091110

    // add if statement for lock register from 0x80 to 0x96 by yooheeyong 20091110
   // " Original "comp= (DataReadDemo & 0x80)|| (DataReadDemo & 0x40) || (DataReadDemo & 0x20;
    if (((DataReadDemo & 0x80) > 0) || ((DataReadDemo & 0x40) > 0) || ((DataReadDemo & 0x20) > 0))
    { // rlocka[7], rlockb[6], rlockc[5]
        DataReadDemo = 1;
    }
    else
    {
        DataReadDemo = 0;
    }

    return result;
}


// ISDB-T SNR
double SemcoTC90527_ISDBT_SNRResultCheck()
{

    double dSnWork;
    double dSn;
    unsigned long ulData=0;

    result_func_SNR |= TC90527_I2cRead_USB(0x8B);

    if (DataReadDemo < 0)
    {
        return -1;
    }

    else
    {
        ulData = DataReadDemo;
    }

    result_func_SNR |= TC90527_I2cRead_USB(0x8C);

    if (DataReadDemo < 0)
    {
        return -1;
    }
    else
    {
        ulData <<= 8;
        ulData |= DataReadDemo;
    }

    result_func_SNR |= TC90527_I2cRead_USB(0x8D);

    if (DataReadDemo < 0)
    {
        return -1;
    }
    else
    {
        ulData <<= 8;
        ulData |= DataReadDemo;
    }

    if (ulData == 0.0)
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

 // Set TS Output
// nTS : 0-serial, 1-parallel
unsigned char SemcoTC90527_Set_TS_Output(unsigned char nTS)
{
    unsigned char result = 0;
    unsigned char temp = 0;

    if (nTS == 0)
    {
        result |= TC90527_I2cRead_USB(0x0F);
        temp = DataReadDemo;
        temp &= 0xFC;
        result |= TC90527_I2cWrite_USB(0x0F, temp);
    }
    else
    {
        result |= TC90527_I2cRead_USB(0x0F);
        temp = DataReadDemo;
        temp |= 0x02;
        result |= TC90527_I2cWrite_USB(0x0F, temp);
    }

    return result;
}
// Get Test Data
unsigned char SemcoTC90527_GetDatas()
{
    unsigned char result = 0;
    //int temp1,temp2,temp3;
    int perra, pecya;

    unsigned char tempwrite, a_seg, fecbst, vberorg, perra_2, perra_1, perra_0, pecya_1, pecya_0;

    result |= SemcoTC90527MasterLock();
    bLock=DataReadDemo;
    SNR=SemcoTC90527_ISDBT_SNRResultCheck();
    result |= result_func_SNR;

    result |= TC90527_I2cRead_USB(0xB3);
    mode_90527 = DataReadDemo;
    mode_90527 = mode_90527 & 0xE0;
    //KS_data1.modulation_scheme;

    result |= TC90527_I2cRead_USB(0xB3);
    code_rate=DataReadDemo;
    code_rate = code_rate & 0x1C;
    //KS_data1.coding_rate;

    //Selecting the measurement cycle mode   - Packet Mode Select
    tempwrite = 0x00 & 0xFD;
    result |= TC90527_I2cWrite_USB(0x77, tempwrite);

    //Setting the measurement cycle
    tempwrite = 0x02 | (0x60 >> 2);
    result |= TC90527_I2cWrite_USB(0x76, tempwrite);

    //Selecting the BER measurement layer   - Layer A Select
    tempwrite = 0x00;
    result |= TC90527_I2cWrite_USB(0x71, tempwrite);

    result |= TC90527_I2cWrite_USB(0x73, 0x40);// for TC90527 : pvita : 1, psela : 0

    //read a_seg[3:0] - B4
    result |= TC90527_I2cRead_USB(0xB4);
    a_seg = (unsigned char)(DataReadDemo & 0x78);
    a_seg = (unsigned char)(a_seg >> 3);

    //read fecbst - 0xFB
    result |= TC90527_I2cRead_USB(0xFB);
    fecbst = (unsigned char)(DataReadDemo & 0x10);
    fecbst = (unsigned char)(fecbst >> 4);

    if (a_seg == 1)
    {
        vberorg = 0x02;
        tempwrite = (unsigned char)(vberorg & 0x7F); //vberorg Set '0'
        tempwrite = (unsigned char)(vberorg | 0x40); //vberbst Set '1'
        result |= TC90527_I2cWrite_USB(0xF8, tempwrite);
    }
    else
    {
        if (fecbst == 1)  //fecbst = 1
        {
            vberorg = 0x02;
            tempwrite = (unsigned char)(vberorg | 0xC0); //vberorg set '1', vberbst Set '1'
            result |= TC90527_I2cWrite_USB(0xF8, tempwrite);
        }
        else //fecbst = 0
        {
            vberorg = 0x02;
            tempwrite = (unsigned char)(vberorg & 0x7F);
            tempwrite = (unsigned char)(vberorg | 0x40); //vberbst Set '1'
            result |= TC90527_I2cWrite_USB(0xF8, tempwrite);
        }

    }
    result |= TC90527_I2cRead_USB(0x9D);
    perra_2 = DataReadDemo;
    result |= TC90527_I2cRead_USB(0x9E);
    perra_1 = DataReadDemo;
    result |= TC90527_I2cRead_USB(0x9F);
    perra_0 = DataReadDemo;

    result |= TC90527_I2cRead_USB(0xA6);
    pecya_1 = DataReadDemo;
    result |= TC90527_I2cRead_USB(0xA7);
    pecya_0 = DataReadDemo;

    perra = (perra_2 << 16) | (perra_1 << 8) | perra_0;  //perra
    pecya = (pecya_1 << 8) | pecya_0;   //pecya

    pre_BER = ((float)perra / (float)(pecya * 204 * 8));

    result |= TC90527_I2cWrite_USB(0x73, 0x00);// for TC90527 : pvita : 0, psela : 0

    //Selecting the measurement cycle mode   - Packet Mode Select
    tempwrite = 0x00 & 0xFD;
    result |= TC90527_I2cWrite_USB(0x77, tempwrite);

    //Setting the measurement cycle
    tempwrite = 0x02 | 0x60;
    result |= TC90527_I2cWrite_USB(0x76, tempwrite);

    //Selecting the BER measurement layer   - Layer A Select
    tempwrite = 0x00;
    result |= TC90527_I2cWrite_USB(0x76, tempwrite);

    result |= TC90527_I2cRead_USB(0x9D);
    perra_2 = DataReadDemo;
    result |= TC90527_I2cRead_USB(0x9E);
    perra_1 = DataReadDemo;
    result |= TC90527_I2cRead_USB(0x9F);
    perra_0 = DataReadDemo;

    result |= TC90527_I2cRead_USB(0xA6);
    pecya_1 = DataReadDemo;
    result |= TC90527_I2cRead_USB(0xA7);
    pecya_0 = DataReadDemo;

    perra = (perra_2 << 16) | (perra_1 << 8) | perra_0;  //perra
    pecya = (pecya_1 << 8) | pecya_0;   //pecya

    post_BER = ((float)(perra * 8 * 0.5 * 9) / (float)(pecya * 204 * 8));

    if (post_BER >= 0.0004)  //packet Áß¿¡ ±úÁø Bit°¡ ÀÖÀ½.  - 4 X 10-4
    {
        hab_error = hab_error + 1;
    }

    return result;
}

//Get Signal Quality
unsigned char SemcoTC90527_GetSignalQuality()
{

    unsigned char lock_state = 0;
    result_func_GSQ |= SemcoTC90527MasterLock();//locking check!!
    lock_state = DataReadDemo ;

    if (lock_state == 0)   //unlock
    {
        return 0;
    }
    else   // lock
    {
        SNR = SemcoTC90527_ISDBT_SNRResultCheck();
        result_func_GSQ |= result_func_SNR;
        if (SNR < 21.5)
        {
            return 1;
        }
        else if ((SNR >= 21.5) && (SNR < 22.7))
        {
            return 2;
        }
        else if ((SNR >= 22.7) && (SNR < 23.9))
        {
            return 3;
        }
        else if ((SNR >= 23.9) && (SNR < 25.1))
        {
            return 4;
        }
        else if ((SNR >= 25.1) && (SNR < 26.3))
        {
            return 5;
        }
        else if ((SNR >= 26.3) && (SNR < 27.5))
        {
            return 6;
        }
        else if ((SNR >= 27.5) && (SNR < 28.7))
        {
            return 7;
        }
        else if ((SNR >= 28.7) && (SNR < 29.9))
        {
            return 8;
        }
        else if ((SNR >= 29.9) && (SNR < 31.1))
        {
            return 9;
        }
        else if (SNR >= 31.1)
        {
            return 10;
        }
    }
    return 0;
}

// Get IF AGC Level //
unsigned char SemcoTC90527_GetIfAgcLevel()
{
    unsigned char result = 0;

    result |= TC90527_I2cRead_USB(0x82);
    //DataReadDemo;
    return result;

}

// Get SSI //
unsigned char SemcoTC90527_GetSSI()
{
    unsigned char result = 0;

    unsigned char nLevel;
    unsigned char Sig_strength;

    result |= SemcoTC90527_GetIfAgcLevel();
    nLevel = DataReadDemo;
    SemcoTC90527MasterLock();
    if (DataReadDemo == 1)
    {
        if (nLevel < 106)
            Sig_strength = 10;
        else if (nLevel >= 106 && nLevel < 112)
            Sig_strength = 9;
        else if (nLevel >= 112 && nLevel < 120)
            Sig_strength = 8;
        else if (nLevel >= 120 && nLevel < 124)
            Sig_strength = 7;
        else if (nLevel >= 124 && nLevel < 128)
            Sig_strength = 6;
        else if (nLevel >= 128 && nLevel < 134)
            Sig_strength = 5;
        else if (nLevel >= 134 && nLevel < 141)
            Sig_strength = 4;
        else if (nLevel >= 141 && nLevel < 153)
            Sig_strength = 3;
        else if (nLevel >= 153 && nLevel < 169)
            Sig_strength = 2;
        else
            Sig_strength = 1;

    }
    else
    {
        Sig_strength = 0;
    }

    DataReadDemo=Sig_strength;

    return result;
}
//-----------------------FUNCTIONS FOR THE DEMULATOR SAMSUNG END------------------------

//-----------------------FUNCTIONS FOR THE TUNER SAMSUNG BEGIN--------------------------
// Initialize the TV4100
unsigned char SemcoSTV4100_Initialize()
{
    unsigned char result=0;
    unsigned char i;

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

    for (i = 1; i < 15; i++)//PENDIENTE POBRAR DESDE i=0
    {
        result |= STV4100_I2C_Write_USB(i, STV4100_Reg[i]);
    }
    /*4100 power up done once (or done by config file)*/
    result |= STV4100_I2C_Read_USB(0x06);

    if ((DataReadTuner & 0x80) > 0)
    {
        DataReadTuner = 0x30;
        DataReadTuner = (unsigned char)~DataReadTuner;
        STV4100_Reg[3] &= DataReadTuner;
        STV4100_Reg[3] |= 0x20;
        result |= STV4100_I2C_Write_USB(0x03, STV4100_Reg[0x03]);

        STV4100_Reg[6] &= 0x1f;
        STV4100_Reg[6] |= 0x20;
        result |= STV4100_I2C_Write_USB(0x06, STV4100_Reg[0x06]);

        SemcoSleep(20); /*startup time*/
    }

    return result;
}

// Set Frequency //
unsigned char SemcoSTV4100_SetFrequency(unsigned long nFrequency_KHz, unsigned char BW)
{
    unsigned char result;
    unsigned char i;
    unsigned char  CalValue, CLPF_CalValue;
    unsigned long InternalClock;
    unsigned long STV4100_Fxtal,ret;
    unsigned char cpma;
    unsigned long N = 0, F = 0;

    STV4100_Fxtal = 16000; // XTal = 16 MHz
    InternalClock = 0;
    result = 0;
    cpma = 0;

    ret = 0;

    //cal BB filter each time //
    result |= STV4100_I2C_Read_USB(0x01);
    DataReadTuner |= 0x04;
    result |= STV4100_I2C_Write_USB(0x01, DataReadTuner);

    SemcoSleep(1);

    result |= STV4100_I2C_Write_USB(0x01, STV4100_Reg[1]);

    result |= STV4100_I2C_Read_USB(0x0C);

    DataReadTuner &= 0xF7;//0x80 negado
    result |= STV4100_I2C_Write_USB(0x0C, DataReadTuner);
    SemcoSleep(1);
    result |= STV4100_I2C_Write_USB(0x0C, STV4100_Reg[12]);

    //store cpma value each frequency change//
    result |= STV4100_I2C_Read_USB(0x0C);
    CalValue = (unsigned char)(DataReadTuner & 0x07);

    result |= STV4100_I2C_Read_USB(0x0B);
    DataReadTuner &= 0xDF;//0x20 negado
    result |= STV4100_I2C_Write_USB(0x0B, DataReadTuner);
    SemcoSleep(1);
    result |= STV4100_I2C_Write_USB(0x0B, STV4100_Reg[11]);

    //store cpma value each frequency change//
    result |= STV4100_I2C_Read_USB(0x0B);
    CLPF_CalValue = (unsigned char)(DataReadTuner & 0x1F);
    result |= STV4100_I2C_Write_USB(0x0B, STV4100_Reg[11]);

    if (BW == 8)
    {
        STV4100_Reg[11] |= 0x40;
    }
    else if ((BW == 7) || (BW == 6))
    {
        STV4100_Reg[11] &= 0xBF;//0x40 negado
    }

    STV4100_Reg[11] &= 0xBF;//0x40 negado
    STV4100_Reg[8] &= 0x7F;//0x80 negado
    STV4100_Reg[3] &= 0x3F;//0xc0 negado
    STV4100_Reg[6] &= 0xF3;//0x0c negado

    if (nFrequency_KHz <= 325000)
    {
        ret = nFrequency_KHz * 16;
        STV4100_Reg[8] &= 0x7F;//0x80 negado
        STV4100_Reg[3] |= 0x80;
        STV4100_Reg[6] |= 0x04;
    }
    else if (nFrequency_KHz <= 650000)
    {
        ret = nFrequency_KHz * 8;
        STV4100_Reg[8] |= 0x80;
        STV4100_Reg[3] |= 0x40;
        STV4100_Reg[6] |= 0x08;
    }
    else if (nFrequency_KHz <= 875000)
    {
        ret = nFrequency_KHz * 4;
        STV4100_Reg[8] |= 0x80;
        STV4100_Reg[3] &= 0x3F;
        STV4100_Reg[6] |= 0x08;
    }

    // fill  CP current//
    STV4100_Reg[3] &= 0xF0;

    if ((int)(ret / 1000) < 2700)
        STV4100_Reg[3] |= 0;
    else if ((int)(ret / 1000) < 2950)
        STV4100_Reg[3] |= 1 << 1;
    else if ((int)(ret / 1000) < 3300)
        STV4100_Reg[3] |= 2 << 1;
    else if ((int)(ret / 1000) < 3700)
        STV4100_Reg[3] |= 3 << 1;
    else if ((int)(ret / 1000) < 4200)
        STV4100_Reg[3] |= 5 << 1;
    else if ((int)(ret / 1000) < 4800)
        STV4100_Reg[3] |= 6 << 1;
    else //if ((int)(ret/1000)<5200)//
        STV4100_Reg[3] |= 7 << 1;

    // set internal clock//
    if (STV4100_Fxtal > 25000)
    {
        InternalClock = STV4100_Fxtal / 2;
    }
    else
    {
        InternalClock = STV4100_Fxtal;
    }

    //write N value//
    N = (ret / InternalClock);
    STV4100_Reg[2] = (unsigned char)(N & 0xff);
    STV4100_Reg[3] &= 0xFE;
    STV4100_Reg[3] |= (unsigned char)((N >> 8) & 0x01);

    if (ret<= 4294967)
    {
        ret=(unsigned long)(1000*ret/InternalClock);
        F= (ret-1000*N)*(262144-1)/1000;
    }
    else
    {
        ret = (unsigned long)(100*ret/InternalClock);
        F = (ret - 100 * N) * (262144 - 1) / 100;
    }

    //write F value//
    STV4100_Reg[4] = (unsigned char)(F & 0xFF);
    STV4100_Reg[5] = (unsigned char)((F >> 8) & 0xFF);
    STV4100_Reg[6] &= 0xFC;
    STV4100_Reg[6] |= (unsigned char)((F >> 16) & 0x3);

    // don't use sigma delta if not needed//
    if (F == 0)
        STV4100_Reg[6] &= 0xDF;
    else
        STV4100_Reg[6] |= 0x20;

    //calibrate vco for each frequency change//
    STV4100_Reg[6] |= 0x10;

    //cpma compensation//
    if (CalValue == 0)
        cpma = 1;
    else if (CalValue < 4)
    {
        if (BW == 8)
            cpma = (unsigned char)(2 * CalValue);
        else
            cpma = (unsigned char)((2 * CalValue) + 1);
    }
    else
        cpma = 7;

    STV4100_Reg[0x0c] &= 0xF8;
    STV4100_Reg[0x0c] |= cpma;
    STV4100_Reg[0x0b] |= 0x80;
    STV4100_Reg[0x0c] |= 0x08;

    //set RFAGC to def value//

    /////////write 14 rw registers//////
    for (i = 0x01; i < 15; i++)
        result |= STV4100_I2C_Write_USB(i, STV4100_Reg[i]);

    //check tuner lock but does not affect error//
    i = 0;
    do
    {
        result |= STV4100_I2C_Read_USB(0x01);
        if ((DataReadTuner & 0x40)>0)
            break;
        SemcoSleep(10);
        i++;
    } while (i < 6);

    STV4100_Reg[11] &= 0xE0;

    //ning 2010.3.9
    CLPF_CalValue = 31;
    STV4100_Reg[11] |= CLPF_CalValue;//31 ==> 24
    STV4100_Reg[11] |= 0x20;
    result |= STV4100_I2C_Write_USB(0x0B, STV4100_Reg[11]);

    STV4100_Reg[9] &= 0xF8;
    STV4100_Reg[9] |= 0x01;
    result |= STV4100_I2C_Write_USB(0x09, STV4100_Reg[9]);

    return result;
}

// Check lock status //
// return : 1-pll lock, 0-pll unlock //
unsigned char SemcoSTV4100_GetLockStatus()
{
    unsigned char result=0;

    result |= STV4100_I2C_Read_USB(0x01);

    DataReadTuner = (unsigned char)((DataReadTuner & 0x40) >> 6);

    return result;
}

// Wake up //
unsigned char SemcoSTV4100_On()
{
    unsigned char result = 0;

    result |= STV4100_I2C_Write_USB(0x06, 0x20);

    return result;
}

// Sleep //
unsigned char SemcoSTV4100_StandBy()
{
    unsigned char result = 0;

    result |= STV4100_I2C_Write_USB(0x06, 0xC0);

    return result;
}

//-----------------------FUNCTIONS FOR THE TUNER SAMSUNG END----------------------------

//-----------------------FUNCTION TO WRITE TO THE DEMULATOR BEGIN-----------------------

//requiere de la direccion de registro y del dato que se decea escribir al Demo por el bus I2c
unsigned char TC90527_I2cWrite_USB(unsigned char REGADDR,unsigned char DATAI2C)
{
    //Combine this values, that will be transmited
    unsigned short wValue=combine(DATAI2C, REGADDR);

    //Buffer to get the response from the firmware
    unsigned char buffer[1];
    buffer[0]=0;

    //Fuction of libusb to do control transfer
    unsigned int response = libusb_control_transfer(
            manejador_dispositivo_usb,     //For device handle
            0x40,              //bmRequestType SETPTUDATA[0] read=0xC0 or write=0x40
            0xA2,              //bRequest SETPTUDATA[1] in the firmware case to do TC90527_I2cWrite_US
            wValue,            //wValue SETPTUDATA[2 y 3] this is the value
            0,                 //wIndex SETPTUDATA[4 y 5], in this case index 0
            buffer,            //recieve a pointer to the buffer of data to transmit or buffer to recive depends of read_or_write
            1,                 //wLength SETPTUDATA[6 y 7], number of data to transmit or cieve in bytes, in this case response with one byte
            100                //timeout waititng for response
        );

    //status 0:Operation Successful   1:Operation Fail
    int compar=buffer[0];
    if ((compar!=0)||(response!=1)) std::cout<<"Error in TC90527_I2cWrite_USB"<<std::endl;

    //Return the status of operation status 0:Operation Successful   1:Operation Fail
    return buffer[0];
}
//-----------------------FUNCTION TO WRITE TO THE DEMULATOR END-------------------------

//-----------------------FUNCTION TO READ FROM THE DEMULATOR BEGIN----------------------

unsigned char TC90527_I2cRead_USB(unsigned char REGADDR)
{
    //Buffer to get the response from the firmware
    unsigned char buffer[2];
    buffer[0]=0;
    buffer[1]=0;

    //Fuction of libusb to do control transfer
    unsigned int response = libusb_control_transfer(
            manejador_dispositivo_usb,     //For device handle
            0xC0,              //bmRequestType SETPTUDATA[0] read=0xC0 or write=0x40
            0xA3,              //bRequest SETPTUDATA[1] in the firmware case to do TC90527_I2cRead_USB
            REGADDR,            //wValue SETPTUDATA[2 y 3] this is the value
            0,                 //wIndex SETPTUDATA[4 y 5], in this case index 0
            buffer,            //recieve a pointer to the buffer of data to transmit or buffer to recive depends of read_or_write
            2,                 //wLength SETPTUDATA[6 y 7], number of data to transmit or cieve in bytes, in this case response with one byte
            100                //timeout waititng for response
        );

    //Store the data read
    DataReadDemo = buffer[1];

    //status 0:Operation Successful   1:Operation Fail
    int compar=buffer[0];
    if ((compar!=0)||(response!=2)) std::cout<<"Error in TC90527_I2cRead_USB"<<std::endl;

    //Return the status of operation status 0:Operation Successful   1:Operation Fail
    return buffer[0];
}
//-----------------------FUNCTION TO READ FROM THE DEMULATOR END------------------------

//-----------------------FUNCTION TO WRITE TO THE TUNER BEGIN---------------------------
unsigned char STV4100_I2C_Write_USB(unsigned char REGADDR, unsigned char DATAI2C)
{
    //Combine this values, that will be transmited
    unsigned short wValue=combine(DATAI2C, REGADDR);

    //Buffer to get the response from the firmware
    unsigned char buffer[1];
    buffer[0]=0;

    //Fuction of libusb to do control transfer
    unsigned int response = libusb_control_transfer(
            manejador_dispositivo_usb,     //For device handle
            0x40,              //bmRequestType SETPTUDATA[0] read=0xC0 or write=0x40
            0xA4,              //bRequest SETPTUDATA[1] in the firmware case to do TC90527_I2cWrite_US
            wValue,            //wValue SETPTUDATA[2 y 3] this is the value
            0,                 //wIndex SETPTUDATA[4 y 5], in this case index 0
            buffer,            //recieve a pointer to the buffer of data to transmit or buffer to recive depends of read_or_write
            1,                 //wLength SETPTUDATA[6 y 7], number of data to transmit or recieve in bytes, in this case response with one byte
            0                  //timeout waititng for response
        );

    //status 0:Operation Successful   1:Operation Fail
    int compar=buffer[0];
    if ((compar!=0)||(response!=1)) std::cout<<"Error in STV4100_I2C_Write_USB"<<std::endl;

    //Return the status of operation status 0:Operation Successful   1:Operation Fail
    return buffer[0];
}
//-----------------------FUNCTION TO WRITE TO THE TUNER END----------------------------

//-----------------------FUNCTION TO READ FROM THE TUNER BEGIN-------------------------
unsigned char STV4100_I2C_Read_USB(unsigned char REGADDR)
{
    //Buffer to get the response from the firmware
    unsigned char buffer[2];
    buffer[0]=0;
    buffer[1]=0;

    //Fuction of libusb to do control transfer
    unsigned int response = libusb_control_transfer(
            manejador_dispositivo_usb,     //For device handle
            0xC0,              //bmRequestType SETPTUDATA[0] read=0xC0 or write=0x40
            0xA5,              //bRequest SETPTUDATA[1] in the firmware case to do TC90527_I2cRead_USB
            REGADDR,            //wValue SETPTUDATA[2 y 3] this is the value
            0,                 //wIndex SETPTUDATA[4 y 5], in this case index 0
            buffer,            //recieve a pointer to the buffer of data to transmit or buffer to recive depends of read_or_write
            2,                 //wLength SETPTUDATA[6 y 7], number of data to transmit or cieve in bytes, in this case response with one byte
            100                //timeout waititng for response
        );

    //Store the data read
    DataReadDemo = buffer[1];

    //status 0:Operation Successful   1:Operation Fail
    int compar=buffer[0];
    if ((compar!=0)||(response!=2)) std::cout<<"Error in STV4100_I2C_Read_USB"<<std::endl;

    //Return the status of operation status 0:Operation Successful   1:Operation Fail
    return buffer[0];
}

//-----------------------FUNCTION TO READ FROM THE TUNER END---------------------------

//Function to combine two bytes
unsigned short combine(unsigned char b1, unsigned char b2)
{
    int combined = b1 << 8 | b2; //Move b1 8 bits to the right and then do or with b2
    return (unsigned short)combined; //Return combined as unsigned short
}

//Function to make a delay in ms
void SemcoSleep(unsigned int nSleepTims_ms)
{
    nSleepTims_ms=nSleepTims_ms*1000;// to transform to miliseconds
    usleep(nSleepTims_ms); //Function usleep of standar library of  c++
}
#endif // SAMSUNG_H_INCLUDED
