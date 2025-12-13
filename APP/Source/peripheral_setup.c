/***********************************************************************************************************************
File Name: peripheral_setup.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    Set up peripherals except the communication modules.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/26/2024  Fred Huang      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "peripheral_setup.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------
void Setup_SystemClock(void);
void Setup_PeripheralClock(void);
void Setup_GPIO(void);
void Setup_DMA(void);
void Setup_ADC(void);
void Setup_SHRTIM(void);
void Setup_ATIM(void);
void Setup_GTIM(void);
void Setup_UART(void);
void Setup_I2C(void);
void Setup_SPI(void);
void Setup_Interrupt(void);
void Setup_Watchdog(void);
//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
//----- Local -----
//----- Global -----
unsigned short adc1RawData[5];
unsigned short adc2RawData[6];
unsigned short adc3RawData[6];
unsigned short adc4RawData[6];
uint32_t SHRTIM_Period = ((uint16_t)((((uint64_t)250000000) / 4) / PWM_FREQ));
uint16_t ATIM_Period = (uint16_t)(((uint32_t)240000000 / 4 / PWM_FREQ) - 1);
uint16_t GTIM_Period = (uint16_t)((uint32_t)(120000000 / 2 / PWM_FREQ) - 1);
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Setup_SystemClock(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup system clock.
    HSE OSC = 8 MHz
    SHRTPLL = 250 MHz
    SYSCLK  = 240 MHz
    HCLK    = 240 MHz
    PCLK1   = 60 MHz
    PCLK2   = 60 MHz
    SystemCoreClock is set at system_n32h47x_48x.c.
**************************************************************************************************/
void Setup_SystemClock(void)
{
    /* Enable HSE */
    _RCC_CTRL.bit.HSEEN = 1;

    /* Wait till HSE is ready */
    while (_RCC_CTRL.bit.HSERDF != 1);

    /* FLASH Five Latency cycle */
    _FLASH_AC.bit.LATENCY = 5;

    /* Enable SHRTPLL module power */
    _RCC_SHRTPLLCTRL3.bit.SHRTPLLEN = 1;

    /* HSE oscillator clock selected as SHRTPLL clock entry*/
    _RCC_SHRTPLLCTRL3.bit.SHRTPLLSRC = 1;

    /* Configure the work mode to normal */
    _RCC_SHRTPLLCTRL1.bit.SHRTPLLMODE = 0;

    /* Enable Saturation and fraction accumulation function */
    _RCC_SHRTPLLCTRL1.bit.SATEN = 1;
    _RCC_SHRTPLLCTRL1.bit.FRACEN = 1;

    /* SHRTPLL = 250 MHz = SHRTPLL_source*(CLKF[25:0]/16384)/(CLKR[5:0] +1)/4 */
    _RCC_SHRTPLLCTRL1.bit.CLKF = 65536000;
    _RCC_SHRTPLLCTRL2.bit.CLKR = 31;
    _RCC_SHRTPLLCTRL3.bit.BWTRIM = 1999;

    /* Reset SHRTPLL */
    _RCC_AHBPRST.bit.SHRTPLLRST = 1;
    _DELAY_US(5);
    _RCC_AHBPRST.bit.SHRTPLLRST = 0;

    /* Wait till SHRTPLL is ready */
    while (_RCC_SHRTPLLCTRL3.bit.SHRTPLLRDF != 1);

    /* HCLK = SYSCLK / 1 = 240MHz */
    _RCC_CFG.bit.AHBPRES = 0;

    /* PCLK1 = HCLK / 4 = 60MHz */
    _RCC_CFG.bit.APB1PRES = 5;

    /* PCLK2 = HCLK / 4 = 60MHz */
    _RCC_CFG.bit.APB2PRES = 5;

    /* PLL multiplication factor = 30 */
    _RCC_CFG.bit.PLLMULFCT = 13;

    /* PLL HSE prescaler = 1 */
    _RCC_CFG.bit.PLLHSEPRES = 0;

    /* HSE oscillator clock selected as PLL clock entry */
    _RCC_CFG.bit.PLLSRC = 1;

    /* PLL input prescaler = 1 */
    _RCC_PLLCTRL.bit.PLLINPRES = 0;

    /* PLL output prescaler = 1 */
    _RCC_PLLCTRL.bit.PLLOUTPRES = 0;

    /* Enable PLL */
    _RCC_CTRL.bit.PLLEN = 1;

    /* Wait till PLL is ready */
    while (_RCC_CTRL.bit.PLLRDF != 1);

    /* Select PLL as system clock source */
    _RCC_PLLCTRL.bit.SCLKPLLSEL = 0;
    _RCC_CFG.bit.SCLKSW = 2;

    /* Wait till PLL is used as system clock source */
    while (_RCC_CFG.bit.SCLKSTS != 2);

    // ADC clock selection and prescaler = 3, PLL seleted as ADC clock (80MHz)
    _RCC_CFG2.bit.ADCHPRES = 2;
    _RCC_CFG2.bit.ADCOTGPLLPRES = 2;
    _RCC_CFG2.bit.ADCOTGPLLSEL = 0;
    _RCC_CFG2.bit.ADCOTGPLLEN = 1;
}

