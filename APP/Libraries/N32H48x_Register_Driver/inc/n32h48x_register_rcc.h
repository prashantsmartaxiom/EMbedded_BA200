/***********************************************************************************************************************
File Name: n32h48x_register_rcc.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU RCC register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/11/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_RCC_H__
#define __N32H48x_REG_RCC_H__

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "n32h47x_48x.h"

//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------
/***************************************************************************************************
 *      RCCx_CTRL - Clock control register
***************************************************************************************************/
struct _RCCx_CTRL_bits
{
    unsigned long PLLEN         :1;
    unsigned long PLLRDF        :1;
    unsigned long HSIEN         :1;
    unsigned long HSIRDF        :1;
    unsigned long HSEEN         :1;
    unsigned long HSERDF        :1;
    unsigned long HSEBP         :1;
    unsigned long HSECSSEN      :1;
    unsigned long HSICAL        :12;
    unsigned long HSITRIM       :7;
    unsigned long Reserved1     :5;
};
typedef union
{
    struct _RCCx_CTRL_bits bit;
    unsigned long all;
} _RCCx_CTRL;
/***************************************************************************************************
 *      RCCx_CFG - Clock configuration register
***************************************************************************************************/
struct _RCCx_CFG_bits
{
    unsigned long PLLSRC        :1;
    unsigned long PLLHSEPRES    :1;
    unsigned long PLLHSIPRES    :1;
    unsigned long PLLMULFCT     :6;
    unsigned long APB1PRES      :3;
    unsigned long APB2PRES      :3;
    unsigned long Reserved1     :1;
    unsigned long AHBPRES       :4;
    unsigned long MCOPRES       :4;
    unsigned long SCLKSTS       :2;
    unsigned long SCLKSW        :2;
    unsigned long Reserved2     :2;
    unsigned long MCOPLLSEL     :2;
};
typedef union
{
    struct _RCCx_CFG_bits bit;
    unsigned long all;
} _RCCx_CFG;
/***************************************************************************************************
 *      RCCx_CFG2 - Clock configuration 2 register
***************************************************************************************************/
struct _RCCx_CFG2_bits
{
    unsigned long ADCHPRES      :4;
    unsigned long ADCOTGPLLPRES :3;
    unsigned long ADCOTGPLLSEL  :1;
    unsigned long ADCOTGPLLEN   :1;
    unsigned long Reserved1     :3;
    unsigned long ADC1MPRES     :5;
    unsigned long Reserved2     :2;
    unsigned long ADC1MSEL      :1;
    unsigned long RNGCPRES      :5;
    unsigned long Reserved3     :1;
    unsigned long HCLKPRES      :1;
    unsigned long GTIMCLKSEL    :1;
    unsigned long ATIMCLKSEL    :1;
    unsigned long Reserved4     :3;
};
typedef union
{
    struct _RCCx_CFG2_bits bit;
    unsigned long all;
} _RCCx_CFG2;
/***************************************************************************************************
 *      RCCx_CFG3 - Clock configuration 3 register
***************************************************************************************************/
struct _RCCx_CFG3_bits
{
    unsigned long BORRSTEN      :1;
    unsigned long TRNG1MSEL     :1;
    unsigned long TRNG1MEN      :1;
    unsigned long Reserved1     :2;
    unsigned long TRNG1MPRES    :4;
    unsigned long USBFSTM       :1;
    unsigned long USBFSSEL      :2;
    unsigned long RPINTF        :1;
    unsigned long RPINTEN       :1;
    unsigned long RPDMAEN       :1;
    unsigned long USBPLLSEL     :1;
    unsigned long USBPLLPRES    :4;
    unsigned long TRACEPRES     :2;
    unsigned long FDCANSEL      :2;
    unsigned long I2S2SEL       :2;
    unsigned long I2S3SEL       :2;
    unsigned long FDCANPLLPRES  :4;
};
typedef union
{
    struct _RCCx_CFG3_bits bit;
    unsigned long all;
} _RCCx_CFG3;
/***************************************************************************************************
 *      RCCx_CLKINT - Clock interrupt register
***************************************************************************************************/
struct _RCCx_CLKINT_bits
{
    unsigned long PLLRDIF       :1;
    unsigned long HSIRDIF       :1;
    unsigned long HSERDIF       :1;
    unsigned long HSECSSIF      :1;
    unsigned long LSIRDIF       :1;
    unsigned long LSERDIF       :1;
    unsigned long BORIF         :1;
    unsigned long LSECSSIF      :1;
    unsigned long PLLRDIEN      :1;
    unsigned long HSIRDIEN      :1;
    unsigned long HSERDIEN      :1;
    unsigned long HSECSSIN      :1;
    unsigned long LSIRDIEN      :1;
    unsigned long LSERDIEN      :1;
    unsigned long BORIEN        :1;
    unsigned long LSECSSIEN     :1;
    unsigned long PLLRDICLR     :1;
    unsigned long HSIRDICLR     :1;
    unsigned long HSERDICLR     :1;
    unsigned long HSECSSICLR    :1;
    unsigned long LSIRDICLR     :1;
    unsigned long LSERDICLR     :1;
    unsigned long BORICLR       :1;
    unsigned long LSECSSICLR    :1;
    unsigned long HSICSSICLR    :1;
    unsigned long HSICSSIEN     :1;
    unsigned long LSECSSF       :1;
    unsigned long HSECSSF       :1;
    unsigned long HSICSSF       :1;
    unsigned long HSICSSIF      :1;
    unsigned long Reserved1     :2;
};
typedef union
{
    struct _RCCx_CLKINT_bits bit;
    unsigned long all;
} _RCCx_CLKINT;
/***************************************************************************************************
 *      RCCx_CTRLSTS - Control status register
***************************************************************************************************/
struct _RCCx_CTRLSTS_bits
{
    unsigned long LSIEN         :1;
    unsigned long LSIRD         :1;
    unsigned long Reserved1     :1;
    unsigned long RMRSTF        :1;
    unsigned long Reserved2     :15;
    unsigned long BORRSTF       :1;
    unsigned long MAINEMCF      :1;
    unsigned long BKPEMCF       :1;
    unsigned long Reserved3     :1;
    unsigned long RAMERSTF      :1;
    unsigned long Reserved4     :1;
    unsigned long SMPURSTF      :1;
    unsigned long PINRSTF       :1;
    unsigned long PORRSTF       :1;
    unsigned long SFTRSTF       :1;
    unsigned long IWDGRSTF      :1;
    unsigned long WWDGRSTF      :1;
    unsigned long LPWRRSTF      :1;
};
typedef union
{
    struct _RCCx_CTRLSTS_bits bit;
    unsigned long all;
} _RCCx_CTRLSTS;
/***************************************************************************************************
 *      RCCx_AHBPCLKEN - AHB peripheral clock enable register
***************************************************************************************************/
struct _RCCx_AHBPCLKEN_bits
{
    unsigned long SRAMEN        :1;
    unsigned long FLITFEN       :1;
    unsigned long FEMCEN        :1;
    unsigned long BKPSRAMEN     :1;
    unsigned long RNGCEN        :1;
    unsigned long XSPIEN        :1;
    unsigned long SDIOEN        :1;
    unsigned long DMA1EN        :1;
    unsigned long DMA2EN        :1;
    unsigned long CRCEN         :1;
    unsigned long ETHEN         :1;
    unsigned long USBHSEN       :1;
    unsigned long FMACEN        :1;
    unsigned long CORDICEN      :1;
    unsigned long DVPEN         :1;
    unsigned long ATIM1EN       :1;
    unsigned long ATIM2EN       :1;
    unsigned long ATIM3EN       :1;
    unsigned long SHRTIMEN      :1;
    unsigned long Reserved1     :13;
};
typedef union
{
    struct _RCCx_AHBPCLKEN_bits bit;
    unsigned long all;
} _RCCx_AHBPCLKEN;
/***************************************************************************************************
 *      RCCx_APB1PCLKEN - APB1 peripheral clock enable register
***************************************************************************************************/
struct _RCCx_APB1PCLKEN_bits
{
    unsigned long PWREN         :1;
    unsigned long BKPEN         :1;
    unsigned long USART2EN      :1;
    unsigned long USART3EN      :1;
    unsigned long UART8EN       :1;
    unsigned long UART5EN       :1;
    unsigned long I2C1EN        :1;
    unsigned long I2C2EN        :1;
    unsigned long SPI2EN        :1;
    unsigned long SPI3EN        :1;
    unsigned long GTIM1EN       :1;
    unsigned long GTIM2EN       :1;
    unsigned long GTIM3EN       :1;
    unsigned long GTIM4EN       :1;
    unsigned long GTIM5EN       :1;
    unsigned long GTIM6EN       :1;
    unsigned long GTIM7EN       :1;
    unsigned long UCDREN        :1;
    unsigned long USBFSEN       :1;
    unsigned long FDCAN1EN      :1;
    unsigned long FDCAN2EN      :1;
    unsigned long FDCAN3EN      :1;
    unsigned long BTIM1EN       :1;
    unsigned long BTIM2EN       :1;
    unsigned long FDCAN1OFFEN   :1;
    unsigned long FDCAN2OFFEN   :1;
    unsigned long FDCAN3OFFEN   :1;
    unsigned long I2C3EN        :1;
    unsigned long I2C4EN        :1;
    unsigned long WWDGEN        :1;
    unsigned long DAC12EN       :1;
    unsigned long DAC34EN       :1;
};
typedef union
{
    struct _RCCx_APB1PCLKEN_bits bit;
    unsigned long all;
} _RCCx_APB1PCLKEN;
/***************************************************************************************************
 *      RCCx_APB2PCLKEN - APB2 peripheral clock enable register
***************************************************************************************************/
struct _RCCx_APB2PCLKEN_bits
{
    unsigned long AFIOEN        :1;
    unsigned long SPI6EN        :1;
    unsigned long SPI5EN        :1;
    unsigned long SPI4EN        :1;
    unsigned long SPI1EN        :1;
    unsigned long USART1EN      :1;
    unsigned long UART6EN       :1;
    unsigned long UART7EN       :1;
    unsigned long GTIM8EN       :1;
    unsigned long GTIM9EN       :1;
    unsigned long GTIM10EN      :1;
    unsigned long COMPEN        :1;
    unsigned long PGAEN         :1;
    unsigned long COMPFILTEN    :1;
    unsigned long USART4EN      :1;
    unsigned long Reserved1     :17;
};
typedef union
{
    struct _RCCx_APB2PCLKEN_bits bit;
    unsigned long all;
} _RCCx_APB2PCLKEN;
/***************************************************************************************************
 *      RCCx_AHBPRST - AHB peripheral reset register
***************************************************************************************************/
struct _RCCx_AHBPRST_bits
{
    unsigned long SHRTIMRST     :1;
    unsigned long SHRTPLLRST    :1;
    unsigned long SHRTPLLPHARST :1;
    unsigned long SACRST        :1;
    unsigned long RNGCRST       :1;
    unsigned long XSPIRST       :1;
    unsigned long USBHSPHYRST   :1;
    unsigned long SDIORST       :1;
    unsigned long FEMCRST       :1;
    unsigned long Reserved1     :1;
    unsigned long IOPARST       :1;
    unsigned long IOPBRST       :1;
    unsigned long IOPCRST       :1;
    unsigned long IOPDRST       :1;
    unsigned long IOPERST       :1;
    unsigned long IOPFRST       :1;
    unsigned long IOPGRST       :1;
    unsigned long IOPHRST       :1;
    unsigned long ADC1RST       :1;
    unsigned long ADC2RST       :1;
    unsigned long ADC3RST       :1;
    unsigned long ADC4RST       :1;
    unsigned long DAC56RST      :1;
    unsigned long DAC78RST      :1;
    unsigned long DVPRST        :1;
    unsigned long ATIM1RST      :1;
    unsigned long ATIM2RST      :1;
    unsigned long ATIM3RST      :1;
    unsigned long CORDICRST     :1;
    unsigned long FMACRST       :1;
    unsigned long USBHSRST      :1;
    unsigned long ETHRST        :1;
};
typedef union
{
    struct _RCCx_AHBPRST_bits bit;
    unsigned long all;
} _RCCx_AHBPRST;
/***************************************************************************************************
 *      RCCx_APB1PRST - APB1 peripheral reset register
***************************************************************************************************/
struct _RCCx_APB1PRST_bits
{
    unsigned long PWRRST        :1;
    unsigned long BKPRST        :1;
    unsigned long USART2RST     :1;
    unsigned long USART3RST     :1;
    unsigned long UART8RST      :1;
    unsigned long UART5RST      :1;
    unsigned long I2C1RST       :1;
    unsigned long I2C2RST       :1;
    unsigned long SPI2RST       :1;
    unsigned long SPI3RST       :1;
    unsigned long GTIM1RST      :1;
    unsigned long GTIM2RST      :1;
    unsigned long GTIM3RST      :1;
    unsigned long GTIM4RST      :1;
    unsigned long GTIM5RST      :1;
    unsigned long GTIM6RST      :1;
    unsigned long GTIM7RST      :1;
    unsigned long UCDRRST       :1;
    unsigned long USBFSRST      :1;
    unsigned long FDCAN1RST     :1;
    unsigned long FDCAN2RST     :1;
    unsigned long FDCAN3RST     :1;
    unsigned long BTIM1RST      :1;
    unsigned long BTIM2RST      :1;
    unsigned long Reserved1     :3;
    unsigned long I2C3RST       :1;
    unsigned long I2C4RST       :1;
    unsigned long WWDGRST       :1;
    unsigned long DAC12RST      :1;
    unsigned long DAC34RST      :1;
};
typedef union
{
    struct _RCCx_APB1PRST_bits bit;
    unsigned long all;
} _RCCx_APB1PRST;
/***************************************************************************************************
 *      RCCx_APB2PRST - APB2 peripheral reset register
***************************************************************************************************/
struct _RCCx_APB2PRST_bits
{
    unsigned long AFIORST       :1;
    unsigned long SPI6RST       :1;
    unsigned long SPI5RST       :1;
    unsigned long SPI4RST       :1;
    unsigned long SPI1RST       :1;
    unsigned long USART1RST     :1;
    unsigned long UART6RST      :1;
    unsigned long UART7RST      :1;
    unsigned long GTIM8RST      :1;
    unsigned long GTIM9RST      :1;
    unsigned long GTIM10RST     :1;
    unsigned long COMPRST       :1;
    unsigned long PGARST        :1;
    unsigned long Reserved1     :1;
    unsigned long USART4RST     :1;
    unsigned long Reserved2     :17;
};
typedef union
{
    struct _RCCx_APB2PRST_bits bit;
    unsigned long all;
} _RCCx_APB2PRST;
/***************************************************************************************************
 *      RCCx_MCOCFG - MCU clock output configuration register
***************************************************************************************************/
struct _RCCx_MCOCFG_bits
{
    unsigned long MCO1          :4;
    unsigned long MCO2          :4;
    unsigned long Reserved1     :24;
};
typedef union
{
    struct _RCCx_MCOCFG_bits bit;
    unsigned long all;
} _RCCx_MCOCFG;
/***************************************************************************************************
 *      RCCx_BDCTRL - Backup domain control register
***************************************************************************************************/
struct _RCCx_BDCTRL_bits
{
    unsigned long LSEEN         :1;
    unsigned long LSERD         :1;
    unsigned long LSEBP         :1;
    unsigned long LSECSSEN      :1;
    unsigned long Reserved1     :4;
    unsigned long RTCSEL        :2;
    unsigned long Reserved2     :5;
    unsigned long RTCEN         :1;
    unsigned long BDSFTRST      :1;
    unsigned long Reserved3     :3;
    unsigned long LPTIM1SEL     :3;
    unsigned long LPTIM1EN      :1;
    unsigned long LPTIM2SEL     :3;
    unsigned long LPTIM2EN      :1;
    unsigned long LPTIM1RST     :1;
    unsigned long LPTIM2RST     :1;
    unsigned long Reserved4     :2;
};
typedef union
{
    struct _RCCx_BDCTRL_bits bit;
    unsigned long all;
} _RCCx_BDCTRL;
/***************************************************************************************************
 *      RCCx_CSSCTRL - Clock security system control register
***************************************************************************************************/
struct _RCCx_CSSCTRL_bits
{
    unsigned long HSEFLTHR      :4;
    unsigned long HSEFHTHR      :4;
    unsigned long HSEPOTHR      :4;
    unsigned long HSEDLSEL      :1;
    unsigned long HSEFLEN       :1;
    unsigned long HSEFHEN       :1;
    unsigned long HSEPOEN       :1;
    unsigned long HSEFLF        :1;
    unsigned long HSEFHF        :1;
    unsigned long HSEPOF        :1;
    unsigned long Reserved1     :1;
    unsigned long WKUPCLKSEL    :1;
    unsigned long Reserved2     :1;
    unsigned long HSICSSEN      :1;
    unsigned long Reserved3     :9;
};
typedef union
{
    struct _RCCx_CSSCTRL_bits bit;
    unsigned long all;
} _RCCx_CSSCTRL;
/***************************************************************************************************
 *      RCCx_AHB1PCLKEN - AHB1 peripheral clock enable register
***************************************************************************************************/
struct _RCCx_AHB1PCLKEN_bits
{
    unsigned long IOPAEN        :1;
    unsigned long IOPBEN        :1;
    unsigned long IOPCEN        :1;
    unsigned long IOPDEN        :1;
    unsigned long IOPEEN        :1;
    unsigned long IOPFEN        :1;
    unsigned long IOPGEN        :1;
    unsigned long IOPHEN        :1;
    unsigned long IOFILTSEL     :1;
    unsigned long ADC1EN        :1;
    unsigned long ADC2EN        :1;
    unsigned long ADC3EN        :1;
    unsigned long ADC4EN        :1;
    unsigned long DAC56EN       :1;
    unsigned long DAC78EN       :1;
    unsigned long SACEN         :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _RCCx_AHB1PCLKEN_bits bit;
    unsigned long all;
} _RCCx_AHB1PCLKEN;
/***************************************************************************************************
 *      RCCx_PLLCTRL - PLL clock control register
***************************************************************************************************/
struct _RCCx_PLLCTRL_bits
{
    unsigned long Reserved1     :4;
    unsigned long LPFC1         :4;
    unsigned long LPFR          :4;
    unsigned long LPFC2         :3;
    unsigned long DEADZEN       :1;
    unsigned long PLLINPRES     :2;
    unsigned long PLLOUTPRES    :2;
    unsigned long SCLKPLLSEL    :2;
    unsigned long Reserved2     :10;
};
typedef union
{
    struct _RCCx_PLLCTRL_bits bit;
    unsigned long all;
} _RCCx_PLLCTRL;
/***************************************************************************************************
 *      RCCx_VREFCTRL - VREFBUF control register
***************************************************************************************************/
struct _RCCx_VREFCTRL_bits
{
    unsigned long TRIMDATA      :6;
    unsigned long VLSEL         :2;
    unsigned long EN            :1;
    unsigned long HIM           :1;
    unsigned long Reserved1     :2;
    unsigned long RDY           :1;
    unsigned long Reserved2     :1;
    unsigned long TRIMEN        :1;
    unsigned long Reserved3     :17;
};
typedef union
{
    struct _RCCx_VREFCTRL_bits bit;
    unsigned long all;
} _RCCx_VREFCTRL;
/***************************************************************************************************
 *      RCCx_SHRTPLLCTRL1 - SHRTPLL clock control 1 register
***************************************************************************************************/
struct _RCCx_SHRTPLLCTRL1_bits
{
    unsigned long CLKF          :26;
    unsigned long Reserved1     :1;
    unsigned long FRACEN        :1;
    unsigned long SHRTPLLMODE   :1;
    unsigned long Reserved2     :1;
    unsigned long SATEN         :1;
    unsigned long Reserved3     :1;
};
typedef union
{
    struct _RCCx_SHRTPLLCTRL1_bits bit;
    unsigned long all;
} _RCCx_SHRTPLLCTRL1;
/***************************************************************************************************
 *      RCCx_SHRTPLLCTRL2 - SHRTPLL clock control 2 register
***************************************************************************************************/
struct _RCCx_SHRTPLLCTRL2_bits
{
    unsigned long Reserved1     :26;
    unsigned long CLKR          :6;
};
typedef union
{
    struct _RCCx_SHRTPLLCTRL2_bits bit;
    unsigned long all;
} _RCCx_SHRTPLLCTRL2;
/***************************************************************************************************
 *      RCCx_SHRTPLLCTRL3 - SHRTPLL clock control 3 register
***************************************************************************************************/
struct _RCCx_SHRTPLLCTRL3_bits
{
    unsigned long Reserved1     :12;
    unsigned long BWTRIM        :12;
    unsigned long Reserved2     :1;
    unsigned long SHRTPLLEN     :1;
    unsigned long SHRTPLLSRC    :1;
    unsigned long Reserved3     :1;
    unsigned long SHRTPLLRDF    :1;
    unsigned long Reserved4     :3;
};
typedef union
{
    struct _RCCx_SHRTPLLCTRL3_bits bit;
    unsigned long all;
} _RCCx_SHRTPLLCTRL3;
/***************************************************************************************************
 *      RCCx_SRAMCFG1 - SRAM configuration 1 register
***************************************************************************************************/
struct _RCCx_SRAMCFG1_bits
{
    unsigned long SRAMPEN       :1;
    unsigned long Reserved1     :1;
    unsigned long CSRINIEN      :1;
    unsigned long CSRINIF       :1;
    unsigned long SRAMREF       :1;
    unsigned long SRAMWEF       :1;
    unsigned long SRAMFIN       :1;
    unsigned long Reserved2     :1;
    unsigned long SR1PEIEN      :1;
    unsigned long Reserved3     :1;
    unsigned long SR1PERSTEN    :1;
    unsigned long Reserved4     :1;
    unsigned long SR2PEIEN      :1;
    unsigned long Reserved5     :1;
    unsigned long SR2PERSTEN    :1;
    unsigned long Reserved6     :1;
    unsigned long CSRECCIEN     :2;
    unsigned long CSRECCRSTEN   :2;
    unsigned long SR1PECLR      :1;
    unsigned long SR2PECLR      :1;
    unsigned long CSRECCCLR     :1;
    unsigned long FECCRSTEN     :1;
    unsigned long SR1PEF        :1;
    unsigned long Reserved7     :1;
    unsigned long SR2PEF        :1;
    unsigned long Reserved8     :1;
    unsigned long CSRECCF       :2;
    unsigned long Reserved9     :2;
};
typedef union
{
    struct _RCCx_SRAMCFG1_bits bit;
    unsigned long all;
} _RCCx_SRAMCFG1;
/***************************************************************************************************
 *      RCCx_SRAMCFG2 - SRAM configuration 2 register
***************************************************************************************************/
struct _RCCx_SRAMCFG2_bits
{
    unsigned long INIDAT        :32;
};
typedef union
{
    struct _RCCx_SRAMCFG2_bits bit;
    unsigned long all;
} _RCCx_SRAMCFG2;
/***************************************************************************************************
 *      RCCx_SRAMCFG3 - SRAM configuration 3 register
***************************************************************************************************/
struct _RCCx_SRAMCFG3_bits
{
    unsigned long INISTART      :32;
};
typedef union
{
    struct _RCCx_SRAMCFG3_bits bit;
    unsigned long all;
} _RCCx_SRAMCFG3;
/***************************************************************************************************
 *      RCCx_SRAMCFG4 - SRAM configuration 4 register
***************************************************************************************************/
struct _RCCx_SRAMCFG4_bits
{
    unsigned long INIEND        :32;
};
typedef union
{
    struct _RCCx_SRAMCFG4_bits bit;
    unsigned long all;
} _RCCx_SRAMCFG4;
/***************************************************************************************************
 *      RCCx_USBHSCTRL1 - USBHS clock control 1 register
***************************************************************************************************/
struct _RCCx_USBHSCTRL1_bits
{
    unsigned long Reserved1     :4;
    unsigned long WKUPSEL       :1;
    unsigned long Reserved2     :3;
    unsigned long USBHSFSEL     :3;
    unsigned long PLLBW         :1;
    unsigned long Reserved3     :3;
    unsigned long PDSEL         :1;
    unsigned long Reserved4     :3;
    unsigned long RESSEL        :1;
    unsigned long Reserved5     :2;
    unsigned long SEBP          :1;
    unsigned long USBHSSEL      :1;
    unsigned long Reserved6     :4;
    unsigned long BSTLEN        :1;
    unsigned long BSTHEN        :1;
    unsigned long RXTXPATH      :1;
    unsigned long Reserved7     :1;
};
typedef union
{
    struct _RCCx_USBHSCTRL1_bits bit;
    unsigned long all;
} _RCCx_USBHSCTRL1;
/***************************************************************************************************
 *      RCCx_USBHSCTRL2 - USBHS clock control 2 register
***************************************************************************************************/
struct _RCCx_USBHSCTRL2_bits
{
    unsigned long FLIMPTRIM     :4;
    unsigned long HDCVTRIM      :4;
    unsigned long DATAVTRIM     :2;
    unsigned long CROSTRIM      :2;
    unsigned long RFTRIM        :2;
    unsigned long IMPTRIM       :2;
    unsigned long Reserved1     :3;
    unsigned long IDDQEN        :1;
    unsigned long RESTRIM       :4;
    unsigned long Reserved2     :8;
};
typedef union
{
    struct _RCCx_USBHSCTRL2_bits bit;
    unsigned long all;
} _RCCx_USBHSCTRL2;
/***************************************************************************************************
 *      RCCx_BOOTREMAP - BOOT remap register
***************************************************************************************************/
struct _RCCx_BOOTREMAP_bits
{
    unsigned long REMAPSEL      :2;
    unsigned long Reserved1     :6;
    unsigned long REMAPKEY      :8;
    unsigned long Reserved2     :16;
};
typedef union
{
    struct _RCCx_BOOTREMAP_bits bit;
    unsigned long all;
} _RCCx_BOOTREMAP;
/***************************************************************************************************
 *      RCCx_FDCANBADDR - FDCAN base address register
***************************************************************************************************/
struct _RCCx_FDCANBADDR_bits
{
    unsigned long BADDR         :32;
};
typedef union
{
    struct _RCCx_FDCANBADDR_bits bit;
    unsigned long all;
} _RCCx_FDCANBADDR;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _RCCx_CTRL _RCC_CTRL @(RCC_BASE + 0x0000);
extern volatile _RCCx_CFG _RCC_CFG @(RCC_BASE + 0x0004);
extern volatile _RCCx_CFG2 _RCC_CFG2 @(RCC_BASE + 0x0008);
extern volatile _RCCx_CFG3 _RCC_CFG3 @(RCC_BASE + 0x000C);
extern volatile _RCCx_CLKINT _RCC_CLKINT @(RCC_BASE + 0x0010);
extern volatile _RCCx_CTRLSTS _RCC_CTRLSTS @(RCC_BASE + 0x0014);
extern volatile _RCCx_AHBPCLKEN _RCC_AHBPCLKEN @(RCC_BASE + 0x0018);
extern volatile _RCCx_APB1PCLKEN _RCC_APB1PCLKEN @(RCC_BASE + 0x001C);
extern volatile _RCCx_APB2PCLKEN _RCC_APB2PCLKEN @(RCC_BASE + 0x0020);
extern volatile _RCCx_AHBPRST _RCC_AHBPRST @(RCC_BASE + 0x0024);
extern volatile _RCCx_APB1PRST _RCC_APB1PRST @(RCC_BASE + 0x0028);
extern volatile _RCCx_APB2PRST _RCC_APB2PRST @(RCC_BASE + 0x002C);
extern volatile _RCCx_MCOCFG _RCC_MCOCFG @(RCC_BASE + 0x0030);
extern volatile _RCCx_BDCTRL _RCC_BDCTRL @(RCC_BASE + 0x0034);
extern volatile _RCCx_CSSCTRL _RCC_CSSCTRL @(RCC_BASE + 0x0038);
extern volatile _RCCx_AHB1PCLKEN _RCC_AHB1PCLKEN @(RCC_BASE + 0x003C);
extern volatile _RCCx_PLLCTRL _RCC_PLLCTRL @(RCC_BASE + 0x0040);
extern volatile _RCCx_VREFCTRL _RCC_VREFCTRL @(RCC_BASE + 0x0044);
extern volatile _RCCx_SHRTPLLCTRL1 _RCC_SHRTPLLCTRL1 @(RCC_BASE + 0x0048);
extern volatile _RCCx_SHRTPLLCTRL2 _RCC_SHRTPLLCTRL2 @(RCC_BASE + 0x004C);
extern volatile _RCCx_SHRTPLLCTRL3 _RCC_SHRTPLLCTRL3 @(RCC_BASE + 0x0050);
extern volatile _RCCx_SRAMCFG1 _RCC_SRAMCFG1 @(RCC_BASE + 0x0070);
extern volatile _RCCx_SRAMCFG2 _RCC_SRAMCFG2 @(RCC_BASE + 0x0074);
extern volatile _RCCx_SRAMCFG3 _RCC_SRAMCFG3 @(RCC_BASE + 0x0078);
extern volatile _RCCx_SRAMCFG4 _RCC_SRAMCFG4 @(RCC_BASE + 0x007C);
extern volatile _RCCx_USBHSCTRL1 _RCC_USBHSCTRL1 @(RCC_BASE + 0x0084);
extern volatile _RCCx_USBHSCTRL2 _RCC_USBHSCTRL2 @(RCC_BASE + 0x0088);
extern volatile _RCCx_BOOTREMAP _RCC_BOOTREMAP @(RCC_BASE + 0x008C);
extern volatile _RCCx_FDCANBADDR _RCC_FDCANBADDR @(RCC_BASE + 0x0090);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_RCC_H__ */
