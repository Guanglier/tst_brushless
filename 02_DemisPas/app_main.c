/* 
 * File:   app_main.c
 * Author: user
 *
 * Created on 22 juin 2013, 10:36
 */

#include <stdio.h>
#include <stdlib.h>
#include "machine.h"

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



#define PIN_HIGH    1
#define PIN_LOW     0

typedef struct{
    uint8   etat_phase_u;
    uint8   etat_phase_v;
    uint8   etat_phase_w;
}DRV_MOT_BASIC_DRIVE_TYPE;

DRV_MOT_BASIC_DRIVE_TYPE    G_BasicDrv[6]=
{
    { PIN_HIGH,     PIN_LOW,    PIN_LOW},       // u4
    { PIN_HIGH,     PIN_HIGH,   PIN_LOW},       // u6
    { PIN_LOW,      PIN_HIGH,   PIN_LOW},       // u2
    { PIN_LOW,      PIN_HIGH,   PIN_HIGH},      // u3
    { PIN_LOW,      PIN_LOW,    PIN_HIGH},      // u4
    { PIN_HIGH,     PIN_LOW,    PIN_HIGH}       // u5
};



void DRV_MOT_PilotageBasique ( void ){
    static uint8    l_etat=0;
    uint8    l_etat_u= G_BasicDrv[l_etat].etat_phase_u;
    uint8    l_etat_v= G_BasicDrv[l_etat].etat_phase_v;
    uint8    l_etat_w= G_BasicDrv[l_etat].etat_phase_w;
    //switch ( )
    DRV_MOT_BASIC_PIN_U = l_etat_u;
    DRV_MOT_BASIC_PIN_V = l_etat_v;
    DRV_MOT_BASIC_PIN_W = l_etat_w;

    l_etat++;
    if ( l_etat > 5) { l_etat = 0;}
}

void DRV_MOT_PilotageBasiqueInit ( void ){
    DRV_MOT_BASIC_PIN_U = 0;
    DRV_MOT_BASIC_PIN_V = 0;
    DRV_MOT_BASIC_PIN_W = 0;

    DRV_MOT_BASIC_PIN_U_TRIS = 0;
    DRV_MOT_BASIC_PIN_V_TRIS = 0;
    DRV_MOT_BASIC_PIN_W_TRIS = 0;

}

// pilotage en mode demi pas
void DRV_MOT_PilotageBasique2 (  ){
    static uint8    l_etat=0, l_etat_1, l_etat_2=0;
    static uint8    l_alt=0;
    static uint16  l_vitesse = 0x0;
    static uint16  l_vitesse_base= 0x4000;
    static uint16  l_tempo = 0x0;
    static uint16  l_pwm = 0x0;
    static uint8    l_etat_u;
    static uint8    l_etat_v;
    static uint8    l_etat_w;


    /*
    if ( l_tempo > 0x1000 ){
        if ( l_vitesse_base > 0x70){
            l_vitesse_base -= 1;
        }
        l_tempo = 0;
    }else{
        l_tempo++;
    }
    */

    if ( l_vitesse > l_vitesse_base )
    {
        switch ( l_etat ){

            case 0: l_etat_1=0; l_etat_2=0;  break;
            case 1: l_etat_1=1; l_etat_2=0;  break;
            case 2: l_etat_1=1; l_etat_2=1;  break;
            case 3: l_etat_1=2; l_etat_2=1;  break;
            case 4: l_etat_1=2; l_etat_2=2;  break;
            case 5: l_etat_1=3; l_etat_2=2;  break;
            case 6: l_etat_1=3; l_etat_2=3;  break;
            case 7: l_etat_1=4; l_etat_2=3;  break;
            case 8: l_etat_1=4; l_etat_2=4;  break;
            case 9: l_etat_1=5; l_etat_2=4;  break;
            case 10: l_etat_1=5; l_etat_2=5;  break;
            case 11: l_etat_1=0; l_etat_2=5;  break;
        };
        l_etat++;
        if ( l_etat > 11){ l_etat = 0; }
        l_vitesse = 0;
    }else{
        l_vitesse++;
    }

    
    if ( l_pwm > 0x4 )
    {
        if ( l_alt == 0)
        {
            l_etat_u= G_BasicDrv[l_etat_1].etat_phase_u;
            l_etat_v= G_BasicDrv[l_etat_1].etat_phase_v;
            l_etat_w= G_BasicDrv[l_etat_1].etat_phase_w;
            l_alt = 1;
        }else{
            l_etat_u= G_BasicDrv[l_etat_2].etat_phase_u;
            l_etat_v= G_BasicDrv[l_etat_2].etat_phase_v;
            l_etat_w= G_BasicDrv[l_etat_2].etat_phase_w;
            l_alt = 0;
        }
        l_pwm = 0;
    }else{
        l_pwm++;
    }


    DRV_MOT_BASIC_PIN_U = l_etat_u;
    DRV_MOT_BASIC_PIN_V = l_etat_v;
    DRV_MOT_BASIC_PIN_W = l_etat_w;


}