/**************************************************************************************************
Function Name:
    void Setup_PeripheralClock(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup peripheral clock.
**************************************************************************************************/
void Setup_PeripheralClock(void)
{
    // AHB peripheral clock enable
    _RCC_AHBPCLKEN.bit.DMA1EN = 1;
    _RCC_AHBPCLKEN.bit.DMA2EN = 1;
    _RCC_AHBPCLKEN.bit.ETHEN = 1;
    _RCC_AHBPCLKEN.bit.ATIM1EN = 1;
    _RCC_AHBPCLKEN.bit.ATIM2EN = 1;
    _RCC_AHBPCLKEN.bit.ATIM3EN = 1;
    _RCC_AHBPCLKEN.bit.SHRTIMEN = 1;

    // AHB1 peripheral clock enable
    _RCC_AHB1PCLKEN.bit.ADC1EN = 1;
    _RCC_AHB1PCLKEN.bit.ADC2EN = 1;
    _RCC_AHB1PCLKEN.bit.ADC3EN = 1;
    _RCC_AHB1PCLKEN.bit.ADC4EN = 1;
    _RCC_AHB1PCLKEN.bit.IOPAEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPBEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPCEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPDEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPEEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPFEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPGEN = 1;
    _RCC_AHB1PCLKEN.bit.IOPHEN = 1;
    _RCC_AHB1PCLKEN.bit.IOFILTSEL = 1;

    // APB1 peripheral clock enable
    _RCC_APB1PCLKEN.bit.USART2EN = 1;
    _RCC_APB1PCLKEN.bit.USART3EN = 1;
    _RCC_APB1PCLKEN.bit.UART5EN = 1;
    _RCC_APB1PCLKEN.bit.UART8EN = 1;
    _RCC_APB1PCLKEN.bit.I2C1EN = 1;
    _RCC_APB1PCLKEN.bit.I2C2EN = 1;
    _RCC_APB1PCLKEN.bit.I2C3EN = 1;
    _RCC_APB1PCLKEN.bit.I2C4EN = 1;
    _RCC_APB1PCLKEN.bit.SPI2EN = 1;
    _RCC_APB1PCLKEN.bit.GTIM7EN = 1;
    _RCC_APB1PCLKEN.bit.WWDGEN = 1;
    _RCC_APB1PCLKEN.bit.PWREN = 1;

    // APB2 peripheral clock enable
    _RCC_APB2PCLKEN.bit.AFIOEN = 1;
    _RCC_APB2PCLKEN.bit.USART1EN = 1;
    _RCC_APB2PCLKEN.bit.USART4EN = 1;
    _RCC_APB2PCLKEN.bit.UART6EN = 1;
    _RCC_APB2PCLKEN.bit.UART7EN = 1;

    // Backup register enable
    _RCC_BDCTRL.bit.RTCEN = 1;
    RTC_WaitForSynchro();
    _RCC_BDCTRL.bit.RTCEN = 0;
}
/***************************************************************************************************
Function Name:
    void Setup_GPIO(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup IO pins
***************************************************************************************************/
void Setup_GPIO(void)
{
    _AFIO_RMP_CFG.bit.SWJ_CFG = 2;
//------------------------------------------------------------
// GPIO Input
//------------------------------------------------------------
    /*************** Port A ***************/
    /*************** Port B ***************/
    /*************** Port C ***************/
    /*************** Port D ***************/
    /*************** Port E ***************/
    /*************** Port F ***************/
    /*************** Port G ***************/
    /*************** Port H ***************/
//------------------------------------------------------------
// GPIO Output
//------------------------------------------------------------
    /*************** Port A ***************/
    // PA12 - SPI2_CS_LED18
    _GPIOA_PMODE.bit.PMODE12 = 1;
    _GPIOA_POTYPE.bit.POT12 = 0;
    _GPIOA_PUPD.bit.PUPD12 = 1;
    // PA15 - SPI2_CS_LED20
    _GPIOA_PMODE.bit.PMODE15 = 1;
    _GPIOA_POTYPE.bit.POT15 = 0;
    _GPIOA_PUPD.bit.PUPD15 = 1;
    /*************** Port B ***************/
    // PB10 - SPI2_CS_LED5
    _GPIOB_PMODE.bit.PMODE10 = 1;
    _GPIOB_POTYPE.bit.POT10 = 0;
    _GPIOB_PUPD.bit.PUPD10 = 1;
    /*************** Port C ***************/
    // PC0 - BBU_IO_1
    _GPIOC_PMODE.bit.PMODE0 = 1;
    _GPIOC_POTYPE.bit.POT0 = 0;
    _GPIOC_PUPD.bit.PUPD0 = 1;
    // PC10 - SPI2_CS_LED1
    _GPIOC_PMODE.bit.PMODE10 = 1;
    _GPIOC_POTYPE.bit.POT10 = 0;
    _GPIOC_PUPD.bit.PUPD10 = 1;
    // PC11 - SPI2_CS_LED2
    _GPIOC_PMODE.bit.PMODE11 = 1;
    _GPIOC_POTYPE.bit.POT11 = 0;
    _GPIOC_PUPD.bit.PUPD11 = 1;
    // PC12 - SPI2_CS_LED3
    _GPIOC_PMODE.bit.PMODE12 = 1;
    _GPIOC_POTYPE.bit.POT12 = 0;
    _GPIOC_PUPD.bit.PUPD12 = 1;
    // PC13 - SPI2_CS_LED8
    _GPIOC_PMODE.bit.PMODE13 = 1;
    _GPIOC_POTYPE.bit.POT13 = 0;
    _GPIOC_PUPD.bit.PUPD13 = 1;
    // PC14 - SPI2_CS_LED21
    _GPIOC_PMODE.bit.PMODE14 = 1;
    _GPIOC_POTYPE.bit.POT14 = 0;
    _GPIOC_PUPD.bit.PUPD14 = 1;
    /*************** Port D ***************/
    // PD7 - SPI2_CS_LED4
    _GPIOD_PMODE.bit.PMODE7 = 1;
    _GPIOD_POTYPE.bit.POT7 = 0;
    _GPIOD_PUPD.bit.PUPD7 = 1;
    // PD15 - LED_GREEN
    _GPIOD_PMODE.bit.PMODE15 = 1;
    _GPIOD_POTYPE.bit.POT15 = 0;
    _GPIOD_PUPD.bit.PUPD15 = 0;
    /*************** Port E ***************/
    // PE5 - SPI2_CS_LED6
    _GPIOE_PMODE.bit.PMODE5 = 1;
    _GPIOE_POTYPE.bit.POT5 = 0;
    _GPIOE_PUPD.bit.PUPD5 = 1;
    // PE6 - SPI2_CS_LED7
    _GPIOE_PMODE.bit.PMODE6 = 1;
    _GPIOE_POTYPE.bit.POT6 = 0;
    _GPIOE_PUPD.bit.PUPD6 = 1;
    /*************** Port F ***************/
    // PF0 - SPI2_CS_LED22
    _GPIOF_PMODE.bit.PMODE0 = 1;
    _GPIOF_POTYPE.bit.POT0 = 0;
    _GPIOF_PUPD.bit.PUPD0 = 1;
    // PF1 - SPI2_CS_LED12
    _GPIOF_PMODE.bit.PMODE1 = 1;
    _GPIOF_POTYPE.bit.POT1 = 0;
    _GPIOF_PUPD.bit.PUPD1 = 1;
    // PF9 - BBU_IO_2
    _GPIOF_PMODE.bit.PMODE9 = 1;
    _GPIOF_POTYPE.bit.POT9 = 0;
    _GPIOF_PUPD.bit.PUPD9 = 1;
    // PF11 - SPI2_CS_LED14
    _GPIOF_PMODE.bit.PMODE11 = 1;
    _GPIOF_POTYPE.bit.POT11 = 0;
    _GPIOF_PUPD.bit.PUPD11 = 1;
    // PF12 - SPI2_CS_LED15
    _GPIOF_PMODE.bit.PMODE12 = 1;
    _GPIOF_POTYPE.bit.POT12 = 0;
    _GPIOF_PUPD.bit.PUPD12 = 1;
    // PF13 - SPI2_CS_LED16
    _GPIOF_PMODE.bit.PMODE13 = 1;
    _GPIOF_POTYPE.bit.POT13 = 0;
    _GPIOF_PUPD.bit.PUPD13 = 1;
    /*************** Port G ***************/
    // PG0 - EN
    _GPIOG_PMODE.bit.PMODE0 = 1;
    _GPIOG_POTYPE.bit.POT0 = 0;
    _GPIOG_PUPD.bit.PUPD0 = 0;
    // PG1 - SPI2_CS_LED17
    _GPIOG_PMODE.bit.PMODE1 = 1;
    _GPIOG_POTYPE.bit.POT1 = 0;
    _GPIOG_PUPD.bit.PUPD1 = 1;
    // PG2 - LED_RED
    _GPIOG_PMODE.bit.PMODE2 = 1;
    _GPIOG_POTYPE.bit.POT2 = 0;
    _GPIOG_PUPD.bit.PUPD2 = 0;
    // PG10 - SPI2_CS_LED9
    _GPIOG_PMODE.bit.PMODE10 = 1;
    _GPIOG_POTYPE.bit.POT10 = 0;
    _GPIOG_PUPD.bit.PUPD10 = 1;
    // PG11 - SPI2_CS_LED10
    _GPIOG_PMODE.bit.PMODE11 = 1;
    _GPIOG_POTYPE.bit.POT11 = 0;
    _GPIOG_PUPD.bit.PUPD11 = 1;
    // PG12 - SPI2_CS_LED11
    _GPIOG_PMODE.bit.PMODE12 = 1;
    _GPIOG_POTYPE.bit.POT12 = 0;
    _GPIOG_PUPD.bit.PUPD12 = 1;
    // PG13 - SPI2_CS_LED13
    _GPIOG_PMODE.bit.PMODE13 = 1;
    _GPIOG_POTYPE.bit.POT13 = 0;
    _GPIOG_PUPD.bit.PUPD13 = 1;
    /*************** Port H ***************/
    // PH2 - SPI2_CS_LED19
    _GPIOH_PMODE.bit.PMODE2 = 1;
    _GPIOH_POTYPE.bit.POT2 = 0;
    _GPIOH_PUPD.bit.PUPD2 = 1;
//------------------------------------------------------------
// ADC
//------------------------------------------------------------
    /*************** ADC 1 ***************/
    // PA3 - Exp_LED3_IDA (ADC1_IN2)
    _GPIOA_PMODE.bit.PMODE3 = 3;
    _GPIOA_PUPD.bit.PUPD3 = 0;
    // PA0 - Exp_LED2_IDB (ADC1_IN3)
    _GPIOA_PMODE.bit.PMODE0 = 3;
    _GPIOA_PUPD.bit.PUPD0 = 0;
//    // PC0 - ADC_NTC3 (ADC1_IN6)
//    _GPIOC_PMODE.bit.PMODE0 = 3;
//    _GPIOC_PUPD.bit.PUPD0 = 0;
    // PC2 - ADC_NTC4 (ADC1_IN8)
    _GPIOC_PMODE.bit.PMODE2 = 3;
    _GPIOC_PUPD.bit.PUPD2 = 0;
    // PC3 - Exp_LED2_IDA (ADC1_IN9)
    _GPIOC_PMODE.bit.PMODE3 = 3;
    _GPIOC_PUPD.bit.PUPD3 = 0;
    // PF7 - Iin_sense (ADC1_IN15)
    _GPIOF_PMODE.bit.PMODE7 = 3;
    _GPIOF_PUPD.bit.PUPD7 = 0;
    /*************** ADC 2 ***************/
    // PA6 - Exp_LED4_IDA (ADC2_IN0)
    _GPIOA_PMODE.bit.PMODE6 = 3;
    _GPIOA_PUPD.bit.PUPD6 = 0;
    // PF8 - ADC_NTC7 (ADC2_IN1)
    _GPIOF_PMODE.bit.PMODE8 = 3;
    _GPIOF_PUPD.bit.PUPD8 = 0;
    // PC5 - Exp_LED4_IDB (ADC2_IN11)
    _GPIOC_PMODE.bit.PMODE5 = 3;
    _GPIOC_PUPD.bit.PUPD5 = 0;
    // PB2 - Exp_LED1_IDA (ADC2_IN12)
    _GPIOB_PMODE.bit.PMODE2 = 3;
    _GPIOB_PUPD.bit.PUPD2 = 0;
    // PF6 - +48Vbus_sense (ADC2_IN16)
    _GPIOF_PMODE.bit.PMODE6 = 3;
    _GPIOF_PUPD.bit.PUPD6 = 0;
    // PA4 - Exp_LED3_IDB (ADC2_IN17)
    _GPIOA_PMODE.bit.PMODE4 = 3;
    _GPIOA_PUPD.bit.PUPD4 = 0;
    /*************** ADC 3 ***************/
    // PE7 - Exp_LED1_IDB (ADC3_IN4)
    _GPIOE_PMODE.bit.PMODE7 = 3;
    _GPIOE_PUPD.bit.PUPD7 = 0;
    // PE8 - Exp_LED5_IDA (ADC3_IN6)
    _GPIOE_PMODE.bit.PMODE8 = 3;
    _GPIOE_PUPD.bit.PUPD8 = 0;
    // PD11 - Exp_LED6_IDA (ADC3_IN8)
    _GPIOD_PMODE.bit.PMODE11 = 3;
    _GPIOD_PUPD.bit.PUPD11 = 0;
    // PD12 - Exp_LED6_IDB (ADC3_IN9)
    _GPIOD_PMODE.bit.PMODE12 = 3;
    _GPIOD_PUPD.bit.PUPD12 = 0;
    // PD14 - ADC_NTC6 (ADC3_IN11)
    _GPIOD_PMODE.bit.PMODE14 = 3;
    _GPIOD_PUPD.bit.PUPD14 = 0;
    // PE10 - Exp_LED5_IDB (ADC3_IN14)
    _GPIOE_PMODE.bit.PMODE10 = 3;
    _GPIOE_PUPD.bit.PUPD10 = 0;
    /*************** ADC 4 ***************/
    // PF10 - ADC_NTC1 (ADC4_IN0)
    _GPIOF_PMODE.bit.PMODE10 = 3;
    _GPIOF_PUPD.bit.PUPD10 = 0;
    // PE15 - Exp_LED8_IDB (ADC4_IN2)
    _GPIOE_PMODE.bit.PMODE15 = 3;
    _GPIOE_PUPD.bit.PUPD15 = 0;
    // PB15 - Exp_LED7_IDB (ADC4_IN3)
    _GPIOB_PMODE.bit.PMODE15 = 3;
    _GPIOB_PUPD.bit.PUPD15 = 0;
    // PB14 - Exp_LED7_IDA (ADC4_IN4)
    _GPIOB_PMODE.bit.PMODE14 = 3;
    _GPIOB_PUPD.bit.PUPD14 = 0;
    // PD13 - ADC_NTC5 (ADC4_IN10)
    _GPIOD_PMODE.bit.PMODE13 = 3;
    _GPIOD_PUPD.bit.PUPD13 = 0;
    // PE12 - Exp_LED8_IDA (ADC4_IN16)
    _GPIOE_PMODE.bit.PMODE12 = 3;
    _GPIOE_PUPD.bit.PUPD12 = 0;
//    // PF9 - ADC_NTC2 (ADC4_IN17)
//    _GPIOF_PMODE.bit.PMODE9 = 3;
//    _GPIOF_PUPD.bit.PUPD9 = 0;
//------------------------------------------------------------
// TIM
//------------------------------------------------------------
    /*************** SHRTIM 1 ***************/
    // PA8 - Exp_LED8_PWMB (SHRTIM1_CHA1)
    _GPIOA_PMODE.bit.PMODE8 = 2;
    _GPIOA_POTYPE.bit.POT8 = 0;
    _GPIOA_PUPD.bit.PUPD8 = 0;
    _GPIOA_SR.bit.SR8 = 0;
    _GPIOA_AFSEL2.bit.AFSEL8 = 6;
    // PA9 - Exp_LED8_PWMA (SHRTIM1_CHA2)
    _GPIOA_PMODE.bit.PMODE9 = 2;
    _GPIOA_POTYPE.bit.POT9 = 0;
    _GPIOA_PUPD.bit.PUPD9 = 0;
    _GPIOA_SR.bit.SR9 = 0;
    _GPIOA_AFSEL2.bit.AFSEL9 = 6;
    // PA10 - Exp_LED5_PWMB (SHRTIM1_CHB1)
    _GPIOA_PMODE.bit.PMODE10 = 2;
    _GPIOA_POTYPE.bit.POT10 = 0;
    _GPIOA_PUPD.bit.PUPD10 = 0;
    _GPIOA_SR.bit.SR10 = 0;
    _GPIOA_AFSEL2.bit.AFSEL10 = 12;
    // PA11 - Exp_LED5_PWMA (SHRTIM1_CHB2)
    _GPIOA_PMODE.bit.PMODE11 = 2;
    _GPIOA_POTYPE.bit.POT11 = 0;
    _GPIOA_PUPD.bit.PUPD11 = 0;
    _GPIOA_SR.bit.SR11 = 0;
    _GPIOA_AFSEL2.bit.AFSEL11 = 12;
    // PC8 - Exp_LED7_PWMB (SHRTIM1_CHE1)
    _GPIOC_PMODE.bit.PMODE8 = 2;
    _GPIOC_POTYPE.bit.POT8 = 0;
    _GPIOC_PUPD.bit.PUPD8 = 0;
    _GPIOC_SR.bit.SR8 = 0;
    _GPIOC_AFSEL2.bit.AFSEL8 = 11;
    // PC9 - Exp_LED7_PWMA (SHRTIM1_CHE2)
    _GPIOC_PMODE.bit.PMODE9 = 2;
    _GPIOC_POTYPE.bit.POT9 = 0;
    _GPIOC_PUPD.bit.PUPD9 = 0;
    _GPIOC_SR.bit.SR9 = 0;
    _GPIOC_AFSEL2.bit.AFSEL9 = 11;
    // PC6 - Exp_LED6_PWMB (SHRTIM1_CHF1)
    _GPIOC_PMODE.bit.PMODE6 = 2;
    _GPIOC_POTYPE.bit.POT6 = 0;
    _GPIOC_PUPD.bit.PUPD6 = 0;
    _GPIOC_SR.bit.SR6 = 0;
    _GPIOC_AFSEL1.bit.AFSEL6 = 11;
    // PC7 - Exp_LED6_PWMA (SHRTIM1_CHF2)
    _GPIOC_PMODE.bit.PMODE7 = 2;
    _GPIOC_POTYPE.bit.POT7 = 0;
    _GPIOC_PUPD.bit.PUPD7 = 0;
    _GPIOC_SR.bit.SR7 = 0;
    _GPIOC_AFSEL1.bit.AFSEL7 = 11;
    /*************** ATIM 1 ***************/
    // PE9 - Exp_LED4_PWMA (ATIM1_CH1)
    _GPIOE_PMODE.bit.PMODE9 = 2;
    _GPIOE_POTYPE.bit.POT9 = 0;
    _GPIOE_PUPD.bit.PUPD9 = 0;
    _GPIOE_SR.bit.SR9 = 1;
    _GPIOE_AFSEL2.bit.AFSEL9 = 3;
    // PE11 - Exp_LED3_PWMB (ATIM1_CH2)
    _GPIOE_PMODE.bit.PMODE11 = 2;
    _GPIOE_POTYPE.bit.POT11 = 0;
    _GPIOE_PUPD.bit.PUPD11 = 0;
    _GPIOE_SR.bit.SR11 = 1;
    _GPIOE_AFSEL2.bit.AFSEL11 = 3;
    // PE13 - Exp_LED3_PWMA (ATIM1_CH3)
    _GPIOE_PMODE.bit.PMODE13 = 2;
    _GPIOE_POTYPE.bit.POT13 = 0;
    _GPIOE_PUPD.bit.PUPD13 = 0;
    _GPIOE_SR.bit.SR13 = 1;
    _GPIOE_AFSEL3.bit.AFSEL13 = 3;
    // PE14 - Exp_LED2_PWMB (ATIM1_CH4)
    _GPIOE_PMODE.bit.PMODE14 = 2;
    _GPIOE_POTYPE.bit.POT14 = 0;
    _GPIOE_PUPD.bit.PUPD14 = 0;
    _GPIOE_SR.bit.SR14 = 1;
    _GPIOE_AFSEL3.bit.AFSEL14 = 3;
    /*************** ATIM 2 ***************/
    // PA7 - Exp_LED1_PWMB (ATIM2_CH1N)
    _GPIOA_PMODE.bit.PMODE7 = 2;
    _GPIOA_POTYPE.bit.POT7 = 0;
    _GPIOA_PUPD.bit.PUPD7 = 0;
    _GPIOA_SR.bit.SR7 = 1;
    _GPIOA_AFSEL1.bit.AFSEL7 = 6;
    // PB0 - Exp_LED1_PWMA (ATIM2_CH2N)
    _GPIOB_PMODE.bit.PMODE0 = 2;
    _GPIOB_POTYPE.bit.POT0 = 0;
    _GPIOB_PUPD.bit.PUPD0 = 0;
    _GPIOB_SR.bit.SR0 = 1;
    _GPIOB_AFSEL0.bit.AFSEL0 = 6;
    // PB1 - Exp_LED4_PWMB (ATIM2_CH3N)
    _GPIOB_PMODE.bit.PMODE1 = 2;
    _GPIOB_POTYPE.bit.POT1 = 0;
    _GPIOB_PUPD.bit.PUPD1 = 0;
    _GPIOB_SR.bit.SR1 = 1;
    _GPIOB_AFSEL0.bit.AFSEL1 = 6;
    // PH3 - Exp_LED2_PWMA (ATIM2_CH4N)
    _GPIOH_PMODE.bit.PMODE3 = 2;
    _GPIOH_POTYPE.bit.POT3 = 0;
    _GPIOH_PUPD.bit.PUPD3 = 0;
    _GPIOH_SR.bit.SR3 = 1;
    _GPIOH_AFSEL0.bit.AFSEL3 = 6;
    /*************** ATIM 3 ***************/
    // PF2 - PWM_FAN1_TACH (ATIM3_CH3 - Input capture)
    _GPIOF_PMODE.bit.PMODE2 = 2;
    _GPIOF_POTYPE.bit.POT2 = 0;
    _GPIOF_PUPD.bit.PUPD2 = 0;
    _GPIOF_SR.bit.SR2 = 1;
    _GPIOF_AFSEL0.bit.AFSEL2 = 3;
    // PF3 - PWM_FAN2_TACH (ATIM3_CH4 - Input capture)
    _GPIOF_PMODE.bit.PMODE3 = 2;
    _GPIOF_POTYPE.bit.POT3 = 0;
    _GPIOF_PUPD.bit.PUPD3 = 0;
    _GPIOF_SR.bit.SR3 = 1;
    _GPIOF_AFSEL0.bit.AFSEL3 = 3;
    /*************** GTIM 1 ***************/
    /*************** GTIM 2 ***************/
    /*************** GTIM 3 ***************/
    /*************** GTIM 4 ***************/
    /*************** GTIM 5 ***************/
    /*************** GTIM 6 ***************/
    /*************** GTIM 7 ***************/
    // PC15 - PWM_FAN_Control (GTIM7_CH4)
    _GPIOC_PMODE.bit.PMODE15 = 2;
    _GPIOC_POTYPE.bit.POT15 = 0;
    _GPIOC_PUPD.bit.PUPD15 = 0;
    _GPIOC_SR.bit.SR15 = 1;
    _GPIOC_AFSEL3.bit.AFSEL15 = 7;
    /*************** GTIM 8 ***************/
    /*************** GTIM 9 ***************/
    /*************** GTIM 10 ***************/
//------------------------------------------------------------
// SPI
//------------------------------------------------------------
    /*************** SPI 2 ***************/
    // PD3 - SPI2_SCK
    _GPIOD_PMODE.bit.PMODE3 = 2;
    _GPIOD_POTYPE.bit.POT3 = 0;
    _GPIOD_PUPD.bit.PUPD3 = 0;
    _GPIOD_AFSEL0.bit.AFSEL3 = 1;
    // PG14 - SPI2_MISO
    _GPIOG_PMODE.bit.PMODE14 = 2;
    _GPIOG_POTYPE.bit.POT14 = 0;
    _GPIOG_PUPD.bit.PUPD14 = 0;
    _GPIOG_AFSEL3.bit.AFSEL14 = 9;
    // PG9 - SPI2_MOSI
    _GPIOG_PMODE.bit.PMODE9 = 2;
    _GPIOG_POTYPE.bit.POT9 = 0;
    _GPIOG_PUPD.bit.PUPD9 = 0;
    _GPIOG_AFSEL2.bit.AFSEL9 = 2;
//------------------------------------------------------------
// I2C
//------------------------------------------------------------
    /*************** I2C 1 ***************/
    // PB8 - I2C_SCL_GUI
    _GPIOB_PMODE.bit.PMODE8 = 2;
    _GPIOB_POTYPE.bit.POT8 = 1;
    _GPIOB_PUPD.bit.PUPD8 = 1;
    _GPIOB_SR.bit.SR8 = 1;
    _GPIOB_AFSEL2.bit.AFSEL8 = 2;
    // PB9 - I2C_SDA_GUI
    _GPIOB_PMODE.bit.PMODE9 = 2;
    _GPIOB_POTYPE.bit.POT9 = 1;
    _GPIOB_PUPD.bit.PUPD9 = 1;
    _GPIOB_SR.bit.SR9 = 1;
    _GPIOB_AFSEL2.bit.AFSEL9 = 13;
    /*************** I2C 2 ***************/
    // PC4 - I2C2_SCL
    _GPIOC_PMODE.bit.PMODE4 = 2;
    _GPIOC_POTYPE.bit.POT4 = 1;
    _GPIOC_PUPD.bit.PUPD4 = 1;
    _GPIOC_SR.bit.SR4 = 1;
    _GPIOC_AFSEL1.bit.AFSEL4 = 7;
    // PA5 - I2C2_SDA
    _GPIOA_PMODE.bit.PMODE5 = 2;
    _GPIOA_POTYPE.bit.POT5 = 1;
    _GPIOA_PUPD.bit.PUPD5 = 1;
    _GPIOA_SR.bit.SR5 = 1;
    _GPIOA_AFSEL1.bit.AFSEL5 = 11;
    /*************** I2C 3 ***************/
    // PF4 - I2C3_SCL
    _GPIOF_PMODE.bit.PMODE4 = 2;
    _GPIOF_POTYPE.bit.POT4 = 1;
    _GPIOF_PUPD.bit.PUPD4 = 1;
    _GPIOF_SR.bit.SR4 = 1;
    _GPIOF_AFSEL1.bit.AFSEL4 = 6;
    // PF5 - I2C3_SDA
    _GPIOF_PMODE.bit.PMODE5 = 2;
    _GPIOF_POTYPE.bit.POT5 = 1;
    _GPIOF_PUPD.bit.PUPD5 = 1;
    _GPIOF_SR.bit.SR5 = 1;
    _GPIOF_AFSEL1.bit.AFSEL5 = 7;
    /*************** I2C 4 ***************/
    // PF14 - I2C4_SCL
    _GPIOF_PMODE.bit.PMODE14 = 2;
    _GPIOF_POTYPE.bit.POT14 = 1;
    _GPIOF_PUPD.bit.PUPD14 = 1;
    _GPIOF_SR.bit.SR14 = 1;
    _GPIOF_AFSEL3.bit.AFSEL14 = 7;
    // PF15 - I2C4_SDA
    _GPIOF_PMODE.bit.PMODE15 = 2;
    _GPIOF_POTYPE.bit.POT15 = 1;
    _GPIOF_PUPD.bit.PUPD15 = 1;
    _GPIOF_SR.bit.SR15 = 1;
    _GPIOF_AFSEL3.bit.AFSEL15 = 7;
//------------------------------------------------------------
// UART
//------------------------------------------------------------
    /*************** USART 1 ***************/
    // PE0 - USART1_Tx
    _GPIOE_PMODE.bit.PMODE0 = 2;
    _GPIOE_POTYPE.bit.POT0 = 0;
    _GPIOE_PUPD.bit.PUPD0 = 1;
    _GPIOE_AFSEL0.bit.AFSEL0 = 7;
    // PE1 - USART1_Rx
    _GPIOE_PMODE.bit.PMODE1 = 2;
    _GPIOE_POTYPE.bit.POT1 = 0;
    _GPIOE_PUPD.bit.PUPD1 = 1;
    _GPIOE_AFSEL0.bit.AFSEL1 = 7;
    // PH5 - USART1_DE
    _GPIOH_PMODE.bit.PMODE5 = 1;
    _GPIOH_POTYPE.bit.POT5 = 0;
    _GPIOH_PUPD.bit.PUPD5 = 0;
    /*************** USART 2 ***************/
    // PB3 - USART2_Tx
    _GPIOB_PMODE.bit.PMODE3 = 2;
    _GPIOB_POTYPE.bit.POT3 = 0;
    _GPIOB_PUPD.bit.PUPD3 = 1;
    _GPIOB_AFSEL0.bit.AFSEL3 = 9;
    // PB4 - USART2_Rx
    _GPIOB_PMODE.bit.PMODE4 = 2;
    _GPIOB_POTYPE.bit.POT4 = 0;
    _GPIOB_PUPD.bit.PUPD4 = 1;
    _GPIOB_AFSEL1.bit.AFSEL4 = 9;
    // PG15 - USART2_DE
    _GPIOG_PMODE.bit.PMODE15 = 1;
    _GPIOG_POTYPE.bit.POT15 = 0;
    _GPIOG_PUPD.bit.PUPD15 = 0;
    /*************** USART 3 ***************/
    // PB5 - USART3_Tx
    _GPIOB_PMODE.bit.PMODE5 = 2;
    _GPIOB_POTYPE.bit.POT5 = 0;
    _GPIOB_PUPD.bit.PUPD5 = 1;
    _GPIOB_AFSEL1.bit.AFSEL5 = 17;
    // PB6 - USART3_Rx
    _GPIOB_PMODE.bit.PMODE6 = 2;
    _GPIOB_POTYPE.bit.POT6 = 0;
    _GPIOB_PUPD.bit.PUPD6 = 1;
    _GPIOB_AFSEL1.bit.AFSEL6 = 18;
    // PB7 - USART3_DE
    _GPIOB_PMODE.bit.PMODE7 = 1;
    _GPIOB_POTYPE.bit.POT7 = 0;
    _GPIOB_PUPD.bit.PUPD7 = 0;
    /*************** USART 4 ***************/
    // PE2 - USART4_Tx
    _GPIOE_PMODE.bit.PMODE2 = 2;
    _GPIOE_POTYPE.bit.POT2 = 0;
    _GPIOE_PUPD.bit.PUPD2 = 1;
    _GPIOE_AFSEL0.bit.AFSEL2 = 7;
    // PE3 - USART4_Rx
    _GPIOE_PMODE.bit.PMODE3 = 2;
    _GPIOE_POTYPE.bit.POT3 = 0;
    _GPIOE_PUPD.bit.PUPD3 = 1;
    _GPIOE_AFSEL0.bit.AFSEL3 = 7;
    // PE4 - USART4_DE
    _GPIOE_PMODE.bit.PMODE4 = 1;
    _GPIOE_POTYPE.bit.POT4 = 0;
    _GPIOE_PUPD.bit.PUPD4 = 0;
    /*************** UART 5 ***************/
    // PD4 - UART5_Tx
    _GPIOD_PMODE.bit.PMODE4 = 2;
    _GPIOD_POTYPE.bit.POT4 = 0;
    _GPIOD_PUPD.bit.PUPD4 = 1;
    _GPIOD_AFSEL1.bit.AFSEL4 = 20;
    // PD5 - UART5_Rx
    _GPIOD_PMODE.bit.PMODE5 = 2;
    _GPIOD_POTYPE.bit.POT5 = 0;
    _GPIOD_PUPD.bit.PUPD5 = 1;
    _GPIOD_AFSEL1.bit.AFSEL5 = 21;
    // PD6 - UART5_DE
    _GPIOD_PMODE.bit.PMODE6 = 1;
    _GPIOD_POTYPE.bit.POT6 = 0;
    _GPIOD_PUPD.bit.PUPD6 = 0;
    /*************** UART 6 ***************/
    // PD0 - UART6_Tx
    _GPIOD_PMODE.bit.PMODE0 = 2;
    _GPIOD_POTYPE.bit.POT0 = 0;
    _GPIOD_PUPD.bit.PUPD0 = 1;
    _GPIOD_AFSEL0.bit.AFSEL0 = 6;
    // PD1 - UART6_Rx
    _GPIOD_PMODE.bit.PMODE1 = 2;
    _GPIOD_POTYPE.bit.POT1 = 0;
    _GPIOD_PUPD.bit.PUPD1 = 1;
    _GPIOD_AFSEL0.bit.AFSEL1 = 6;
    // PD2 - UART6_DE
    _GPIOD_PMODE.bit.PMODE2 = 1;
    _GPIOD_POTYPE.bit.POT2 = 0;
    _GPIOD_PUPD.bit.PUPD2 = 0;
    /*************** UART 7 ***************/
    // PG7 - UART7_Tx
    _GPIOG_PMODE.bit.PMODE7 = 2;
    _GPIOG_POTYPE.bit.POT7 = 0;
    _GPIOG_PUPD.bit.PUPD7 = 1;
    _GPIOG_AFSEL1.bit.AFSEL7 = 8;
    // PG8 - UART7_Rx
    _GPIOG_PMODE.bit.PMODE8 = 2;
    _GPIOG_POTYPE.bit.POT8 = 0;
    _GPIOG_PUPD.bit.PUPD8 = 1;
    _GPIOG_AFSEL2.bit.AFSEL8 = 8;
    // PG6 - UART7_DE
    _GPIOG_PMODE.bit.PMODE6 = 1;
    _GPIOG_POTYPE.bit.POT6 = 0;
    _GPIOG_PUPD.bit.PUPD6 = 0;
    /*************** UART 8 ***************/
    // PG4 - UART8_Tx
    _GPIOG_PMODE.bit.PMODE4 = 2;
    _GPIOG_POTYPE.bit.POT4 = 0;
    _GPIOG_PUPD.bit.PUPD4 = 1;
    _GPIOG_AFSEL1.bit.AFSEL4 = 23;
    // PG5 - UART8_Rx
    _GPIOG_PMODE.bit.PMODE5 = 2;
    _GPIOG_POTYPE.bit.POT5 = 0;
    _GPIOG_PUPD.bit.PUPD5 = 1;
    _GPIOG_AFSEL1.bit.AFSEL5 = 24;
    // PG3 - UART8_DE
    _GPIOG_PMODE.bit.PMODE3 = 1;
    _GPIOG_POTYPE.bit.POT3 = 0;
    _GPIOG_PUPD.bit.PUPD3 = 0;
}

