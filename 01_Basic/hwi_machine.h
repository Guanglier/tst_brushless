/************************* FICHIER ENTETE *********************//**
*
* @file hwi_machine.h
*
* @ingroup HWI
*
* @brief fichier de la plateforme
*
* @details permet de specifier toutes les caracteristiques de la cible
* 
*
* Société ATERMES
****************************************************************//*
HISTORIQUE:
-----------
Version -    Date    - Auteur       - Description, N° FA, FE

 V01.00 - 13/03/2009 – Monange          - version originale.

******************************************************************/

#ifndef HWI_MACHINE
#define HWI_MACHINE


	#include "p18f4680.h"


	#define HWI_ROM_LOCATE			
	#define HWI_ROM_NEAR_LOCATE		
	#define HWI_FAR_ROM_PTR
	#define HWI_FAR_RAM_PTR			

	#define uint8		unsigned char
	#define int8		signed char
	#define uint16		unsigned int
	#define int16		signed int
	#define uint24		unsigned short long
	#define int24		signed short long
	#define int32		signed long
	#define uint32		unsigned long
	#define F32			float


 

    #define uint8       unsigned char
    #define uint16      unsigned short



    #define BOARD_LED_D1_PIN        PORTDbits.RD0
    #define BOARD_LED_D2_PIN        PORTDbits.RD1
    #define BOARD_LED_D3_PIN        PORTDbits.RD2
    #define BOARD_LED_D4_PIN        PORTDbits.RD3
    #define BOARD_LED_D5_PIN        PORTDbits.RD4
    #define BOARD_LED_D6_PIN        PORTDbits.RD5
    #define BOARD_LED_D7_PIN        PORTDbits.RD6

    #define BOARD_LED_D1_TRIS_PIN        TRISDbits.TRISD0
    #define BOARD_LED_D2_TRIS_PIN        TRISDbits.TRISD1
    #define BOARD_LED_D3_TRIS_PIN        TRISDbits.TRISD2
    #define BOARD_LED_D4_TRIS_PIN        TRISDbits.TRISD3
    #define BOARD_LED_D5_TRIS_PIN        TRISDbits.TRISD4
    #define BOARD_LED_D6_TRIS_PIN        TRISDbits.TRISD5
    #define BOARD_LED_D7_TRIS_PIN        TRISDbits.TRISD6
    #define BOARD_LED_D8_TRIS_PIN        TRISDbits.TRISD7



	
#endif
