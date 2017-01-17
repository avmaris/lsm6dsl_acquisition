/*
 * timeradc.c
 *
 *  Created on: Nov 29, 2016
 *      Author: marisov
 */


#include "driverlib.h"
#include <stdio.h>
#include "timeradc.h"
#include "board.h"

void timerInit(uint32_t period){
    //Start timer in continuous mode sourced by SMCLK
    Timer_A_initContinuousModeParam initContParam = {0};
    initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initContParam.clockSourceDivider = PREDEFINE_TA_DIV;
    initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initContParam.timerClear = TIMER_A_DO_CLEAR;
    initContParam.startTimer = false;
    Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

    //Initiaze compare mode
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);

    Timer_A_initCompareModeParam initCompParam = {0};
    initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
    initCompParam.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    initCompParam.compareValue = COMPARE_VALUE;
    Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);

}


void battSignalingOn(){
	TB2CCTL2 = OUTMOD_7;
}

void battSignalingOff(){
	TB2CCTL2 = OUTMOD_0;
}

void battSignaliningInit(){
	   GPIO_setAsPeripheralModuleFunctionOutputPin(LED1_PORT, LED1_PIN,
	            GPIO_PRIMARY_MODULE_FUNCTION);

	    Timer_B_clearTimerInterrupt(TIMER_B2_BASE);

	   // Timer_B_initContinuousModeParam
		Timer_B_initUpModeParam param = {0};
	    param.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
	    param.clockSourceDivider = BLINK_CLK_DIV;
	    param.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
	    param.timerClear = TIMER_B_DO_CLEAR;
	    param.startTimer = false;
	    param.timerPeriod = BLINK_TIMER_PERIOD;
	    //Timer_B_initContinuousMode
		Timer_B_initUpMode (TIMER_B2_BASE, &param);

	    Timer_B_startCounter(TIMER_B2_BASE, TIMER_B_UP_MODE);
	                         //TIMER_B_CONTINUOUS_MODE);
	    // PWM
	    Timer_B_initCompareModeParam initComp2Param = {0};
	    initComp2Param.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_2;
	    initComp2Param.compareInterruptEnable =TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE;
	       // TIMER_B_CAPTURECOMPARE_INTERRUPT_ENABLE;
	    initComp2Param.compareOutputMode = TIMER_B_OUTPUTMODE_RESET_SET;
	    initComp2Param.compareValue = BLINK_HI_TIME_CMP_VAL;
	    Timer_B_initCompareMode(TIMER_B2_BASE, &initComp2Param);
	    battSignalingOff();
}

void ADCInit(){

    /*
     * Vbat port configuration
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN2,
        GPIO_TERNARY_MODULE_FUNCTION);
    //
    /*

    //Initialize the ADC10 Module
    /*
     * Base Address for the ADC10 Module
     * Use internal ADC10 bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC10_B_init(ADC10_B_BASE,
                 ADC10_B_SAMPLEHOLDSOURCE_SC,
                 ADC10_B_CLOCKSOURCE_ADC10OSC,
                 ADC10_B_CLOCKDIVIDER_1);

    ADC10_B_enable(ADC10_B_BASE);

    /*
     * Base Address for the ADC10 Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC10_B_setupSamplingTimer(ADC10_B_BASE,
                               ADC10_B_CYCLEHOLD_16_CYCLES,
                               ADC10_B_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
     * Base Address for the ADC10 Module
     * Use input A1
     * Use positive reference of Internally generated Vref
     * Use negative reference of AVss
     */
    Ref_setReferenceVoltage(REF_BASE, REF_VREF2_0V);
    ADC10_B_configureMemory(ADC10_B_BASE, ADC10_B_INPUT_A14, ADC10_B_VREFPOS_INT, ADC10_B_VREFNEG_AVSS);
    ADC10_B_clearInterrupt(ADC10_B_BASE, ADC10IE0);

    //Enable Memory Buffer interrupt
    /*ADC10_B_enableInterrupt(ADC10_B_BASE,
                            ADC10IE0);*/

    //Configure internal reference
    //If ref generator busy, WAIT
    while(REF_BUSY == Ref_isRefGenBusy(REF_BASE));



}

uint32_t getVbat(){
	//ADCInit();
	//delayus(10);
	uint32_t vBat;
	vBat = 2*getV(REF_VREF1_5V, ADC10_B_INPUT_A2);
	//ADC10_B_disable(ADC10_B_BASE);
	return vBat;
}


uint32_t getV(uint16_t ref, uint8_t inputSourceSelect)
{
	uint32_t refmVolt = 0;
	uint32_t vmes = 0;
//	ADC10_B_disableConversions(uint16_t baseAddress,
//	                                bool preempt)

	switch(ref)
	{
	    case  REF_VREF1_5V: refmVolt = 1500; break;
	    case  REF_VREF2_0V: refmVolt = 2000; break;
	    case  REF_VREF2_5V: refmVolt = 2500; break;
	}
//	ADC10_B_enable(ADC10_B_BASE);
    while(ADC10_B_isBusy(ADC10_B_BASE)){};
    Ref_setReferenceVoltage(REF_BASE, ref);
    Ref_enableReferenceVoltage(REF_BASE);
    ADC10_B_configureMemory(ADC10_B_BASE, inputSourceSelect, ADC10_B_VREFPOS_INT, ADC10_B_VREFNEG_AVSS);
    ADC10_B_startConversion(ADC10_B_BASE, ADC10_B_SINGLECHANNEL);

    while(!ADC10_B_getInterruptStatus(ADC10_B_BASE, ADC10_B_IFG0)){
    	vmes++;
    	if(vmes > 100)
    		break;
    };
    vmes = (refmVolt*ADC10_B_getResults(ADC10_B_BASE))/1023;
    ADC10_B_disableConversions(ADC10_B_BASE, 0);
//    ADC10_B_disable(ADC10_B_BASE);
    return vmes;
}