/**************************************************************************************************
Function Name:
    void Setup_DMA(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setupe DMA.
**************************************************************************************************/
void Setup_DMA(void)
{

    usart1Dma1Rx.func.init(&usart1Dma1Rx, (unsigned long)usart1.member.reg.DAT);
    usart2Dma1Rx.func.init(&usart2Dma1Rx, (unsigned long)usart2.member.reg.DAT);
    usart3Dma1Rx.func.init(&usart3Dma1Rx, (unsigned long)usart3.member.reg.DAT);
    usart4Dma1Rx.func.init(&usart4Dma1Rx, (unsigned long)usart4.member.reg.DAT);
    uart5Dma1Rx.func.init(&uart5Dma1Rx, (unsigned long)uart5.member.reg.DAT);
    uart6Dma1Rx.func.init(&uart6Dma1Rx, (unsigned long)uart6.member.reg.DAT);
    uart7Dma1Rx.func.init(&uart7Dma1Rx, (unsigned long)uart7.member.reg.DAT);
    uart8Dma1Rx.func.init(&uart8Dma1Rx, (unsigned long)uart8.member.reg.DAT);

    usart1Dma2Tx.func.init(&usart1Dma2Tx, (unsigned long)usart1.member.reg.DAT);
    usart2Dma2Tx.func.init(&usart2Dma2Tx, (unsigned long)usart2.member.reg.DAT);
    usart3Dma2Tx.func.init(&usart3Dma2Tx, (unsigned long)usart3.member.reg.DAT);
    usart4Dma2Tx.func.init(&usart4Dma2Tx, (unsigned long)usart4.member.reg.DAT);
    uart5Dma2Tx.func.init(&uart5Dma2Tx, (unsigned long)uart5.member.reg.DAT);
    uart6Dma2Tx.func.init(&uart6Dma2Tx, (unsigned long)uart6.member.reg.DAT);
    uart7Dma2Tx.func.init(&uart7Dma2Tx, (unsigned long)uart7.member.reg.DAT);
    uart8Dma2Tx.func.init(&uart8Dma2Tx, (unsigned long)uart8.member.reg.DAT);

//    // Peripheral address
//    _DMA2_PADDR1.bit.ADDR = (uint32_t)&_ADC1_DAT.all;
//    _DMA2_PADDR2.bit.ADDR = (uint32_t)&_ADC2_DAT.all;
//    _DMA2_PADDR3.bit.ADDR = (uint32_t)&_ADC3_DAT.all;
//    _DMA2_PADDR4.bit.ADDR = (uint32_t)&_ADC4_DAT.all;
//
//    // Memory address
//    _DMA2_MADDR1.bit.ADDR = (uint32_t)adc1RawData;
//    _DMA2_MADDR2.bit.ADDR = (uint32_t)adc2RawData;
//    _DMA2_MADDR3.bit.ADDR = (uint32_t)adc3RawData;
//    _DMA2_MADDR4.bit.ADDR = (uint32_t)adc4RawData;
//
//    // Number of data to transfer
//    _DMA2_TXNUM1.bit.NDTX = 6;
//    _DMA2_TXNUM2.bit.NDTX = 6;
//    _DMA2_TXNUM3.bit.NDTX = 6;
//    _DMA2_TXNUM4.bit.NDTX = 7;
//
//    // Data transfer direction : Read from peripheral
//    _DMA2_CHCFG1.bit.DIR = 0;
//    _DMA2_CHCFG2.bit.DIR = 0;
//    _DMA2_CHCFG3.bit.DIR = 0;
//    _DMA2_CHCFG4.bit.DIR = 0;
//
//    // Peripheral increment mode : disabled
//    _DMA2_CHCFG1.bit.PINC = 0;
//    _DMA2_CHCFG2.bit.PINC = 0;
//    _DMA2_CHCFG3.bit.PINC = 0;
//    _DMA2_CHCFG4.bit.PINC = 0;
//
//    // Memory increment mode : enabled
//    _DMA2_CHCFG1.bit.MINC = 1;
//    _DMA2_CHCFG2.bit.MINC = 1;
//    _DMA2_CHCFG3.bit.MINC = 1;
//    _DMA2_CHCFG4.bit.MINC = 1;
//
//    // Memory data size : 16bit
//    _DMA2_CHCFG1.bit.MSIZE = 1;
//    _DMA2_CHCFG2.bit.MSIZE = 1;
//    _DMA2_CHCFG3.bit.MSIZE = 1;
//    _DMA2_CHCFG4.bit.MSIZE = 1;
//
//    // Peripheral data size : 16bit
//    _DMA2_CHCFG1.bit.PSIZE = 1;
//    _DMA2_CHCFG2.bit.PSIZE = 1;
//    _DMA2_CHCFG3.bit.PSIZE = 1;
//    _DMA2_CHCFG4.bit.PSIZE = 1;
//
//    // DMA circular mode : enabled
//    _DMA2_CHCFG1.bit.CIRC = 1;
//    _DMA2_CHCFG2.bit.CIRC = 1;
//    _DMA2_CHCFG3.bit.CIRC = 1;
//    _DMA2_CHCFG4.bit.CIRC = 1;
//
//    // Priority level : low
//    _DMA2_CHCFG1.bit.PRIOLVL = 0;
//    _DMA2_CHCFG2.bit.PRIOLVL = 0;
//    _DMA2_CHCFG3.bit.PRIOLVL = 0;
//    _DMA2_CHCFG4.bit.PRIOLVL = 0;
//
//    // Memory to memory mode : disabled
//    _DMA2_CHCFG1.bit.MEM2MEM = 0;
//    _DMA2_CHCFG2.bit.MEM2MEM = 0;
//    _DMA2_CHCFG3.bit.MEM2MEM = 0;
//    _DMA2_CHCFG4.bit.MEM2MEM = 0;
//
//    // Disable DMA transmission complete interrupt
//    _DMA2_CHCFG1.bit.TXCIE = 0;
//    _DMA2_CHCFG2.bit.TXCIE = 0;
//    _DMA2_CHCFG3.bit.TXCIE = 0;
//    _DMA2_CHCFG4.bit.TXCIE = 0;
//
//    // DMA request remap channel selection
//    _DMA2_CHSEL1.bit.CH_SEL = 0; // ADC1
//    _DMA2_CHSEL2.bit.CH_SEL = 1; // ADC2
//    _DMA2_CHSEL3.bit.CH_SEL = 2; // ADC3
//    _DMA2_CHSEL4.bit.CH_SEL = 3; // ADC4
//
//    // Enable DMA
//    _DMA2_CHCFG1.bit.CHEN = 1;
//    _DMA2_CHCFG2.bit.CHEN = 1;
//    _DMA2_CHCFG3.bit.CHEN = 1;
//    _DMA2_CHCFG4.bit.CHEN = 1;
}

