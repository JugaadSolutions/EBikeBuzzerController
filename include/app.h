#ifndef _APC_IAS_H_
#define _APC_IAS_H_


/*
*----------------------------------------------------------------------------------------------------------------
*	MACROS
*-----------------------------------------------------------------------------------------------------------------
*/

//#define __FACTORY_CONFIGURATION__


#define EERPOM_WRITE_START_ADD	 		(0X00)

#define MAX_DATA_SIZE					(20)
/*
*----------------------------------------------------------------------------------------------------------------
*	Enumerations
*-----------------------------------------------------------------------------------------------------------------
*/


extern void APP_init(void);
extern void APP_task(void);

#endif