/* 
 * File:   app_main.c
 * Author: user
 *
 * Created on 22 juin 2013, 10:36
 */

#include <stdio.h>
#include <stdlib.h>
#include "machine.h"
//#include <plib/timers.h>


#define float24 float

// CONFIG1H
#pragma config OSC = HSPLL         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = BOHW     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)



#define BTN_1    PORTBbits.RB0
#define BTN_2    PORTBbits.RB1

#define BTN_3    PORTAbits.RA0
#define BTN_4    PORTAbits.RA1



#define DRV_MOT_BASIC_PIN_U         PORTDbits.RD7
#define DRV_MOT_BASIC_PIN_V         PORTDbits.RD6
#define DRV_MOT_BASIC_PIN_W         PORTDbits.RD5

#define DRV_MOT_BASIC_PIN_U_TRIS    TRISDbits.TRISD7
#define DRV_MOT_BASIC_PIN_V_TRIS    TRISDbits.TRISD6
#define DRV_MOT_BASIC_PIN_W_TRIS    TRISDbits.TRISD5


enum{
    DRV_MOT_VDD=1,
    DRV_MOT_0V=0
};


// secteurs encadrés par les vecteurs u1-u6
// vecteurs répertoriés dans DRV_MOT_SECT_TYPE
enum{
    DRV_MOT_SECT_0,
    DRV_MOT_SECT_1,
    DRV_MOT_SECT_2,
    DRV_MOT_SECT_3,
    DRV_MOT_SECT_4,
    DRV_MOT_SECT_5,
    DRV_MOT_NB_SECT
};



// angle à 0 par rapport à u
typedef struct{
    uint8       drv_u;
    uint8       drv_v;
    uint8       drv_w;
}DRV_MOT_DRIVE_TYPE;


#define DRV_MOT_ANGLE_BASE_VECT_DEG     (60.0)


// vecteurs de base. L'angle correspondant est :
// index * DRV_MOT_ANGLE_BASE_VECT_DEG  avec index[0-5]
DRV_MOT_DRIVE_TYPE       MotSectors[6]
={
    { DRV_MOT_VDD ,  DRV_MOT_0V ,    DRV_MOT_0V },      // DRV_MOT_SECT_0
    { DRV_MOT_VDD ,  DRV_MOT_VDD ,   DRV_MOT_0V },      // DRV_MOT_SECT_1
    { DRV_MOT_0V ,   DRV_MOT_VDD ,   DRV_MOT_0V },
    { DRV_MOT_0V ,   DRV_MOT_VDD ,   DRV_MOT_VDD },
    { DRV_MOT_0V ,   DRV_MOT_0V ,    DRV_MOT_VDD },
    { DRV_MOT_VDD ,  DRV_MOT_0V ,    DRV_MOT_VDD }     // DRV_MOT_SECT_5
};


     uint8   g_val_pwm_u = 0;
     uint8   g_val_pwm_v = 0;
     uint8   g_val_pwm_w = 0;