/***************************************************************************************************
Function Name:
    void Setup_ADC(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup ADC1, ADC2, ADC3, ADC4
***************************************************************************************************/
void Setup_ADC(void)
{
    uint8_t sampling_time = 2; // 4.5 cycles

    /* ADC BUF Enable  */
    *(uint32_t *)((uint32_t)0x40010000U + 0x4C) |= (0x01U << 13);

    // Clock selection : PLL
    _ADC1_CTRL3.bit.CKMOD = 1;
    _ADC2_CTRL3.bit.CKMOD = 1;
    _ADC3_CTRL3.bit.CKMOD = 1;
    _ADC4_CTRL3.bit.CKMOD = 1;

    // Disable scan mode
    _ADC1_CTRL1.bit.SCANMD = 0;
    _ADC2_CTRL1.bit.SCANMD = 0;
    _ADC3_CTRL1.bit.SCANMD = 0;
    _ADC4_CTRL1.bit.SCANMD = 0;

    // Working mode : Independent
    _ADC1_CTRL1.bit.MULTMODE = 0;
    _ADC2_CTRL1.bit.MULTMODE = 0;
    _ADC3_CTRL1.bit.MULTMODE = 0;
    _ADC4_CTRL1.bit.MULTMODE = 0;

    // External trigger event : Software
    _ADC1_CTRL2.bit.EXTRSEL = 42;
    _ADC2_CTRL2.bit.EXTRSEL = 42;
    _ADC3_CTRL2.bit.EXTRSEL = 42;
    _ADC4_CTRL2.bit.EXTRSEL = 42;

    // Disable continuous conversion
    _ADC1_CTRL2.bit.CTU = 0;
    _ADC2_CTRL2.bit.CTU = 0;
    _ADC3_CTRL2.bit.CTU = 0;
    _ADC4_CTRL2.bit.CTU = 0;

    // Data alignment : right
    _ADC1_CTRL2.bit.ALIG = 0;
    _ADC2_CTRL2.bit.ALIG = 0;
    _ADC3_CTRL2.bit.ALIG = 0;
    _ADC4_CTRL2.bit.ALIG = 0;

    // Oversampling times : 256
    _ADC1_CTRL3.bit.OSR = 8;
    _ADC2_CTRL3.bit.OSR = 8;
    _ADC3_CTRL3.bit.OSR = 8;
    _ADC4_CTRL3.bit.OSR = 8;

    // Oversampling data right shit : 8 bit
    _ADC1_CTRL3.bit.OSS = 8;
    _ADC2_CTRL3.bit.OSS = 8;
    _ADC3_CTRL3.bit.OSS = 8;
    _ADC4_CTRL3.bit.OSS = 8;

    // Oversampling mode : Regular continuous
    _ADC1_CTRL3.bit.OSRMD = 0;
    _ADC2_CTRL3.bit.OSRMD = 0;
    _ADC3_CTRL3.bit.OSRMD = 0;
    _ADC4_CTRL3.bit.OSRMD = 0;

    // Enable oversampling function
    _ADC1_CTRL3.bit.OSRE = 1;
    _ADC2_CTRL3.bit.OSRE = 1;
    _ADC3_CTRL3.bit.OSRE = 1;
    _ADC4_CTRL3.bit.OSRE = 1;

    // Resolution : 12 bit
    _ADC1_CTRL3.bit.RES = 3;
    _ADC2_CTRL3.bit.RES = 3;
    _ADC3_CTRL3.bit.RES = 3;
    _ADC4_CTRL3.bit.RES = 3;

    // ADC1 regular channel configuration
    _ADC1_RSEQ3.bit.LEN = 1;
//    _ADC1_RSEQ1.bit.SEQ1 = 2;
//    _ADC1_RSEQ1.bit.SEQ2 = 3;
//    _ADC1_RSEQ1.bit.SEQ3 = 8;
//    _ADC1_RSEQ1.bit.SEQ4 = 9;
//    _ADC1_RSEQ1.bit.SEQ5 = 15;
    _ADC1_SAMPT1.bit.SAMP2 = sampling_time;
    _ADC1_SAMPT1.bit.SAMP3 = sampling_time;
    _ADC1_SAMPT2.bit.SAMP8 = sampling_time;
    _ADC1_SAMPT2.bit.SAMP9 = sampling_time;
    _ADC1_SAMPT2.bit.SAMP15 = sampling_time;

    // ADC2 regular channel configuration
    _ADC2_RSEQ3.bit.LEN = 1;
//    _ADC2_RSEQ1.bit.SEQ1 = 0;
//    _ADC2_RSEQ1.bit.SEQ2 = 1;
//    _ADC2_RSEQ1.bit.SEQ3 = 11;
//    _ADC2_RSEQ1.bit.SEQ4 = 12;
//    _ADC2_RSEQ1.bit.SEQ5 = 16;
//    _ADC2_RSEQ1.bit.SEQ6 = 17;
    _ADC2_SAMPT1.bit.SAMP0 = sampling_time;
    _ADC2_SAMPT1.bit.SAMP1 = sampling_time;
    _ADC2_SAMPT2.bit.SAMP11 = sampling_time;
    _ADC2_SAMPT2.bit.SAMP12 = sampling_time;
    _ADC2_SAMPT3.bit.SAMP16 = sampling_time;
    _ADC2_SAMPT3.bit.SAMP17 = sampling_time;

    // ADC3 regular channel configuration
    _ADC3_RSEQ3.bit.LEN = 1;
//    _ADC3_RSEQ1.bit.SEQ1 = 4;
//    _ADC3_RSEQ1.bit.SEQ2 = 6;
//    _ADC3_RSEQ1.bit.SEQ3 = 8;
//    _ADC3_RSEQ1.bit.SEQ4 = 9;
//    _ADC3_RSEQ1.bit.SEQ5 = 11;
//    _ADC3_RSEQ1.bit.SEQ6 = 14;
    _ADC3_SAMPT1.bit.SAMP4 = sampling_time;
    _ADC3_SAMPT1.bit.SAMP6 = sampling_time;
    _ADC3_SAMPT2.bit.SAMP8 = sampling_time;
    _ADC3_SAMPT2.bit.SAMP9 = sampling_time;
    _ADC3_SAMPT2.bit.SAMP11 = sampling_time;
    _ADC3_SAMPT2.bit.SAMP14 = sampling_time;

    // ADC4 regular channel configuration
    _ADC4_RSEQ3.bit.LEN = 1;
//    _ADC4_RSEQ1.bit.SEQ1 = 0;
//    _ADC4_RSEQ1.bit.SEQ2 = 2;
//    _ADC4_RSEQ1.bit.SEQ3 = 3;
//    _ADC4_RSEQ1.bit.SEQ4 = 4;
//    _ADC4_RSEQ1.bit.SEQ5 = 10;
//    _ADC4_RSEQ1.bit.SEQ6 = 16;
    _ADC4_SAMPT1.bit.SAMP0 = sampling_time;
    _ADC4_SAMPT1.bit.SAMP2 = sampling_time;
    _ADC4_SAMPT1.bit.SAMP3 = sampling_time;
    _ADC4_SAMPT1.bit.SAMP4 = sampling_time;
    _ADC4_SAMPT2.bit.SAMP10 = sampling_time;
    _ADC4_SAMPT3.bit.SAMP16 = sampling_time;

    // Disable DMA Mode 1
    _ADC1_CTRL2.bit.DMAMD = 0;
    _ADC2_CTRL2.bit.DMAMD = 0;
    _ADC3_CTRL2.bit.DMAMD = 0;
    _ADC4_CTRL2.bit.DMAMD = 0;

    // Enable ADC
    _ADC1_CTRL2.bit.ON = 1;
    _ADC2_CTRL2.bit.ON = 1;
    _ADC3_CTRL2.bit.ON = 1;
    _ADC4_CTRL2.bit.ON = 1;
    while (!_ADC1_STS.bit.RDY);
    while (!_ADC2_STS.bit.RDY);
    while (!_ADC3_STS.bit.RDY);
    while (!_ADC4_STS.bit.RDY);

    // Calibration
    _ADC1_CTRL3.bit.CALDIF = 0;
    _ADC2_CTRL3.bit.CALDIF = 0;
    _ADC3_CTRL3.bit.CALDIF = 0;
    _ADC4_CTRL3.bit.CALDIF = 0;
    _ADC1_CTRL2.bit.ENCAL = 1;
    _ADC2_CTRL2.bit.ENCAL = 1;
    _ADC3_CTRL2.bit.ENCAL = 1;
    _ADC4_CTRL2.bit.ENCAL = 1;
    while (_ADC1_CTRL2.bit.ENCAL);
    while (_ADC2_CTRL2.bit.ENCAL);
    while (_ADC3_CTRL2.bit.ENCAL);
    while (_ADC4_CTRL2.bit.ENCAL);

    // Turn off start of conversion
    _ADC1_CTRL2.bit.SWSTRRCH = 0;
    _ADC2_CTRL2.bit.SWSTRRCH = 0;
    _ADC3_CTRL2.bit.SWSTRRCH = 0;
    _ADC4_CTRL2.bit.SWSTRRCH = 0;
}

