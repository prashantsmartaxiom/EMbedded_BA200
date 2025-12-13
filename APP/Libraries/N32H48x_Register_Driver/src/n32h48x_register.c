/***********************************************************************************************************************
File name: n32h48x_register.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------
    None.                                   None.
External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------
    None.                                   None.
Description:
    N32H482xE and N32H487xE MCUs expansion peripheral register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    11/20/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h48x_register.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
// Define FLASH register ----------------------------------------
__no_init __root volatile _FLASHx_AC _FLASH_AC;
__no_init __root volatile _FLASHx_CTRL _FLASH_CTRL;
__no_init __root volatile _FLASHx_STS _FLASH_STS;
__no_init __root volatile _FLASHx_ADD _FLASH_ADD;
__no_init __root volatile _FLASHx_KEY _FLASH_KEY;
__no_init __root volatile _FLASHx_OPTKEY _FLASH_OPTKEY;
__no_init __root volatile _FLASHx_OB _FLASH_OB;
__no_init __root volatile _FLASHx_WRP _FLASH_WRP;
__no_init __root volatile _FLASHx_ECC _FLASH_ECC;
__no_init __root volatile _FLASHx_RDN _FLASH_RDN;
__no_init __root volatile _FLASHx_CAHR _FLASH_CAHR;
__no_init __root volatile _FLASHx_ROWP _FLASH_ROWP;

// Define RCC register ----------------------------------------
__no_init __root volatile _RCCx_CTRL _RCC_CTRL;
__no_init __root volatile _RCCx_CFG _RCC_CFG;
__no_init __root volatile _RCCx_CFG2 _RCC_CFG2;
__no_init __root volatile _RCCx_CFG3 _RCC_CFG3;
__no_init __root volatile _RCCx_CLKINT _RCC_CLKINT;
__no_init __root volatile _RCCx_CTRLSTS _RCC_CTRLSTS;
__no_init __root volatile _RCCx_AHBPCLKEN _RCC_AHBPCLKEN;
__no_init __root volatile _RCCx_APB1PCLKEN _RCC_APB1PCLKEN;
__no_init __root volatile _RCCx_APB2PCLKEN _RCC_APB2PCLKEN;
__no_init __root volatile _RCCx_AHBPRST _RCC_AHBPRST;
__no_init __root volatile _RCCx_APB1PRST _RCC_APB1PRST;
__no_init __root volatile _RCCx_APB2PRST _RCC_APB2PRST;
__no_init __root volatile _RCCx_MCOCFG _RCC_MCOCFG;
__no_init __root volatile _RCCx_BDCTRL _RCC_BDCTRL;
__no_init __root volatile _RCCx_CSSCTRL _RCC_CSSCTRL;
__no_init __root volatile _RCCx_AHB1PCLKEN _RCC_AHB1PCLKEN;
__no_init __root volatile _RCCx_PLLCTRL _RCC_PLLCTRL;
__no_init __root volatile _RCCx_VREFCTRL _RCC_VREFCTRL;
__no_init __root volatile _RCCx_SHRTPLLCTRL1 _RCC_SHRTPLLCTRL1;
__no_init __root volatile _RCCx_SHRTPLLCTRL2 _RCC_SHRTPLLCTRL2;
__no_init __root volatile _RCCx_SHRTPLLCTRL3 _RCC_SHRTPLLCTRL3;
__no_init __root volatile _RCCx_SRAMCFG1 _RCC_SRAMCFG1;
__no_init __root volatile _RCCx_SRAMCFG2 _RCC_SRAMCFG2;
__no_init __root volatile _RCCx_SRAMCFG3 _RCC_SRAMCFG3;
__no_init __root volatile _RCCx_SRAMCFG4 _RCC_SRAMCFG4;
__no_init __root volatile _RCCx_USBHSCTRL1 _RCC_USBHSCTRL1;
__no_init __root volatile _RCCx_USBHSCTRL2 _RCC_USBHSCTRL2;
__no_init __root volatile _RCCx_BOOTREMAP _RCC_BOOTREMAP;
__no_init __root volatile _RCCx_FDCANBADDR _RCC_FDCANBADDR;

// Define GPIO register ----------------------------------------
__no_init __root volatile _GPIOx_PMODE _GPIOA_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOA_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOA_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOA_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOA_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOA_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOA_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOA_DS;
__no_init __root volatile _GPIOx_SR _GPIOA_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOA_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOA_PBC;
__no_init __root volatile _GPIOx_PID _GPIOA_PID;
__no_init __root volatile _GPIOx_POD _GPIOA_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOA_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOB_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOB_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOB_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOB_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOB_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOB_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOB_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOB_DS;
__no_init __root volatile _GPIOx_SR _GPIOB_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOB_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOB_PBC;
__no_init __root volatile _GPIOx_PID _GPIOB_PID;
__no_init __root volatile _GPIOx_POD _GPIOB_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOB_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOC_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOC_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOC_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOC_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOC_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOC_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOC_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOC_DS;
__no_init __root volatile _GPIOx_SR _GPIOC_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOC_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOC_PBC;
__no_init __root volatile _GPIOx_PID _GPIOC_PID;
__no_init __root volatile _GPIOx_POD _GPIOC_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOC_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOD_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOD_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOD_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOD_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOD_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOD_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOD_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOD_DS;
__no_init __root volatile _GPIOx_SR _GPIOD_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOD_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOD_PBC;
__no_init __root volatile _GPIOx_PID _GPIOD_PID;
__no_init __root volatile _GPIOx_POD _GPIOD_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOD_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOE_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOE_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOE_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOE_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOE_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOE_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOE_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOE_DS;
__no_init __root volatile _GPIOx_SR _GPIOE_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOE_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOE_PBC;
__no_init __root volatile _GPIOx_PID _GPIOE_PID;
__no_init __root volatile _GPIOx_POD _GPIOE_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOE_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOF_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOF_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOF_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOF_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOF_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOF_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOF_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOF_DS;
__no_init __root volatile _GPIOx_SR _GPIOF_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOF_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOF_PBC;
__no_init __root volatile _GPIOx_PID _GPIOF_PID;
__no_init __root volatile _GPIOx_POD _GPIOF_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOF_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOG_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOG_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOG_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOG_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOG_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOG_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOG_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOG_DS;
__no_init __root volatile _GPIOx_SR _GPIOG_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOG_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOG_PBC;
__no_init __root volatile _GPIOx_PID _GPIOG_PID;
__no_init __root volatile _GPIOx_POD _GPIOG_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOG_PLOCK;

__no_init __root volatile _GPIOx_PMODE _GPIOH_PMODE;
__no_init __root volatile _GPIOx_PUPD _GPIOH_PUPD;
__no_init __root volatile _GPIOx_POTYPE _GPIOH_POTYPE;
__no_init __root volatile _GPIOx_AFSEL0 _GPIOH_AFSEL0;
__no_init __root volatile _GPIOx_AFSEL1 _GPIOH_AFSEL1;
__no_init __root volatile _GPIOx_AFSEL2 _GPIOH_AFSEL2;
__no_init __root volatile _GPIOx_AFSEL3 _GPIOH_AFSEL3;
__no_init __root volatile _GPIOx_DS _GPIOH_DS;
__no_init __root volatile _GPIOx_SR _GPIOH_SR;
__no_init __root volatile _GPIOx_PBSC _GPIOH_PBSC;
__no_init __root volatile _GPIOx_PBC _GPIOH_PBC;
__no_init __root volatile _GPIOx_PID _GPIOH_PID;
__no_init __root volatile _GPIOx_POD _GPIOH_POD;
__no_init __root volatile _GPIOx_PLOCK _GPIOH_PLOCK;

// Define AFIO register ----------------------------------------
__no_init __root volatile _AFIOx_RMP_CFG _AFIO_RMP_CFG;
__no_init __root volatile _AFIOx_EXTI_CFG1 _AFIO_EXTI_CFG1;
__no_init __root volatile _AFIOx_EXTI_CFG2 _AFIO_EXTI_CFG2;
__no_init __root volatile _AFIOx_EXTI_CFG3 _AFIO_EXTI_CFG3;
__no_init __root volatile _AFIOx_EXTI_CFG4 _AFIO_EXTI_CFG4;
__no_init __root volatile _AFIOx_TOL5V_CFG1 _AFIO_TOL5V_CFG1;
__no_init __root volatile _AFIOx_TOL5V_CFG2 _AFIO_TOL5V_CFG2;
__no_init __root volatile _AFIOx_TOL5V_CFG3 _AFIO_TOL5V_CFG3;
__no_init __root volatile _AFIOx_TOL5V_CFG4 _AFIO_TOL5V_CFG4;
__no_init __root volatile _AFIOx_FILTER_CFG1 _AFIO_FILTER_CFG1;
__no_init __root volatile _AFIOx_FILTER_CFG2 _AFIO_FILTER_CFG2;
__no_init __root volatile _AFIOx_FILTER_CFG3 _AFIO_FILTER_CFG3;
__no_init __root volatile _AFIOx_FILTER_CFG4 _AFIO_FILTER_CFG4;
__no_init __root volatile _AFIOx_EMC_CFG _AFIO_EMC_CFG;
__no_init __root volatile _AFIOx_EMC_CNT _AFIO_EMC_CNT;
__no_init __root volatile _AFIOx_SHRT_EXEV_CFG1 _AFIO_SHRT_EXEV_CFG1;
__no_init __root volatile _AFIOx_SHRT_EXEV_CFG2 _AFIO_SHRT_EXEV_CFG2;
__no_init __root volatile _AFIOx_SHRT_EXEV_CFG3 _AFIO_SHRT_EXEV_CFG3;

// Define DMA register ----------------------------------------
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG1;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG2;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG3;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG4;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG5;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG6;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG7;
__no_init __root volatile _DMAx_CHCFGy _DMA1_CHCFG8;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR1;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR2;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR3;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR4;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR5;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR6;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR7;
__no_init __root volatile _DMAx_MADDRy _DMA1_MADDR8;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR1;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR2;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR3;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR4;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR5;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR6;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR7;
__no_init __root volatile _DMAx_PADDRy _DMA1_PADDR8;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL1;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL2;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL3;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL4;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL5;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL6;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL7;
__no_init __root volatile _DMAx_CHSELy _DMA1_CHSEL8;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM1;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM2;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM3;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM4;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM5;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM6;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM7;
__no_init __root volatile _DMAx_TXNUMy _DMA1_TXNUM8;
__no_init __root volatile _DMAx_INTCLR _DMA1_INTCLR;
__no_init __root volatile _DMAx_INTSTS _DMA1_INTSTS;
__no_init __root volatile _DMAx_BURSTEN _DMA1_BURSTEN;

__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG1;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG2;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG3;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG4;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG5;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG6;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG7;
__no_init __root volatile _DMAx_CHCFGy _DMA2_CHCFG8;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR1;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR2;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR3;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR4;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR5;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR6;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR7;
__no_init __root volatile _DMAx_MADDRy _DMA2_MADDR8;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR1;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR2;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR3;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR4;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR5;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR6;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR7;
__no_init __root volatile _DMAx_PADDRy _DMA2_PADDR8;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL1;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL2;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL3;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL4;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL5;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL6;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL7;
__no_init __root volatile _DMAx_CHSELy _DMA2_CHSEL8;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM1;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM2;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM3;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM4;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM5;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM6;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM7;
__no_init __root volatile _DMAx_TXNUMy _DMA2_TXNUM8;
__no_init __root volatile _DMAx_INTCLR _DMA2_INTCLR;
__no_init __root volatile _DMAx_INTSTS _DMA2_INTSTS;
__no_init __root volatile _DMAx_BURSTEN _DMA2_BURSTEN;

// Define ADC register ----------------------------------------
__no_init __root volatile _ADCx_STS _ADC1_STS;
__no_init __root volatile _ADCx_CTRL1 _ADC1_CTRL1;
__no_init __root volatile _ADCx_CTRL2 _ADC1_CTRL2;
__no_init __root volatile _ADCx_CTRL3 _ADC1_CTRL3;
__no_init __root volatile _ADCx_SAMPT1 _ADC1_SAMPT1;
__no_init __root volatile _ADCx_SAMPT2 _ADC1_SAMPT2;
__no_init __root volatile _ADCx_SAMPT3 _ADC1_SAMPT3;
__no_init __root volatile _ADCx_DIFSEL _ADC1_DIFSEL;
__no_init __root volatile _ADCx_CALFACT _ADC1_CALFACT;
__no_init __root volatile _ADCx_OFFSET1 _ADC1_OFFSET1;
__no_init __root volatile _ADCx_OFFSET2 _ADC1_OFFSET2;
__no_init __root volatile _ADCx_OFFSET3 _ADC1_OFFSET3;
__no_init __root volatile _ADCx_OFFSET4 _ADC1_OFFSET4;
__no_init __root volatile _ADCx_AWD1HIGH _ADC1_AWD1HIGH;
__no_init __root volatile _ADCx_AWD1LOW _ADC1_AWD1LOW;
__no_init __root volatile _ADCx_AWD2HIGH _ADC1_AWD2HIGH;
__no_init __root volatile _ADCx_AWD2LOW _ADC1_AWD2LOW;
__no_init __root volatile _ADCx_AWD3HIGH _ADC1_AWD3HIGH;
__no_init __root volatile _ADCx_AWD3LOW _ADC1_AWD3LOW;
__no_init __root volatile _ADCx_AWD2EN _ADC1_AWD2EN;
__no_init __root volatile _ADCx_AWD3EN _ADC1_AWD3EN;
__no_init __root volatile _ADCx_AWD2INTEN _ADC1_AWD2INTEN;
__no_init __root volatile _ADCx_AWD3INTEN _ADC1_AWD3INTEN;
__no_init __root volatile _ADCx_AWD2STS _ADC1_AWD2STS;
__no_init __root volatile _ADCx_AWD3STS _ADC1_AWD3STS;
__no_init __root volatile _ADCx_RSEQ1 _ADC1_RSEQ1;
__no_init __root volatile _ADCx_RSEQ2 _ADC1_RSEQ2;
__no_init __root volatile _ADCx_RSEQ3 _ADC1_RSEQ3;
__no_init __root volatile _ADCx_JSEQ _ADC1_JSEQ;
__no_init __root volatile _ADCx_JDAT1 _ADC1_JDAT1;
__no_init __root volatile _ADCx_JDAT2 _ADC1_JDAT2;
__no_init __root volatile _ADCx_JDAT3 _ADC1_JDAT3;
__no_init __root volatile _ADCx_JDAT4 _ADC1_JDAT4;
__no_init __root volatile _ADCx_DAT _ADC1_DAT;
__no_init __root volatile _ADCx_FIFOCFG _ADC1_FIFOCFG;
__no_init __root volatile _ADCx_FIFOSTS _ADC1_FIFOSTS;
__no_init __root volatile _ADCx_PCTRL _ADC1_PCTRL;
__no_init __root volatile _ADCx_CTRL4 _ADC1_CTRL4;
__no_init __root volatile _ADCx_INTEN _ADC1_INTEN;
__no_init __root volatile _ADCx_GCOMP _ADC1_GCOMP;

__no_init __root volatile _ADCx_STS _ADC2_STS;
__no_init __root volatile _ADCx_CTRL1 _ADC2_CTRL1;
__no_init __root volatile _ADCx_CTRL2 _ADC2_CTRL2;
__no_init __root volatile _ADCx_CTRL3 _ADC2_CTRL3;
__no_init __root volatile _ADCx_SAMPT1 _ADC2_SAMPT1;
__no_init __root volatile _ADCx_SAMPT2 _ADC2_SAMPT2;
__no_init __root volatile _ADCx_SAMPT3 _ADC2_SAMPT3;
__no_init __root volatile _ADCx_DIFSEL _ADC2_DIFSEL;
__no_init __root volatile _ADCx_CALFACT _ADC2_CALFACT;
__no_init __root volatile _ADCx_OFFSET1 _ADC2_OFFSET1;
__no_init __root volatile _ADCx_OFFSET2 _ADC2_OFFSET2;
__no_init __root volatile _ADCx_OFFSET3 _ADC2_OFFSET3;
__no_init __root volatile _ADCx_OFFSET4 _ADC2_OFFSET4;
__no_init __root volatile _ADCx_AWD1HIGH _ADC2_AWD1HIGH;
__no_init __root volatile _ADCx_AWD1LOW _ADC2_AWD1LOW;
__no_init __root volatile _ADCx_AWD2HIGH _ADC2_AWD2HIGH;
__no_init __root volatile _ADCx_AWD2LOW _ADC2_AWD2LOW;
__no_init __root volatile _ADCx_AWD3HIGH _ADC2_AWD3HIGH;
__no_init __root volatile _ADCx_AWD3LOW _ADC2_AWD3LOW;
__no_init __root volatile _ADCx_AWD2EN _ADC2_AWD2EN;
__no_init __root volatile _ADCx_AWD3EN _ADC2_AWD3EN;
__no_init __root volatile _ADCx_AWD2INTEN _ADC2_AWD2INTEN;
__no_init __root volatile _ADCx_AWD3INTEN _ADC2_AWD3INTEN;
__no_init __root volatile _ADCx_AWD2STS _ADC2_AWD2STS;
__no_init __root volatile _ADCx_AWD3STS _ADC2_AWD3STS;
__no_init __root volatile _ADCx_RSEQ1 _ADC2_RSEQ1;
__no_init __root volatile _ADCx_RSEQ2 _ADC2_RSEQ2;
__no_init __root volatile _ADCx_RSEQ3 _ADC2_RSEQ3;
__no_init __root volatile _ADCx_JSEQ _ADC2_JSEQ;
__no_init __root volatile _ADCx_JDAT1 _ADC2_JDAT1;
__no_init __root volatile _ADCx_JDAT2 _ADC2_JDAT2;
__no_init __root volatile _ADCx_JDAT3 _ADC2_JDAT3;
__no_init __root volatile _ADCx_JDAT4 _ADC2_JDAT4;
__no_init __root volatile _ADCx_DAT _ADC2_DAT;
__no_init __root volatile _ADCx_FIFOCFG _ADC2_FIFOCFG;
__no_init __root volatile _ADCx_FIFOSTS _ADC2_FIFOSTS;
__no_init __root volatile _ADCx_PCTRL _ADC2_PCTRL;
__no_init __root volatile _ADCx_CTRL4 _ADC2_CTRL4;
__no_init __root volatile _ADCx_INTEN _ADC2_INTEN;
__no_init __root volatile _ADCx_GCOMP _ADC2_GCOMP;

__no_init __root volatile _ADCx_STS _ADC3_STS;
__no_init __root volatile _ADCx_CTRL1 _ADC3_CTRL1;
__no_init __root volatile _ADCx_CTRL2 _ADC3_CTRL2;
__no_init __root volatile _ADCx_CTRL3 _ADC3_CTRL3;
__no_init __root volatile _ADCx_SAMPT1 _ADC3_SAMPT1;
__no_init __root volatile _ADCx_SAMPT2 _ADC3_SAMPT2;
__no_init __root volatile _ADCx_SAMPT3 _ADC3_SAMPT3;
__no_init __root volatile _ADCx_DIFSEL _ADC3_DIFSEL;
__no_init __root volatile _ADCx_CALFACT _ADC3_CALFACT;
__no_init __root volatile _ADCx_OFFSET1 _ADC3_OFFSET1;
__no_init __root volatile _ADCx_OFFSET2 _ADC3_OFFSET2;
__no_init __root volatile _ADCx_OFFSET3 _ADC3_OFFSET3;
__no_init __root volatile _ADCx_OFFSET4 _ADC3_OFFSET4;
__no_init __root volatile _ADCx_AWD1HIGH _ADC3_AWD1HIGH;
__no_init __root volatile _ADCx_AWD1LOW _ADC3_AWD1LOW;
__no_init __root volatile _ADCx_AWD2HIGH _ADC3_AWD2HIGH;
__no_init __root volatile _ADCx_AWD2LOW _ADC3_AWD2LOW;
__no_init __root volatile _ADCx_AWD3HIGH _ADC3_AWD3HIGH;
__no_init __root volatile _ADCx_AWD3LOW _ADC3_AWD3LOW;
__no_init __root volatile _ADCx_AWD2EN _ADC3_AWD2EN;
__no_init __root volatile _ADCx_AWD3EN _ADC3_AWD3EN;
__no_init __root volatile _ADCx_AWD2INTEN _ADC3_AWD2INTEN;
__no_init __root volatile _ADCx_AWD3INTEN _ADC3_AWD3INTEN;
__no_init __root volatile _ADCx_AWD2STS _ADC3_AWD2STS;
__no_init __root volatile _ADCx_AWD3STS _ADC3_AWD3STS;
__no_init __root volatile _ADCx_RSEQ1 _ADC3_RSEQ1;
__no_init __root volatile _ADCx_RSEQ2 _ADC3_RSEQ2;
__no_init __root volatile _ADCx_RSEQ3 _ADC3_RSEQ3;
__no_init __root volatile _ADCx_JSEQ _ADC3_JSEQ;
__no_init __root volatile _ADCx_JDAT1 _ADC3_JDAT1;
__no_init __root volatile _ADCx_JDAT2 _ADC3_JDAT2;
__no_init __root volatile _ADCx_JDAT3 _ADC3_JDAT3;
__no_init __root volatile _ADCx_JDAT4 _ADC3_JDAT4;
__no_init __root volatile _ADCx_DAT _ADC3_DAT;
__no_init __root volatile _ADCx_FIFOCFG _ADC3_FIFOCFG;
__no_init __root volatile _ADCx_FIFOSTS _ADC3_FIFOSTS;
__no_init __root volatile _ADCx_PCTRL _ADC3_PCTRL;
__no_init __root volatile _ADCx_CTRL4 _ADC3_CTRL4;
__no_init __root volatile _ADCx_INTEN _ADC3_INTEN;
__no_init __root volatile _ADCx_GCOMP _ADC3_GCOMP;

__no_init __root volatile _ADCx_STS _ADC4_STS;
__no_init __root volatile _ADCx_CTRL1 _ADC4_CTRL1;
__no_init __root volatile _ADCx_CTRL2 _ADC4_CTRL2;
__no_init __root volatile _ADCx_CTRL3 _ADC4_CTRL3;
__no_init __root volatile _ADCx_SAMPT1 _ADC4_SAMPT1;
__no_init __root volatile _ADCx_SAMPT2 _ADC4_SAMPT2;
__no_init __root volatile _ADCx_SAMPT3 _ADC4_SAMPT3;
__no_init __root volatile _ADCx_DIFSEL _ADC4_DIFSEL;
__no_init __root volatile _ADCx_CALFACT _ADC4_CALFACT;
__no_init __root volatile _ADCx_OFFSET1 _ADC4_OFFSET1;
__no_init __root volatile _ADCx_OFFSET2 _ADC4_OFFSET2;
__no_init __root volatile _ADCx_OFFSET3 _ADC4_OFFSET3;
__no_init __root volatile _ADCx_OFFSET4 _ADC4_OFFSET4;
__no_init __root volatile _ADCx_AWD1HIGH _ADC4_AWD1HIGH;
__no_init __root volatile _ADCx_AWD1LOW _ADC4_AWD1LOW;
__no_init __root volatile _ADCx_AWD2HIGH _ADC4_AWD2HIGH;
__no_init __root volatile _ADCx_AWD2LOW _ADC4_AWD2LOW;
__no_init __root volatile _ADCx_AWD3HIGH _ADC4_AWD3HIGH;
__no_init __root volatile _ADCx_AWD3LOW _ADC4_AWD3LOW;
__no_init __root volatile _ADCx_AWD2EN _ADC4_AWD2EN;
__no_init __root volatile _ADCx_AWD3EN _ADC4_AWD3EN;
__no_init __root volatile _ADCx_AWD2INTEN _ADC4_AWD2INTEN;
__no_init __root volatile _ADCx_AWD3INTEN _ADC4_AWD3INTEN;
__no_init __root volatile _ADCx_AWD2STS _ADC4_AWD2STS;
__no_init __root volatile _ADCx_AWD3STS _ADC4_AWD3STS;
__no_init __root volatile _ADCx_RSEQ1 _ADC4_RSEQ1;
__no_init __root volatile _ADCx_RSEQ2 _ADC4_RSEQ2;
__no_init __root volatile _ADCx_RSEQ3 _ADC4_RSEQ3;
__no_init __root volatile _ADCx_JSEQ _ADC4_JSEQ;
__no_init __root volatile _ADCx_JDAT1 _ADC4_JDAT1;
__no_init __root volatile _ADCx_JDAT2 _ADC4_JDAT2;
__no_init __root volatile _ADCx_JDAT3 _ADC4_JDAT3;
__no_init __root volatile _ADCx_JDAT4 _ADC4_JDAT4;
__no_init __root volatile _ADCx_DAT _ADC4_DAT;
__no_init __root volatile _ADCx_FIFOCFG _ADC4_FIFOCFG;
__no_init __root volatile _ADCx_FIFOSTS _ADC4_FIFOSTS;
__no_init __root volatile _ADCx_PCTRL _ADC4_PCTRL;
__no_init __root volatile _ADCx_CTRL4 _ADC4_CTRL4;
__no_init __root volatile _ADCx_INTEN _ADC4_INTEN;
__no_init __root volatile _ADCx_GCOMP _ADC4_GCOMP;

// Define SPI and I2S register ----------------------------------------
__no_init __root volatile _SPIx_CTRL1 _SPI1_CTRL1;
__no_init __root volatile _SPIx_CTRL2 _SPI1_CTRL2;
__no_init __root volatile _SPIx_STS _SPI1_STS;
__no_init __root volatile _SPIx_DAT _SPI1_DAT;
__no_init __root volatile _SPIx_CRCTDAT _SPI1_CRCTDAT;
__no_init __root volatile _SPIx_CRCRDAT _SPI1_CRCRDAT;
__no_init __root volatile _SPIx_CRCPOLY _SPI1_CRCPOLY;
__no_init __root volatile _SPIx_RX_FIFO _SPI1_RX_FIFO;
__no_init __root volatile _SPIx_FIFO_NUM _SPI1_FIFO_NUM;
__no_init __root volatile _SPIx_FIFO_CNT _SPI1_FIFO_CNT;
__no_init __root volatile _SPIx_TRANS_NUM _SPI1_TRANS_NUM;
__no_init __root volatile _SPIx_CR3 _SPI1_CR3;

__no_init __root volatile _SPIx_CTRL1 _SPI2_CTRL1;
__no_init __root volatile _SPIx_CTRL2 _SPI2_CTRL2;
__no_init __root volatile _SPIx_STS _SPI2_STS;
__no_init __root volatile _SPIx_DAT _SPI2_DAT;
__no_init __root volatile _SPIx_CRCTDAT _SPI2_CRCTDAT;
__no_init __root volatile _SPIx_CRCRDAT _SPI2_CRCRDAT;
__no_init __root volatile _SPIx_CRCPOLY _SPI2_CRCPOLY;
__no_init __root volatile _SPIx_RX_FIFO _SPI2_RX_FIFO;
__no_init __root volatile _SPIx_FIFO_NUM _SPI2_FIFO_NUM;
__no_init __root volatile _SPIx_FIFO_CNT _SPI2_FIFO_CNT;
__no_init __root volatile _SPIx_TRANS_NUM _SPI2_TRANS_NUM;
__no_init __root volatile _SPIx_CR3 _SPI2_CR3;
__no_init __root volatile _SPIx_I2Sx_CFGR _SPI2_I2S2_CFGR;
__no_init __root volatile _SPIx_I2Sx_PREDIV _SPI2_I2S2_PREDIV;
__no_init __root volatile _I2Sx_CTRL2 _I2S2_CTRL2;
__no_init __root volatile _I2Sx_STS _I2S2_STS;
__no_init __root volatile _I2Sx_DAT _I2S2_DAT;
__no_init __root volatile _I2Sx_CFGR _I2S2_CFGR;

__no_init __root volatile _SPIx_CTRL1 _SPI3_CTRL1;
__no_init __root volatile _SPIx_CTRL2 _SPI3_CTRL2;
__no_init __root volatile _SPIx_STS _SPI3_STS;
__no_init __root volatile _SPIx_DAT _SPI3_DAT;
__no_init __root volatile _SPIx_CRCTDAT _SPI3_CRCTDAT;
__no_init __root volatile _SPIx_CRCRDAT _SPI3_CRCRDAT;
__no_init __root volatile _SPIx_CRCPOLY _SPI3_CRCPOLY;
__no_init __root volatile _SPIx_RX_FIFO _SPI3_RX_FIFO;
__no_init __root volatile _SPIx_FIFO_NUM _SPI3_FIFO_NUM;
__no_init __root volatile _SPIx_FIFO_CNT _SPI3_FIFO_CNT;
__no_init __root volatile _SPIx_TRANS_NUM _SPI3_TRANS_NUM;
__no_init __root volatile _SPIx_CR3 _SPI3_CR3;
__no_init __root volatile _SPIx_I2Sx_CFGR _SPI3_I2S3_CFGR;
__no_init __root volatile _SPIx_I2Sx_PREDIV _SPI3_I2S3_PREDIV;
__no_init __root volatile _I2Sx_CTRL2 _I2S3_CTRL2;
__no_init __root volatile _I2Sx_STS _I2S3_STS;
__no_init __root volatile _I2Sx_DAT _I2S3_DAT;
__no_init __root volatile _I2Sx_CFGR _I2S3_CFGR;

__no_init __root volatile _SPIx_CTRL1 _SPI4_CTRL1;
__no_init __root volatile _SPIx_CTRL2 _SPI4_CTRL2;
__no_init __root volatile _SPIx_STS _SPI4_STS;
__no_init __root volatile _SPIx_DAT _SPI4_DAT;
__no_init __root volatile _SPIx_CRCTDAT _SPI4_CRCTDAT;
__no_init __root volatile _SPIx_CRCRDAT _SPI4_CRCRDAT;
__no_init __root volatile _SPIx_CRCPOLY _SPI4_CRCPOLY;
__no_init __root volatile _SPIx_RX_FIFO _SPI4_RX_FIFO;
__no_init __root volatile _SPIx_FIFO_NUM _SPI4_FIFO_NUM;
__no_init __root volatile _SPIx_FIFO_CNT _SPI4_FIFO_CNT;
__no_init __root volatile _SPIx_TRANS_NUM _SPI4_TRANS_NUM;
__no_init __root volatile _SPIx_CR3 _SPI4_CR3;

__no_init __root volatile _SPIx_CTRL1 _SPI5_CTRL1;
__no_init __root volatile _SPIx_CTRL2 _SPI5_CTRL2;
__no_init __root volatile _SPIx_STS _SPI5_STS;
__no_init __root volatile _SPIx_DAT _SPI5_DAT;
__no_init __root volatile _SPIx_CRCTDAT _SPI5_CRCTDAT;
__no_init __root volatile _SPIx_CRCRDAT _SPI5_CRCRDAT;
__no_init __root volatile _SPIx_CRCPOLY _SPI5_CRCPOLY;
__no_init __root volatile _SPIx_RX_FIFO _SPI5_RX_FIFO;
__no_init __root volatile _SPIx_FIFO_NUM _SPI5_FIFO_NUM;
__no_init __root volatile _SPIx_FIFO_CNT _SPI5_FIFO_CNT;
__no_init __root volatile _SPIx_TRANS_NUM _SPI5_TRANS_NUM;
__no_init __root volatile _SPIx_CR3 _SPI5_CR3;

__no_init __root volatile _SPIx_CTRL1 _SPI6_CTRL1;
__no_init __root volatile _SPIx_CTRL2 _SPI6_CTRL2;
__no_init __root volatile _SPIx_STS _SPI6_STS;
__no_init __root volatile _SPIx_DAT _SPI6_DAT;
__no_init __root volatile _SPIx_CRCTDAT _SPI6_CRCTDAT;
__no_init __root volatile _SPIx_CRCRDAT _SPI6_CRCRDAT;
__no_init __root volatile _SPIx_CRCPOLY _SPI6_CRCPOLY;
__no_init __root volatile _SPIx_RX_FIFO _SPI6_RX_FIFO;
__no_init __root volatile _SPIx_FIFO_NUM _SPI6_FIFO_NUM;
__no_init __root volatile _SPIx_FIFO_CNT _SPI6_FIFO_CNT;
__no_init __root volatile _SPIx_TRANS_NUM _SPI6_TRANS_NUM;
__no_init __root volatile _SPIx_CR3 _SPI6_CR3;

// Define I2C register ----------------------------------------
__no_init __root volatile _I2Cx_CTRL1 _I2C1_CTRL1;
__no_init __root volatile _I2Cx_CTRL2 _I2C1_CTRL2;
__no_init __root volatile _I2Cx_OADDR1 _I2C1_OADDR1;
__no_init __root volatile _I2Cx_OADDR2 _I2C1_OADDR2;
__no_init __root volatile _I2Cx_DAT _I2C1_DAT;
__no_init __root volatile _I2Cx_STS1 _I2C1_STS1;
__no_init __root volatile _I2Cx_STS2 _I2C1_STS2;
__no_init __root volatile _I2Cx_CLKCTRL _I2C1_CLKCTRL;
__no_init __root volatile _I2Cx_TMRISE _I2C1_TMRISE;
__no_init __root volatile _I2Cx_BYTENUM _I2C1_BYTENUM;
__no_init __root volatile _I2Cx_GFLTRCTRL _I2C1_GFLTRCTRL;
__no_init __root volatile _I2Cx_FIFODAT _I2C1_FIFODAT;

__no_init __root volatile _I2Cx_CTRL1 _I2C2_CTRL1;
__no_init __root volatile _I2Cx_CTRL2 _I2C2_CTRL2;
__no_init __root volatile _I2Cx_OADDR1 _I2C2_OADDR1;
__no_init __root volatile _I2Cx_OADDR2 _I2C2_OADDR2;
__no_init __root volatile _I2Cx_DAT _I2C2_DAT;
__no_init __root volatile _I2Cx_STS1 _I2C2_STS1;
__no_init __root volatile _I2Cx_STS2 _I2C2_STS2;
__no_init __root volatile _I2Cx_CLKCTRL _I2C2_CLKCTRL;
__no_init __root volatile _I2Cx_TMRISE _I2C2_TMRISE;
__no_init __root volatile _I2Cx_BYTENUM _I2C2_BYTENUM;
__no_init __root volatile _I2Cx_GFLTRCTRL _I2C2_GFLTRCTRL;
__no_init __root volatile _I2Cx_FIFODAT _I2C2_FIFODAT;

__no_init __root volatile _I2Cx_CTRL1 _I2C3_CTRL1;
__no_init __root volatile _I2Cx_CTRL2 _I2C3_CTRL2;
__no_init __root volatile _I2Cx_OADDR1 _I2C3_OADDR1;
__no_init __root volatile _I2Cx_OADDR2 _I2C3_OADDR2;
__no_init __root volatile _I2Cx_DAT _I2C3_DAT;
__no_init __root volatile _I2Cx_STS1 _I2C3_STS1;
__no_init __root volatile _I2Cx_STS2 _I2C3_STS2;
__no_init __root volatile _I2Cx_CLKCTRL _I2C3_CLKCTRL;
__no_init __root volatile _I2Cx_TMRISE _I2C3_TMRISE;
__no_init __root volatile _I2Cx_BYTENUM _I2C3_BYTENUM;
__no_init __root volatile _I2Cx_GFLTRCTRL _I2C3_GFLTRCTRL;
__no_init __root volatile _I2Cx_FIFODAT _I2C3_FIFODAT;

__no_init __root volatile _I2Cx_CTRL1 _I2C4_CTRL1;
__no_init __root volatile _I2Cx_CTRL2 _I2C4_CTRL2;
__no_init __root volatile _I2Cx_OADDR1 _I2C4_OADDR1;
__no_init __root volatile _I2Cx_OADDR2 _I2C4_OADDR2;
__no_init __root volatile _I2Cx_DAT _I2C4_DAT;
__no_init __root volatile _I2Cx_STS1 _I2C4_STS1;
__no_init __root volatile _I2Cx_STS2 _I2C4_STS2;
__no_init __root volatile _I2Cx_CLKCTRL _I2C4_CLKCTRL;
__no_init __root volatile _I2Cx_TMRISE _I2C4_TMRISE;
__no_init __root volatile _I2Cx_BYTENUM _I2C4_BYTENUM;
__no_init __root volatile _I2Cx_GFLTRCTRL _I2C4_GFLTRCTRL;
__no_init __root volatile _I2Cx_FIFODAT _I2C4_FIFODAT;

// Define USART register ----------------------------------------
__no_init __root volatile _USARTx_CTRL1 _USART1_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _USART1_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _USART1_CTRL3;
__no_init __root volatile _USARTx_STS _USART1_STS;
__no_init __root volatile _USARTx_DAT _USART1_DAT;
__no_init __root volatile _USARTx_BRCF _USART1_BRCF;
__no_init __root volatile _USARTx_GTP _USART1_GTP;
__no_init __root volatile _USARTx_FIFO _USART1_FIFO;
__no_init __root volatile _USARTx_IFW _USART1_IFW;
__no_init __root volatile _USARTx_RTO _USART1_RTO;

__no_init __root volatile _USARTx_CTRL1 _USART2_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _USART2_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _USART2_CTRL3;
__no_init __root volatile _USARTx_STS _USART2_STS;
__no_init __root volatile _USARTx_DAT _USART2_DAT;
__no_init __root volatile _USARTx_BRCF _USART2_BRCF;
__no_init __root volatile _USARTx_GTP _USART2_GTP;
__no_init __root volatile _USARTx_FIFO _USART2_FIFO;
__no_init __root volatile _USARTx_IFW _USART2_IFW;
__no_init __root volatile _USARTx_RTO _USART2_RTO;

__no_init __root volatile _USARTx_CTRL1 _USART3_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _USART3_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _USART3_CTRL3;
__no_init __root volatile _USARTx_STS _USART3_STS;
__no_init __root volatile _USARTx_DAT _USART3_DAT;
__no_init __root volatile _USARTx_BRCF _USART3_BRCF;
__no_init __root volatile _USARTx_GTP _USART3_GTP;
__no_init __root volatile _USARTx_FIFO _USART3_FIFO;
__no_init __root volatile _USARTx_IFW _USART3_IFW;
__no_init __root volatile _USARTx_RTO _USART3_RTO;

__no_init __root volatile _USARTx_CTRL1 _USART4_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _USART4_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _USART4_CTRL3;
__no_init __root volatile _USARTx_STS _USART4_STS;
__no_init __root volatile _USARTx_DAT _USART4_DAT;
__no_init __root volatile _USARTx_BRCF _USART4_BRCF;
__no_init __root volatile _USARTx_GTP _USART4_GTP;
__no_init __root volatile _USARTx_FIFO _USART4_FIFO;
__no_init __root volatile _USARTx_IFW _USART4_IFW;
__no_init __root volatile _USARTx_RTO _USART4_RTO;

__no_init __root volatile _USARTx_CTRL1 _UART5_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _UART5_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _UART5_CTRL3;
__no_init __root volatile _USARTx_STS _UART5_STS;
__no_init __root volatile _USARTx_DAT _UART5_DAT;
__no_init __root volatile _USARTx_BRCF _UART5_BRCF;
__no_init __root volatile _USARTx_GTP _UART5_GTP;
__no_init __root volatile _USARTx_FIFO _UART5_FIFO;
__no_init __root volatile _USARTx_IFW _UART5_IFW;
__no_init __root volatile _USARTx_RTO _UART5_RTO;

__no_init __root volatile _USARTx_CTRL1 _UART6_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _UART6_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _UART6_CTRL3;
__no_init __root volatile _USARTx_STS _UART6_STS;
__no_init __root volatile _USARTx_DAT _UART6_DAT;
__no_init __root volatile _USARTx_BRCF _UART6_BRCF;
__no_init __root volatile _USARTx_GTP _UART6_GTP;
__no_init __root volatile _USARTx_FIFO _UART6_FIFO;
__no_init __root volatile _USARTx_IFW _UART6_IFW;
__no_init __root volatile _USARTx_RTO _UART6_RTO;

__no_init __root volatile _USARTx_CTRL1 _UART7_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _UART7_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _UART7_CTRL3;
__no_init __root volatile _USARTx_STS _UART7_STS;
__no_init __root volatile _USARTx_DAT _UART7_DAT;
__no_init __root volatile _USARTx_BRCF _UART7_BRCF;
__no_init __root volatile _USARTx_GTP _UART7_GTP;
__no_init __root volatile _USARTx_FIFO _UART7_FIFO;
__no_init __root volatile _USARTx_IFW _UART7_IFW;
__no_init __root volatile _USARTx_RTO _UART7_RTO;

__no_init __root volatile _USARTx_CTRL1 _UART8_CTRL1;
__no_init __root volatile _USARTx_CTRL2 _UART8_CTRL2;
__no_init __root volatile _USARTx_CTRL3 _UART8_CTRL3;
__no_init __root volatile _USARTx_STS _UART8_STS;
__no_init __root volatile _USARTx_DAT _UART8_DAT;
__no_init __root volatile _USARTx_BRCF _UART8_BRCF;
__no_init __root volatile _USARTx_GTP _UART8_GTP;
__no_init __root volatile _USARTx_FIFO _UART8_FIFO;
__no_init __root volatile _USARTx_IFW _UART8_IFW;
__no_init __root volatile _USARTx_RTO _UART8_RTO;

//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------

//---------------- END LINE --------------------------------------------------------------------------------------------
