/************************* FICHIER ENTETE *********************//**
*
* @file hwi.h
*
* @defgroup HWI Hardware interface group
*
* @brief fichier d'inclusion HWI
*
* @details
* a inclure par l'appli, permet d'acceder aux fonctions hwi
*
* Société ATERMES
****************************************************************//*

HISTORIQUE:
-----------
Version -    Date    - Auteur       - Description, N° FA, FE

 V01.00 - 13/03/2009 – Monange          - version originale.

******************************************************************/



#ifndef HWI_H
#define HWI_H

/****************************************************************************/
/** FICHIERS A EN-TETE **/
/****************************************************************************/
#include "hwi_machine.h"
#include "hwi_esnum_methods.h"
//#include "hwi_spi_methods.h"
#include "hwi_pwm_methods.h"
//#include "hwi_timers_methods.h"
//#include "hwi_interrupts_methods.h"
#include "hwi_usart_methods.h"
//#include "hwi_eeprom_methods.h"
//#include "hwi_adc_methods.h"

/****************************************************************************/
/** DEFINITION **/
/****************************************************************************/
#define HWI_ERROR	1
#define HWI_OK		0
#define OSC_TEMPS_CYCLE_NS	(uint32)(100)		//Tcy=10Mhz

/****************************************************************************/
/** MACROS **/
/****************************************************************************/

/****************************************************************************/
/** TYPEDEFS ET STRUCTURES **/
/****************************************************************************/
typedef enum {
	HWI_NO_ERROR = 2,
	HWI_BAD_CHANNEL = 3
}HWI_ERROR_TYPE;	
/****************************************************************************/
/** VARIABLES EXPORTEES **/
/****************************************************************************/

/****************************************************************************/
/** FONCTIONS EXPORTEES **/
/****************************************************************************/

#endif
