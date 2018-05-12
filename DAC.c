/*
 * DAC.c
 *
 *  Created on: May 7, 2018
 *      Author: D
 */
#include "msp.h"

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    P1->SEL0 |= BIT2;
    P1->OUT &= ~BIT0;
    P5->SEL1 |= BIT4;
    P5->SEL0 |= BIT4;

    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
    __enable_irq();

    ADC14-> CTL1 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_3;
    ADC14->MCTL[0] |= ADC14_MCTL_INCH_1;
    ADC14->IERO |= ADC14_IER0_IEO;

    while(1){
        for(i=20,000; i>0; i--);
        ADC14->CTL0 |= ADC14_CTL0_ENC |ADC14_CTL0_SC;
    }
}

void ADC14_IRQHandler(void){
    if(ADC14->MEM[0] >= 0x20000)
        P1->OUT |= BIT0;
    else
        P1->OUT &= ~BIT0;
}

