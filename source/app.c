
/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/
#include "config.h"
#include "board.h"
#include "timer.h"
#include "app.h"
#include "lcd.h"
#include "string.h"
#include "eep.h"
#include "uart.h"


const rom UINT8 *MSG[]=
		{"VG ",
		"AVG:",
		};

/*
*------------------------------------------------------------------------------
* APP Structures
* Data format: (STIRNG)
* LINE - 1: If contains BAL ignore data
* LINE - 2:
* LINE - 3:
* LINE - 4: +(CURRENT) +(VOLTAGE)
*
*------------------------------------------------------------------------------
*/
typedef struct _APP
{

	UINT32 previousCount;

	//used to store cumulative voltage
	UINT32 cumulativeVtg;

	//used to store actual voltage from uart
	UINT24 voltage;

	//used to store average of voltage after some interval
	UINT24 average;

	//used to store low voltage limit
	UINT24 lowVoltageLimit;

	//used to store critical voltage limit
	UINT24 critVoltageLimit;

	//used to store the data to received form UART
	char data[MAX_DATA_SIZE*4];

	//used to store number lines received from uart
	UINT8 lineCount;

	//used to increment the data buffer
	UINT8 dataIndex;

	//used as a flag: set after uart data is processed
	UINT8 dataProcessed;

	//counter used to do average of incoming voltage 
	UINT8 dataReceivedCount;

}APP;	

/*
*------------------------------------------------------------------------------
* Variables
*------------------------------------------------------------------------------
*/

#pragma idata APP_DATA
APP app = {0};
#pragma idata

/*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

void processData ( void );

void setMsg ( UINT8 msgIndex );
void handleBuzzer (void);

/*
*------------------------------------------------------------------------------
* void IAS-init( void )
*------------------------------------------------------------------------------
*/

void APP_init ( void )
{

	UINT8 i = 0;

	//read limits from EEPROM
	for ( i = 0; i < sizeof( app.lowVoltageLimit ); i++ )
	{
		app.lowVoltageLimit <<= 8;
		app.lowVoltageLimit |= Read_b_eep( EERPOM_WRITE_START_ADD + i );
		Busy_eep();
	}

	for ( i = 0; i < sizeof( app.critVoltageLimit ); i++ )
	{
		app.critVoltageLimit <<= 8;
		app.critVoltageLimit |= Read_b_eep( sizeof( app.lowVoltageLimit )+i );
		Busy_eep();
	}

	app.lineCount = 0;

	app.dataProcessed = TRUE;
	app.average = ~0;

	//initalize lcd

	LCD_setBackSpace('\x08');	//Indicates LCD driver "\x08" is the symbol for backspace

	setMsg( 0 );
}





/*
*------------------------------------------------------------------------------
* void IAS-task(void)
*------------------------------------------------------------------------------
*/
void APP_task ( void )
{
	char *pToString = 0;
	const char string[] = "BAL";
	UINT8 data = 0;	

	if ( app.dataReceivedCount >= AVERAGE_INTERVAL )
	{
		app.average = ( app.cumulativeVtg/AVERAGE_INTERVAL );
		app.cumulativeVtg = 0;
		app.dataReceivedCount = 0;
		app.previousCount = GetAppTime();
	}


	handleBuzzer();

	//store the data into buffer and count the number of lines
	if ( app.dataProcessed == TRUE )
	{
		if ( UART_hasData() )
		{
			data = UART_read();
	
			if ( data == END_OF_LINE )
				app.lineCount++;
			else
				app.data[app.dataIndex++] = data;
	
		}
	}

	//check for all line received
	if ( app.lineCount == NO_OF_LINES )
	{
		//set the flag to zero to hold the uart data untill data is processed
		app.dataProcessed = FALSE;

		pToString = strstr( app.data, string );

		if ( pToString == 0 )
			processData();

		app.lineCount = 0;
		app.dataIndex = 0;

		app.dataProcessed = TRUE;
	}
		
		
}

/*
*------------------------------------------------------------------------------
* void processData( void )
*------------------------------------------------------------------------------
*/

void processData ( void )
{
	UINT8 j = 0, i = 0;
	
	//Used to store the address of the string 
	char *cPtr = 0;

	//Used as null pointer to use in 'strtok' function
	char *cNullPtr = 0;

	//Used as delimiter in 'strtok' function
	const char delim[] = "+";

	//Used to store store starting address of parsed string
	UINT8 *token[2] = {0};

	//store the pointer 
	cPtr = strtok( app.data, delim );
	while ( (cPtr = strtok( cNullPtr, delim )) != 0 )
	{
		
		token[j++] = ( UINT8* )cPtr;
	}
	
	j = 0;
	app.voltage = 0;
	//store the voltage into buffer
	while ( token[1][j] != '\0' )
	{
		if( *(token[1]+j) != '.' )
		{
			app.voltage *= 10;
			app.voltage += *(token[1]+j) - '0';

			//app.voltageBuf[i++] = *(token[1]+j);
		}

		j++;
	}
	app.cumulativeVtg += app.voltage;

	app.dataReceivedCount++;
}






void setMsg ( UINT8 msgIndex )
{
	UINT8 i;
	UINT8 msg[20];

	LCD_clear();
 	strcpypgm2ram(msg,MSG[msgIndex]);

	i = 0;
	while ( msg[i] != '\0' )
	{
		LCD_putChar(msg[i]);
		i++;
	}

}


/*
*------------------------------------------------------------------------------
* void handleBuzzer(void)
*------------------------------------------------------------------------------
*/

void handleBuzzer ( void )
{
	static BOOL buzzerOn = 1;

	if ( (app.average < app.critVoltageLimit) )
		BUZZER = 1;
	else if ( (app.average < app.lowVoltageLimit) )
	{
		if( buzzerOn == 1 )
		{
			BUZZER = 1;
			if ( (GetAppTime() - app.previousCount) >= BUZZER_TIMEOUT )
			{
				app.previousCount = GetAppTime();
				buzzerOn = 0;
			}

		}
		else if ( buzzerOn == 0 )
		{
			BUZZER = 0;
			if ( (GetAppTime() - app.previousCount) >= BUZZER_TIMEOUT )
			{
				app.previousCount = GetAppTime();
				buzzerOn = 1;
			}

		}
	}
	else
		BUZZER = 0;
}