void DRV_MOT_PilotageBasique2Init ( void ){
    DRV_MOT_BASIC_PIN_U = 0;
    DRV_MOT_BASIC_PIN_V = 0;
    DRV_MOT_BASIC_PIN_W = 0;

    DRV_MOT_BASIC_PIN_U_TRIS = 0;
    DRV_MOT_BASIC_PIN_V_TRIS = 0;
    DRV_MOT_BASIC_PIN_W_TRIS = 0;

}




uint16 pause ( uint16 li_val );
uint16 pause2 ( uint16 li_val );



int main(int argc, char** argv) {
    uint16  l_vitesse = 0x8000;
    ADCON1 = 0xff;
    CMCON = 0x07;


    BOARD_LED_D1_PIN = 1;
    BOARD_LED_D1_TRIS_PIN = 0;

    DRV_MOT_PilotageBasiqueInit ();
    DRV_MOT_PilotageBasique2Init ( );

    do{

        /*
        if( 0 == pause (l_vitesse) ){
            //DRV_MOT_PilotageBasique ();
        }
        */
        DRV_MOT_PilotageBasique2 ();
/*
        if ( l_vitesse > 0xc0 )
        {
            if ( 0 == pause2 (0x2000) )
            {
                if ( l_vitesse > 0xC0 ){
                    l_vitesse -= 0x01;
                }
            }
        }else{
            if ( 0 == pause2 (0x4000) )
            {
                if ( l_vitesse > 0xA8 ){
                    l_vitesse -= 0x01;
                }
            }
        }
*/

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







/*
enum{
    DRV_MOT_VDD,
    DRV_MOT_0V
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
DRV_MOT_DRIVE_TYPE       MotSectors[DRV_MOT_NB_SECT]={
    { DRV_MOT_VDD ,  DRV_MOT_0V ,    DRV_MOT_0V },      // DRV_MOT_SECT_0
    { DRV_MOT_VDD ,  DRV_MOT_VDD ,   DRV_MOT_0V },      // DRV_MOT_SECT_1
    { DRV_MOT_0V ,   DRV_MOT_VDD ,   DRV_MOT_0V },
    { DRV_MOT_0V ,   DRV_MOT_VDD ,   DRV_MOT_VDD },
    { DRV_MOT_0V ,   DRV_MOT_0V ,    DRV_MOT_VDD },
    { DRV_MOT_VDD ,  DRV_MOT_0V ,    DRV_MOT_VDD },     // DRV_MOT_SECT_5
};


uint8   Drive ( float24 li_angle_Deg ){
    DRV_MOT_DRIVE_TYPE  l_drive_a, l_drive_b;
    static uint8   l_u8_Sect_a;
    static uint8   l_u8_Sect_b;
    static float24 l_f_Angle_a_Deg;
    //static float24 l_f_Angle_b_Deg;
    static float24 l_f_Angle_Rel_Deg;
    static float24 l_f_RapportDrive_b_a;

    // -----------------------------------------------------------
    // suivant l'angle, prendre les deux configurations de pilotage
    // donc déterminer les deux secteurs correspondants
    l_u8_Sect_a = (uint8)(li_angle_Deg / DRV_MOT_ANGLE_BASE_VECT_DEG);

    // si erreur alors ....
    if ( l_u8_Sect_a > DRV_MOT_NB_SECT ){
        while(1);
    }
    if ( l_u8_Sect_a >= DRV_MOT_SECT_5){
        l_u8_Sect_b = DRV_MOT_SECT_0;
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
    l_f_Angle_a_Deg = (float24) l_u8_Sect_a * DRV_MOT_ANGLE_BASE_VECT_DEG;
    //l_f_Angle_b_Deg = (float24) l_u8_Sect_b * DRV_MOT_ANGLE_BASE_VECT_DEG;

    l_f_Angle_Rel_Deg = li_angle_Deg - l_f_Angle_a_Deg;
    l_f_RapportDrive_b_a = l_f_Angle_Rel_Deg / ( DRV_MOT_ANGLE_BASE_VECT_DEG );



}
*/


