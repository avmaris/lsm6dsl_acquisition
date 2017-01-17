/*-----------------------------------------------------------------------
 /  PFF - Low level disk interface modlue include file    (C)ChaN, 2009
  * modified by Marisov A.V.
  * 	-> Added function to low level control by present Enable port.
 /-----------------------------------------------------------------------*/

#ifndef _DISKIO

#include "integer.h"
#include "driverlib.h"

/* Status of Disk Functions */
typedef BYTE DSTATUS;

/* Results of Disk Functions */
typedef enum {
    RES_OK = 0, /* 0: Function succeeded */
    RES_ERROR, /* 1: Disk error */
    RES_NOTRDY, /* 2: Not ready */
    RES_PARERR /* 3: Invalid parameter */
} DRESULT;

/*---------------------------------------*/
/* Prototypes for card control*/
void sdRelease(void);
void sdSelect(void);
void sdPowerON(void);
void sdPowerOFF(void);
uint8_t sdPowerStatus(void);
uint8_t sdSelectStatus(void);
uint8_t sdCsEnStatus(void);

void sdInit(uint8_t CS_Port, uint16_t CS_Pins,
		uint8_t SD_en_Port, uint16_t SD_en_Pins);
/*---------------------------------------*/
/* Prototypes for disk control functions */

DRESULT disk_initilize___(void);
DRESULT disk_readp(BYTE*, DWORD, WORD, WORD);
DRESULT disk_writep(const BYTE*, DWORD);

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_SDC				(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK			0x08	/* Block addressing */

#define _DISKIO
#endif