void   CalcDriveU16 ( uint16 li_angle_Deg ){
    DRV_MOT_DRIVE_TYPE  l_drive_a, l_drive_b;
    static uint8   l_u8_Sect_a;
    static uint8   l_u8_Sect_b;
    static float24 l_f_Angle_a_Deg;
    static uint16 l_u16_Angle_a_Deg;
    static float24 l_f_temp;
    uint8   l_u8_tmp, l_u8_tmp2;
    //static float24 l_f_Angle_b_Deg;
    static float24 l_f_Angle_Rel_Deg;
    static float24 l_f_RapportDrive_b_a;

    // -----------------------------------------------------------
    // suivant l'angle, prendre les deux configurations de pilotage
    // donc déterminer les deux secteurs correspondants
    //l_u8_Sect_a = (uint8)(li_angle_Deg / DRV_MOT_ANGLE_BASE_VECT_DEG);
    l_u8_Sect_a = (uint8)( li_angle_Deg / (uint16)DRV_MOT_ANGLE_BASE_VECT_DEG);

    // si erreur alors ....
    if ( l_u8_Sect_a > DRV_MOT_NB_SECT ){
        while(1);
    }
    if ( l_u8_Sect_a >= DRV_MOT_SECT_5){
        l_u8_Sect_b = DRV_MOT_SECT_0;
    }else{
        l_u8_Sect_b = l_u8_Sect_a + 1;
    }
    l_drive_a = MotSectors [ l_u8_Sect_a ];
    l_drive_b = MotSectors [ l_u8_Sect_b ];


    // les deux vecteurs base entourant le secteur : Ux1 et Ux2 sont
    // commutés pendant T1 et T2, puis vecteur nul (désactivation) pendant T0
    // Changement entre 2 vecteurs adjacents : seulement une broche !
    // si on veut champ tournant constant, il faut rajouter le vecteur nul !



    // ------------------------------------------------------------
    // TEST : champ tournant non constant, donc pas de vecteur nul
    // calculer le rapport de puissance à appliquer aux deux configurations
    // pour obtenir l'angle voulu -> on aura le rapport cyclique à appliquer
    l_u16_Angle_a_Deg = (uint16) l_u8_Sect_a * (uint16) DRV_MOT_ANGLE_BASE_VECT_DEG ;

    l_f_Angle_Rel_Deg = (uint16)li_angle_Deg - l_u16_Angle_a_Deg;


    l_f_temp = l_f_Angle_Rel_Deg / ( 0.01 * DRV_MOT_ANGLE_BASE_VECT_DEG );



l_u8_tmp = (uint8)l_f_temp;
l_u8_tmp2 = 100 - l_u8_tmp ;




    g_val_pwm_u = 200;
    g_val_pwm_v = 200;
    g_val_pwm_w = 200;

    if ( l_drive_a.drv_u == l_drive_b.drv_u ){
        if ( DRV_MOT_VDD == l_drive_a.drv_u ){g_val_pwm_u = 100;}else{g_val_pwm_u = 0;}
    }
    if ( l_drive_a.drv_v == l_drive_b.drv_v ){
       if ( DRV_MOT_VDD == l_drive_a.drv_v ){g_val_pwm_v = 100;}else{g_val_pwm_v = 0;}
    }
    if ( l_drive_a.drv_w == l_drive_b.drv_w ){
       if ( DRV_MOT_VDD == l_drive_a.drv_w ){g_val_pwm_w = 100;}else{g_val_pwm_w = 0;}
    }

    if ( 200 == g_val_pwm_u )
    {
        if ( DRV_MOT_VDD == l_drive_b.drv_u ){
            g_val_pwm_u = (uint8)( l_f_temp );
        }else{
            g_val_pwm_u = (uint8)( l_u8_tmp2 );
        }
        return;
    }
    if ( 200 == g_val_pwm_v ){
        if ( DRV_MOT_VDD == l_drive_b.drv_v ){
            g_val_pwm_v = (uint8)( l_f_temp );
        }else{
            g_val_pwm_v = (uint8)( l_u8_tmp2 );
        }
        return;
    }
    if ( 200 == g_val_pwm_w ){
        if ( DRV_MOT_VDD == l_drive_b.drv_w ){
            g_val_pwm_w = (uint8)( l_f_temp );
        }else{
            g_val_pwm_w = (uint8)( l_u8_tmp2 );
        }
        return;
    }



}

