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
	//#include "p18f8722.h"
        //#include "p18f6527.h"

/*
	#define HWI_ROM_LOCATE			rom
	#define HWI_ROM_NEAR_LOCATE		rom near
	#define HWI_FAR_ROM_PTR			far rom
	#define HWI_FAR_RAM_PTR			far ram
*/
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

	
#endif
