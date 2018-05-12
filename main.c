#include "msp.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    P1->SEL0 |= BIT2;
    P1->OUT &= ~BIT0;
    P5->SEL1 |= BIT4;
    P5->SEL0 |= BIT4;


	P1-> SEL0 |= BIT2;
	EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
	EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SSEL__SMCLK;

	EUSCI_A0->BRW = 1;
	EUSCI_A0->MCTLW = (10 << EUSCI_A_MCTLW_BRF_OFS) | EUSCI_A_MCTLW_OS16;
	EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;
	EUSCI_A0->IFG &= ~EUSCI_A_CTLW0_SWRST;
	EUSCI_A0-> IE |= EUSCI_A_IE_RXIE;

	NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);

	__enable_irq();

    ADC14-> CTL1 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_3;
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1;
    ADC14->IER0 |= ADC14_IER0_IE0;

    while(1){
        int i;
        for(i=20,000; i>0; i--);
        ADC14->CTL0 |= (ADC14_CTL0_ENC | ADC14_CTL0_SC);
    }
}

void EUSCIA_IRQHandler(void){
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A0->TXBUF = EUSCI_A0->RXBUF;
    }
}

void ADC14_IRQHandler(void){
    if(ADC14->MEM[0] >= 0x20000)
      P1->OUT |= BIT0;
    else
      P1->OUT &= ~BIT0;
    }