void   CalcDrive ( float24 li_angle_Deg ){
    DRV_MOT_DRIVE_TYPE  l_drive_a, l_drive_b;
    static uint8   l_u8_Sect_a;
    static uint8   l_u8_Sect_b;
    static float24 l_f_Angle_a_Deg;
    static uint16 l_u16_Angle_a_Deg;
    static float24 l_f_temp;
    uint8   l_u8_tmp, l_u8_tmp2;
    //static float24 l_f_Angle_b_Deg;
    static float24 l_f_Angle_Rel_Deg;
    static float24 l_f_RapportDrive_b_a;

    // -----------------------------------------------------------
    // suivant l'angle, prendre les deux configurations de pilotage
    // donc déterminer les deux secteurs correspondants
    //l_u8_Sect_a = (uint8)(li_angle_Deg / DRV_MOT_ANGLE_BASE_VECT_DEG);
    l_u8_Sect_a = (uint8)( (uint16)li_angle_Deg / (uint16)DRV_MOT_ANGLE_BASE_VECT_DEG);

    // si erreur alors ....
    if ( l_u8_Sect_a > DRV_MOT_NB_SECT ){
        while(1);
    }
    if ( l_u8_Sect_a >= DRV_MOT_SECT_5){
        l_u8_Sect_b = DRV_MOT_SECT_0;
    }else{
        l_u8_Sect_b = l_u8_Sect_a + 1;
    }
    l_drive_a = MotSectors [ l_u8_Sect_a ];
    l_drive_b = MotSectors [ l_u8_Sect_b ];


    // les deux vecteurs base entourant le secteur : Ux1 et Ux2 sont
    // commutés pendant T1 et T2, puis vecteur nul (désactivation) pendant T0
    // Changement entre 2 vecteurs adjacents : seulement une broche !
    // si on veut champ tournant constant, il faut rajouter le vecteur nul !



    // ------------------------------------------------------------
    // TEST : champ tournant non constant, donc pas de vecteur nul
    // calculer le rapport de puissance à appliquer aux deux configurations
    // pour obtenir l'angle voulu -> on aura le rapport cyclique à appliquer
    l_u16_Angle_a_Deg = (uint16) l_u8_Sect_a * (uint16) DRV_MOT_ANGLE_BASE_VECT_DEG ;

    l_f_Angle_Rel_Deg = (uint16)li_angle_Deg - l_u16_Angle_a_Deg;
  

    l_f_temp = l_f_Angle_Rel_Deg / ( 0.01 * DRV_MOT_ANGLE_BASE_VECT_DEG );



l_u8_tmp = (uint8)l_f_temp;
l_u8_tmp2 = 100 - l_u8_tmp ;




    g_val_pwm_u = 200;
    g_val_pwm_v = 200;
    g_val_pwm_w = 200;

    if ( l_drive_a.drv_u == l_drive_b.drv_u ){
        if ( DRV_MOT_VDD == l_drive_a.drv_u ){g_val_pwm_u = 100;}else{g_val_pwm_u = 0;}
    }
    if ( l_drive_a.drv_v == l_drive_b.drv_v ){
       if ( DRV_MOT_VDD == l_drive_a.drv_v ){g_val_pwm_v = 100;}else{g_val_pwm_v = 0;}
    }
    if ( l_drive_a.drv_w == l_drive_b.drv_w ){
       if ( DRV_MOT_VDD == l_drive_a.drv_w ){g_val_pwm_w = 100;}else{g_val_pwm_w = 0;}
    }

    if ( 200 == g_val_pwm_u )
    {
        if ( DRV_MOT_VDD == l_drive_b.drv_u ){
            g_val_pwm_u = (uint8)( l_f_temp );
        }else{
            g_val_pwm_u = (uint8)( l_u8_tmp2 );
        }
        return;
    }
    if ( 200 == g_val_pwm_v ){
        if ( DRV_MOT_VDD == l_drive_b.drv_v ){
            g_val_pwm_v = (uint8)( l_f_temp );
        }else{
            g_val_pwm_v = (uint8)( l_u8_tmp2 );
        }
        return;
    }
    if ( 200 == g_val_pwm_w ){
        if ( DRV_MOT_VDD == l_drive_b.drv_w ){
            g_val_pwm_w = (uint8)( l_f_temp );
        }else{
            g_val_pwm_w = (uint8)( l_u8_tmp2 );
        }
        return;
    }



}