/**************************************************************************************************
Function Name:
    void Setup_SHRTIM(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup SHRTIM for PWM output
**************************************************************************************************/
void Setup_SHRTIM(void)
{
    SHRTIM_TIM_SetPrescaler(SHRTIM1, SHRTIM_TIMER_A, SHRTIM_PRESCALERRATIO_DIV4);
    SHRTIM_TIM_SetPrescaler(SHRTIM1, SHRTIM_TIMER_B, SHRTIM_PRESCALERRATIO_DIV4);
    SHRTIM_TIM_SetPrescaler(SHRTIM1, SHRTIM_TIMER_E, SHRTIM_PRESCALERRATIO_DIV4);
    SHRTIM_TIM_SetPrescaler(SHRTIM1, SHRTIM_TIMER_F, SHRTIM_PRESCALERRATIO_DIV4);

    SHRTIM_TIM_SetCounterMode(SHRTIM1, SHRTIM_TIMER_A, SHRTIM_MODE_CONTINUOUS);
    SHRTIM_TIM_SetCounterMode(SHRTIM1, SHRTIM_TIMER_B, SHRTIM_MODE_CONTINUOUS);
    SHRTIM_TIM_SetCounterMode(SHRTIM1, SHRTIM_TIMER_E, SHRTIM_MODE_CONTINUOUS);
    SHRTIM_TIM_SetCounterMode(SHRTIM1, SHRTIM_TIMER_F, SHRTIM_MODE_CONTINUOUS);

    SHRTIM_TIM_SetPeriod(SHRTIM1, SHRTIM_TIMER_A, SHRTIM_Period);
    SHRTIM_TIM_SetPeriod(SHRTIM1, SHRTIM_TIMER_B, SHRTIM_Period);
    SHRTIM_TIM_SetPeriod(SHRTIM1, SHRTIM_TIMER_E, SHRTIM_Period);
    SHRTIM_TIM_SetPeriod(SHRTIM1, SHRTIM_TIMER_F, SHRTIM_Period);

    SHRTIM_TIM_SetCountingMode(SHRTIM1, SHRTIM_TIMER_A, SHRTIM_COUNTING_MODE_UP);
    SHRTIM_TIM_SetCountingMode(SHRTIM1, SHRTIM_TIMER_B, SHRTIM_COUNTING_MODE_UP);
    SHRTIM_TIM_SetCountingMode(SHRTIM1, SHRTIM_TIMER_E, SHRTIM_COUNTING_MODE_UP);
    SHRTIM_TIM_SetCountingMode(SHRTIM1, SHRTIM_TIMER_F, SHRTIM_COUNTING_MODE_UP);

    SHRTIM_ForceUpdate(SHRTIM1, SHRTIM_TIMER_A);
    SHRTIM_ForceUpdate(SHRTIM1, SHRTIM_TIMER_B);
    SHRTIM_ForceUpdate(SHRTIM1, SHRTIM_TIMER_E);
    SHRTIM_ForceUpdate(SHRTIM1, SHRTIM_TIMER_F);

    SHRTIM_TIM_EnableResyncUpdate(SHRTIM1, SHRTIM_TIMER_A);
    SHRTIM_TIM_EnableResyncUpdate(SHRTIM1, SHRTIM_TIMER_B);
    SHRTIM_TIM_EnableResyncUpdate(SHRTIM1, SHRTIM_TIMER_E);
    SHRTIM_TIM_EnableResyncUpdate(SHRTIM1, SHRTIM_TIMER_F);

    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TA1, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TA2, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TB1, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TB2, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TE1, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TE2, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TF1, SHRTIM_OUT_POSITIVE_POLARITY);
    SHRTIM_OUT_SetPolarity(SHRTIM1, SHRTIM_OUTPUT_TF2, SHRTIM_OUT_POSITIVE_POLARITY);

    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TA1, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TA2, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TB1, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TB2, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TE1, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TE2, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TF1, SHRTIM_OUT_IDLELEVEL_INACTIVE);
    SHRTIM_OUT_SetIdleLevel(SHRTIM1, SHRTIM_OUTPUT_TF2, SHRTIM_OUT_IDLELEVEL_INACTIVE);

    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TA1, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TA2, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TB1, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TB2, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TE1, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TE2, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TF1, SHRTIM_OUTPUTSET_TIMPER);
    SHRTIM_OUT_SetOutputSetSrc(SHRTIM1, SHRTIM_OUTPUT_TF2, SHRTIM_OUTPUTSET_TIMPER);

    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TA1, SHRTIM_OUTPUTRESET_TIMCMP1);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TA2, SHRTIM_OUTPUTRESET_TIMCMP3);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TB1, SHRTIM_OUTPUTRESET_TIMCMP1);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TB2, SHRTIM_OUTPUTRESET_TIMCMP3);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TE1, SHRTIM_OUTPUTRESET_TIMCMP1);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TE2, SHRTIM_OUTPUTRESET_TIMCMP3);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TF1, SHRTIM_OUTPUTRESET_TIMCMP1);
    SHRTIM_OUT_SetOutputResetSrc(SHRTIM1, SHRTIM_OUTPUT_TF2, SHRTIM_OUTPUTRESET_TIMCMP3);

    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TA1);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TA2);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TB1);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TB2);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TE1);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TE2);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TF1);
    SHRTIM_EnableOutput(SHRTIM1, SHRTIM_OUTPUT_TF2);

    SHRTIM_TIM_CounterEnable(SHRTIM1, SHRTIM_TIMER_A);
    SHRTIM_TIM_CounterEnable(SHRTIM1, SHRTIM_TIMER_B);
    SHRTIM_TIM_CounterEnable(SHRTIM1, SHRTIM_TIMER_E);
    SHRTIM_TIM_CounterEnable(SHRTIM1, SHRTIM_TIMER_F);
}

