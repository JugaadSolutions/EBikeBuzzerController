#ifndef CONFIG_H
#define CONFIG_H

/*
*------------------------------------------------------------------------------
* config.h
*
*/

#define NO_OF_DIGITS 6


#define DEVICE_ADDRESS			0xFA


//#define __FACTORY_CONFIGURATION__

//#define TEST_LCD

//DIGIT DISPLAY TEST
#define __DISPLAY_TEST__
#define __COMMON_CATHODE__

//TIMER Configuration
#define TIMESTAMP_DURATION 		(200)


//USART Configuration
#define ACTIVE_USARTS		1

//#define __ADC_COUNT_DISPLAY__


//COM module configuration
#define __BCC_XOR__
#define __RESPONSE_ENABLED__
//#define __LOOP_BACK__
#define BROADCAST_ADDRESS		0xFF
#define CMD_SOP	0xAA
#define CMD_EOP 0xBB
#define RESP_SOP	0xCC
#define RESP_EOP	0xDD


enum
{
	CMD_PACKET_SIZE = 30,
	RESP_PACKET_SIZE = 30
};

#define 	RX_PACKET_SIZE		(64)	
#define 	TX_PACKET_SIZE		(64)


/*
*----------------------------------------
*	Keypad Configurations
*----------------------------------------
*/
//#define __FACTORY_CONFIGURATION__
//#define __SIMULATION__
//#define INPUT_SIMULATION

#define MAX_MSG 7
#define DISPLAY_REFRESH_PERIOD	2	//period of refresh in milliseconds



enum
{
	MAX_INPUT_CHARS = 32,
	MAX_KEYS = 16,
	MAX_CHAR_PER_KEY = 4,
	MIN_KEYPRESS_DURATION = 20 

	
};

/*
*----------------------------------------
*	APP Configurations
*----------------------------------------
*/

#define	END_OF_LINE		 '\n'
#define	NO_OF_LINES		 (0X04)
#define AVERAGE_INTERVAL (10)    
#define BUZZER_TIMEOUT   (5) //1 sec

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

#endif
/*
*  End of config.h
*/



