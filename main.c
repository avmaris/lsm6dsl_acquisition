#include "driverlib.h"
#include "leddiod.h"
#include "board.h"
#include "sdcard/diskio.h"
#include "sdcard/pff.h"
#include <stdio.h>
#include "timeradc.h"
#include "termo.h"
#include "lsm6dsl/lsm6dsl.h"

void dataManage();

typedef struct dataAquiStr{
	struct gyroAccelStr gaData;
	uint16_t temperature;
};

uint16_t acquDataPoi = 0;
#pragma SET_DATA_SECTION("xl_g_data")
const uint8_t accgtBuf[DATA_STORAGE_SIZE];
#pragma SET_DATA_SECTION()
struct dataAquiStr *acquisitionData =
		(struct dataAquiStr *)accgtBuf;

char system_init(); // System initializing
void sdCardHandle();
unsigned long timerClick = 0;

int main(void) {

    system_init();

    sdCardHandle();

    while(1){
    	 __bis_SR_register(LPM0_bits + GIE);
    	 __no_operation();
    };
    return (0);
}

/*
 * System initializing
 * */
char system_init(){
	int j;
    WDT_A_hold(WDT_A_BASE);

    WDTCTL = WDTPW + WDTHOLD; // Disable Watchdog
    _EINT(); // enable interrupts
    printf("Initialization");

    ledInit(LED1_PORT, LED1_PIN); //Diode initialize


    // Configure Pins for LFXIN
    //Set PJ.4 and PJ.5 as Primary Module Function Input.
    /*

//TODO : put pins to board // >>
     * Select Port J
     * Set Pin 4, 5 to input Primary Module Function, (LFXIN).
     */

    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_PJ,
        GPIO_PIN4 + GPIO_PIN5,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    //Set external frequency for XT1
    CS_setExternalClockSource(32768, 0);
    //Set DCO frequency to max DCO setting
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_3);
    //Select XT1 as the clock source for ACLK with no frequency divider
    CS_initClockSignal(CS_ACLK, CS_XT1CLK_SELECT, CS_CLOCK_DIVIDER_1);
    //Start XT1 with no time out
    CS_turnOnXT1(CS_XT1_DRIVE_0);


    blink(10);
    ledStateSet(OFF);
    termoInit();
    ADCInit();

//    timerInit();

    sdInit(SPI_CS_PORT, SPI_CS_PIN, SD_EN_PORT, SD_EN_PIN); // SD-Card initialize
    lsm6dslInit(); // Accelerometer initialize
    battSignaliningInit();
	return OK_RETURN;
}
void sdCardHandle(){
	FATFS fs; /* File system object */
	FRESULT res;
//	char Line[128];
//	int j;
//	WORD s1;
	res = disk_initilize___();
//	if(res == FR_OK)
//	{
//		disk_readp(buf, sect, 510, 2);
	res = pf_mount(&fs);
//	if(res == FR_OK)
//	{
//		res = pf_open("test.txt");
//		if(res == FR_OK)
//		{
//			res = pf_read(Line, sizeof(Line), &s1);
//			if(res == FR_OK){
//			    for(j = 10; j; j--){
//			    	__delay_cycles(62500);
//					ledStateSet(CHANGE);
//			    }
//			    ledStateSet(OFF);
//			}
//			else
//				ledStateSet(ON);
//		}/**/
//	}
////	else
////			ledStateSet(ON);
//
//	}
//	else
//		ledStateSet(ON);
}

void printG_XK(struct dataAquiStr* datAq, int size){
	int i;
	for(i = 0; i < size; i++){
/*		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
					acquisitionData[i].gaData.accel.valX,
					acquisitionData[i].gaData.accel.valY,
					acquisitionData[i].gaData.accel.valZ,
					acquisitionData[i].gaData.gyro.valX,
					acquisitionData[i].gaData.gyro.valY,
					acquisitionData[i].gaData.gyro.valZ,
					acquisitionData[i].temperature);*/
		printf("t%d\n", datAq[i].temperature);
	}
}

void dataManage(){
	struct xyzDatStr *acs;
	struct xyzDatStr *grs;
	int dataSize;
	acs = &(acquisitionData->gaData.accel);
	grs = &(acquisitionData->gaData.gyro);
	lsm6dslGXL_Read(acs, grs);
	acquisitionData->temperature = readT();
	acquisitionData++;

	int bytesInBuf = (int) (acquisitionData) - (int)accgtBuf;
	int nextLen = (int)(acquisitionData + 1)-(int)accgtBuf;

	if (nextLen > DATA_STORAGE_SIZE)
	{
		dataSize = (bytesInBuf) / sizeof(struct dataAquiStr);
		//acquDataPoi = 0;
		acquisitionData = (struct dataAquiStr *)accgtBuf;
		printG_XK(acquisitionData, dataSize);
		//TODO : write data to sd;
	}

}
//******************************************************************************
//
//This is the TIMER1_A3 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER1_A0_VECTOR)))
#endif
void TIMER1_A0_ISR(void)
{
    uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    compVal += COMPARE_VALUE;
    timerClick++;

  	 /* 40ms */
  	 if((timerClick&0x1F) == 25) // enter each ~ 1s
  	 {
  	 	 if(getVbat() < VBAT_TH)
  	 		battSignalingOn();
  	 	 else
  	 		battSignalingOff();
  		 printf("V%d\n",getVbat());
  	 }

  	 /* 40ms */
  	 if((timerClick&0x3) == 3) // enter each ~ 8.3Hz
  	 {
  		dataManage();
  	 }


    //Add Offset to CCR0
    Timer_A_setCompareValue(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0,compVal);
}
