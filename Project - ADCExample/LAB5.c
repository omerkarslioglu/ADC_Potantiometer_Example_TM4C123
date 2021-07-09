#include <stdint.h>
#include <stdlib.h>
#include <tm4c123gh6pm.h>

void init_Clock(void);
void SysClock_80MHz (void);
void ADC0_init(void);
unsigned long voltageRead (void);

volatile static uint32_t adcResult = 0 ;
volatile static float    voltage   = 0 ;

int main (void)
{ 
	SysClock_80MHz();
	init_Clock();
	ADC0_init();
	
	while(1)
	{
		adcResult=voltageRead();
		voltage=adcResult*3.3/4095 ;  // Convert ADC value to voltage
	}
return 0;
}


// Definition : Initialize System Clock Function
// Exp. :
// Fout = N x Fref
// We must configure RCC and RCC2 register TO TAKE 80 MHz
void SysClock_80MHz (void)
{	    
	SYSCTL_RCC2_R |=   (0x80000800);                             
	// 31th bit must be equal to 1 (for enabling) and 11th bit must be equal to 1 (for bypass)
	
	SYSCTL_RCC_R   = (SYSCTL_RCC_R & ~ (0X000007C0))+(0x15<<6);	 
	// Clearing RCC[10:6] then setting XTAL=RCC[10:6]=15 (16 MHz External Osc.)
	
	SYSCTL_RCC2_R &=  ~(0x00000070); 										         // Clearing RCC2[6:4] OSCSRRC2 =000=MOSC (Configuration for main oscillator source)
	SYSCTL_RCC2_R &=  ~(0x00002000); 									  				 // Clearing RCC2[13]=PERDN=0 (Clearing PWRDN to active PLL)
	SYSCTL_RCC2_R |=   (0x40000000); 							  						 // Setting RCC2[30]=1 (selecting DIV400=400MHz)
	SYSCTL_RCC2_R  = (SYSCTL_RCC2_R & ~(0x1FC00000))+(0x04<<22); // Gives N=4 , N+1=5 , 400/5=80 (Setting system clock divider RCC[28:22])
	while ((SYSCTL_RIS_R & 0x00000040)==0){} 										 // Wait for PLL to lock by polling PLLRIS 
	SYSCTL_RCC2_R &=  ~(0x00000800); 									           //RCC2[11]=0 (Setting BYPASS to 0, select PLL as the source of system clock)
}

// Definition : Initialize ADC0
void init_Clock(void)
{
	SYSCTL_RCGC2_R |= 0x10; 							// to initialize clock to Port E 
	while(!( SYSCTL_PRGPIO_R & 0x10)){} 	// waiting to enable clk
}

// Definition : Initialize ADC0
void ADC0_init(void)
{
  GPIO_PORTE_DIR_R 		= 0x00;     					// PE3 input
  GPIO_PORTE_AFSEL_R 	= 0x08;     					// Enable alternate function for PE3  
  GPIO_PORTE_DEN_R 		= 0x00;     					// Disable "digital enable" for PE3 , 
	                                          // because I am gonna use periph. of adc . 
  GPIO_PORTE_AMSEL_R 	= 0x08;								// Make PE3 analog 
	
	SYSCTL_RCGC0_R = 0x00010000;   						// Activate ADC0 (16th bit corresponds to ADC0)
	
  SYSCTL_RCGC0_R &= ~(0x00000300);  				// Set sampling rate to 125K
																						// 9th and 8th bits correspond to ADC0 sampling rate
																						// 11th and 10th bits correspond to ADC1 sampling rate
																						// 00 means 125K
																						// 01 means 250K
																						// 10 means 500K
																						// 11 means 1M
		
  // If required by the application , reconfigure the ssample sequencer prioties in the ADCSSPRI register .
	// The default configuration has Sample Sequencer 0 with the highest priority and Sample Sequencer 3 as the lowest priority.
	//ADC0_SSPRI_R  = 0x00000123;          		// Set sequencer 3 to highest priority (00)
																						// 13th and 12th bits correspond to SS3
	
	// Configutation of Sample Sequencer 
	// I must be sure that the sample sequencer is disabled by clearing the corresponding ASEn bit in the ADCACTSS register .
  ADC0_ACTSS_R  &= ~(1<<3);     						// Disable sample sequencer 3 before changes (0)

																						
	// I configured the trigger event for th sample sequencer in the ADCEMUX register .
  // SS3 trigger event = continously sample , continous means that no trigger is required , the ADC will start as soon as it is enabled .
  ADC0_EMUX_R   &= (0xF<<12);     					// Set trigger for SS3
																					
																						
	// When using a PWM generator as the trigger source,I can use the ADC Trigger Source Select (ADCTSSEL) register to
	// specify in which PWM module the generator is located. The default register reset selects PWM module 0 for all generators .
																	
	// For each sample in the sample sequence ,  
	// I must configure the corresponding input source in the ADCSSMUXn register .
  ADC0_SSMUX3_R &= 0xFFFFFFF0; 							// I choosed Ain0 (PE3) as input source (0) (page 801 on Datasheet)
	
  ADC0_SSCTL3_R  = 0x0006;         					// Choose control bits
																						// 3rd bit temperature sensor (0)
																						// 2nd bit raw interrupt enable (1)
																						// 1st bit end of sequence (1) (needed to start sampling)
																						// 0th bit sampler differential input (0)
	// If interrupt are to be used , 
	// I must set the corresponding MASK bit in the ADCIM register . 																					
	ADC0_IM_R = (1<<3); // A sample has completed conv. and the respective ADCSSCTL3 IEn bit is set , 
											// enabling raw interrupt.
																	
  ADC0_ACTSS_R |= 0x00000008;     					// Enable SS3
	
	ADC0_ISC_R = (1<<3);                      // Clear interrupt status for SS3 
																					  // to be able to start sampling again
																						// 3rd bit corresponds to Interrupt(IN)3
}																						

// Function convert from ADC value to readale voltage 
unsigned long voltageRead (void)
{ 	
		unsigned long ADC_result;
		ADC0_PSSI_R = 0x0008;            		// Initiate SS3 (1)
		while((ADC0_RIS_R&0x08)==0){}   		// Wait for A/D conversion to finish 
																				// (3rd bit corresponds to SS3)													
		ADC_result = ADC0_SSFIFO3_R&0xFFF;	// Get ADC result
																			  // First 12 bits contain data	
		ADC0_ISC_R = 0x0008;             	
    return ADC_result;									
} 
	