void DRV_MOT_PilotagePWM_Soft ( void ){
    static uint8   l_cpt_pwm_u = 0;
    static uint8   l_cpt_pwm_v = 0;
    static uint8   l_cpt_pwm_w = 0;




    if ( g_val_pwm_u > l_cpt_pwm_u){  DRV_MOT_BASIC_PIN_U = 0; }else{ DRV_MOT_BASIC_PIN_U = 1;   }
    if ( g_val_pwm_v > l_cpt_pwm_v){  DRV_MOT_BASIC_PIN_V = 0; }else{ DRV_MOT_BASIC_PIN_V = 1;   }
    if ( g_val_pwm_w > l_cpt_pwm_w){  DRV_MOT_BASIC_PIN_W = 0; }else{ DRV_MOT_BASIC_PIN_W = 1;   }

    if ( l_cpt_pwm_u > 99){l_cpt_pwm_u = 0; } else{ l_cpt_pwm_u++; }
    if ( l_cpt_pwm_v > 99){l_cpt_pwm_v = 0; } else{ l_cpt_pwm_v++; }
    if ( l_cpt_pwm_w > 99){l_cpt_pwm_w = 0; } else{ l_cpt_pwm_w++; }
    
}



void DRV_MOT_Pilotage_PWM_Init ( void ){
    DRV_MOT_BASIC_PIN_U = 0;
    DRV_MOT_BASIC_PIN_V = 0;
    DRV_MOT_BASIC_PIN_W = 0;

    DRV_MOT_BASIC_PIN_U_TRIS = 0;
    DRV_MOT_BASIC_PIN_V_TRIS = 0;
    DRV_MOT_BASIC_PIN_W_TRIS = 0;

}


//#define TMR_VAL     (0xE17B)
#define TMR_VAL     (0xFEAE)

uint16 pause ( uint16 li_val );
uint16 pause2 ( uint16 li_val );


#define _XTAL_FREQ 10000000
#define USE_AND_MASKS

int main(int argc, char** argv) {
    uint16  l_cptUpdate = 0x0;
    uint8   l_cpt_pwm = 0;
    float24 l_float = 0;
    uint16  l_angle_deg = 0;
    unsigned char Timer0Config;
    
    ADCON1 = 0xff;
    CMCON = 0x07;


    BOARD_LED_D1_PIN = 1;
    BOARD_LED_D1_TRIS_PIN = 0;

    l_float = 0;

    DRV_MOT_Pilotage_PWM_Init ();


    //Timer0Config = TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_8 ;
    //OpenTimer0(Timer0Config);
    //WriteTimer0(TMR_VAL); //Please use HEX. Decimal don't work
    CalcDrive ( 0.0 );


    T0CON = 0x08 | 0x02;
    INTCONbits.TMR0IF = 0; //reset Interrupt Flag
    
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
    
    INTCONbits.TMR0IF = 0;
    INTCONbits.GIE = 1;

    //ei();     // This is like fliping the master switch to enable interrupt


    do{

     
        //DRV_MOT_PilotageBasique2 ();
        if ( l_cptUpdate > 0x8000)
        {
            l_cptUpdate = 0;


            CalcDrive ( l_float);
            
            
            l_float+= 0.2;
            if ( l_float > 359.0){
                l_float = 0;
            }
            
      
        }else{
            l_cptUpdate++;
        }
    

    }while(1);

    return (EXIT_SUCCESS);
}



uint16 pause ( uint16 li_val ){
    static uint16  l_tempo_1;

    if ( l_tempo_1<li_val ){
        l_tempo_1++;
    }else{
        l_tempo_1 = 0;

    }
    return l_tempo_1;

}

uint16 pause2 ( uint16 li_val ){
    static uint16  l_tempo_2;

    if ( l_tempo_2<li_val ){
        l_tempo_2++;
    }else{
        l_tempo_2 = 0;

    }
    return l_tempo_2;

}



void interrupt  fct_it ( void ){


    if(INTCONbits.TMR0IF == 1)
    {

        INTCONbits.TMR0IF = 0;
        WriteTimer0(TMR_VAL); //Please use HEX. Decimal don't work
        DRV_MOT_PilotagePWM_Soft ();
    }


}