/**************************************************************************************************
Function Name:
    void Setup_ATIM(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup ATIM for PWM output
**************************************************************************************************/
void Setup_ATIM(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;

    /* Compute the value to be set in AR register to generate signal frequency at x hz */
    uint16_t Channel1Pulse = 0;
    uint16_t Channel2Pulse = 0;
    uint16_t Channel3Pulse = 0;
    uint16_t Channel4Pulse = 0;

    /* Time Base configuration */
    TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Prescaler     = 3; // 3+1 = 4
    TIM_TimeBaseStructure.CounterMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period        = ATIM_Period;
    TIM_TimeBaseStructure.ClkDiv        = TIM_CLK_DIV1;
    TIM_InitTimeBase(ATIM1, &TIM_TimeBaseStructure);
    TIM_InitTimeBase(ATIM2, &TIM_TimeBaseStructure);

    /* Channel 1, 2 , 3 and 4 Configuration in PWM mode */
    TIM_InitOcStruct(&TIM_OCInitStructure);
    TIM_OCInitStructure.OCMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse        = Channel1Pulse;
    TIM_OCInitStructure.OCPolarity   = TIM_OC_POLARITY_LOW;
    TIM_OCInitStructure.OCIdleState  = TIM_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.OCNIdleState = TIM_OCN_IDLE_STATE_RESET;

    TIM_InitOc1(ATIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel2Pulse;
    TIM_InitOc2(ATIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel3Pulse;
    TIM_InitOc3(ATIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel4Pulse;
    TIM_InitOc4(ATIM1, &TIM_OCInitStructure);

    /* Channel 1, 2 , 3 and 4 Configuration in PWM mode */
    TIM_InitOcStruct(&TIM_OCInitStructure);
    TIM_OCInitStructure.OCMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputNState = TIM_OUTPUT_NSTATE_ENABLE;
    TIM_OCInitStructure.Pulse        = Channel1Pulse;
    TIM_OCInitStructure.OCNPolarity  = TIM_OCN_POLARITY_LOW;
    TIM_OCInitStructure.OCIdleState  = TIM_OC_IDLE_STATE_RESET;
    TIM_OCInitStructure.OCNIdleState = TIM_OCN_IDLE_STATE_RESET;

    TIM_InitOc1(ATIM2, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel2Pulse;
    TIM_InitOc2(ATIM2, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel3Pulse;
    TIM_InitOc3(ATIM2, &TIM_OCInitStructure);

    TIM_OCInitStructure.Pulse = Channel4Pulse;
    TIM_InitOc4(ATIM2, &TIM_OCInitStructure);

    /* ATIM1 counter enable */
    TIM_Enable(ATIM1, ENABLE);
    TIM_Enable(ATIM2, ENABLE);

    /* Main Output Enable */
    TIM_EnableCtrlPwmOutputs(ATIM1, ENABLE);
    TIM_EnableCtrlPwmOutputs(ATIM2, ENABLE);
}

/**************************************************************************************************
Function Name:
    void Setup_GTIM(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup GTIM for fan control PWM
**************************************************************************************************/
void Setup_GTIM(void)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    OCInitType TIM_OCInitStructure;

    uint16_t Channel4Pulse = 0;

    TIM_InitTimBaseStruct(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.Prescaler     = 1; // 1+1 = 2
    TIM_TimeBaseStructure.CounterMode   = TIM_CNT_MODE_UP;
    TIM_TimeBaseStructure.Period        = GTIM_Period;
    TIM_TimeBaseStructure.ClkDiv        = TIM_CLK_DIV1;
    TIM_InitTimeBase(GTIM7, &TIM_TimeBaseStructure);

    TIM_InitOcStruct(&TIM_OCInitStructure);
    TIM_OCInitStructure.OCMode       = TIM_OCMODE_PWM2;
    TIM_OCInitStructure.OutputState  = TIM_OUTPUT_STATE_ENABLE;
    TIM_OCInitStructure.Pulse        = Channel4Pulse;
    TIM_OCInitStructure.OCPolarity   = TIM_OC_POLARITY_LOW;

    TIM_InitOc4(GTIM7, &TIM_OCInitStructure);

    TIM_Enable(GTIM7, ENABLE);

    TIM_EnableCtrlPwmOutputs(GTIM7, ENABLE);
}
/***************************************************************************************************
Function Name:
    void Setup_UART(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup UART + DMA
***************************************************************************************************/
void Setup_UART(void)
{
    usart1.func.init(&usart1);
    usart2.func.init(&usart2);
    usart3.func.init(&usart3);
    usart4.func.init(&usart4);
    uart5.func.init(&uart5);
    uart6.func.init(&uart6);
    uart7.func.init(&uart7);
    uart8.func.init(&uart8);
}
/***************************************************************************************************
Function Name:
    void Setup_I2C(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup I2C
***************************************************************************************************/
void Setup_I2C(void)
{
    slaveI2c1.func.init(&slaveI2c1, I2C_SLAVE_ADDR);
    masterI2c2.func.init(&masterI2c2);
    masterI2c3.func.init(&masterI2c3);
    masterI2c4.func.init(&masterI2c4);
}
/***************************************************************************************************
Function Name:
    void Setup_SPI(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup I2C
***************************************************************************************************/
void Setup_SPI(void)
{
    masterSpi1.func.init(&masterSpi1);
}
/***************************************************************************************************
Function Name:
    void Setup_Interrupt(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup Interrupt
***************************************************************************************************/
void Setup_Interrupt(void)
{
    NVIC_SetPriority(USART1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(USART3_IRQn);
    NVIC_SetPriority(USART4_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(USART4_IRQn);
    NVIC_SetPriority(UART5_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(UART5_IRQn);
    NVIC_SetPriority(UART6_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(UART6_IRQn);
    NVIC_SetPriority(UART7_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(UART7_IRQn);
    NVIC_SetPriority(UART8_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
    NVIC_EnableIRQ(UART8_IRQn);
}
/**************************************************************************************************
Function Name:
    void Setup_Watchdog(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup 12s IWDG for system crash to reset MCU.
**************************************************************************************************/
void Setup_Watchdog(void)
{
//    // WWDGT clock = (APB1(60MHz) / 4096) / PSC(2^3) = 1831Hz (~546 us)
//    _WWDGT_CFG.bit.PSC = 3;

}
//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Peripheral_Setup(void)
Input:
    NULL
Output:
    NULL
Comment:
    Setup all peripheral.
**************************************************************************************************/
void Peripheral_Setup(void)
{
    // Disable ISR at first
    __disable_irq();

    Setup_SystemClock();
    Setup_PeripheralClock();
    Setup_GPIO();
    Setup_DMA();
    Setup_ADC();
    Setup_SHRTIM();
    Setup_ATIM();
    Setup_GTIM();
    Setup_UART();
    Setup_I2C();
    Setup_SPI();
    Setup_Interrupt();
//    Setup_Watchdog();
}

//----------------------------------------------------------------------------------------------------------------------
