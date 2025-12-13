/***********************************************************************************************************************
File Name: n32h48x_register_usart.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU USART register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/25/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_USART_H__
#define __N32H48x_REG_USART_H__

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
 *      USARTx_CTRL1 - USART Control Register 1 Register
***************************************************************************************************/
struct _USARTx_CTRL1_bits
{
    unsigned long UEN               :1;
    unsigned long RXEN              :1;
    unsigned long TXEN              :1;
    unsigned long PSEL              :1;
    unsigned long PCEN              :1;
    unsigned long WL                :1;
    unsigned long RCVWU             :1;
    unsigned long WUM               :1;
    unsigned long IDLEIEN           :1;
    unsigned long RXDNEIEN          :1;
    unsigned long TXDEIEN           :1;
    unsigned long TXCIEN            :1;
    unsigned long PEIEN             :1;
    unsigned long SDBRK             :1;
    unsigned long DEM               :1;
    unsigned long DEP               :1;
    unsigned long DEDT              :5;
    unsigned long DEAT              :5;
    unsigned long OSPM              :1;
    unsigned long SWAP              :1;
    unsigned long IFCEN             :1;
    unsigned long Reserved1         :3;
};
typedef union
{
    struct _USARTx_CTRL1_bits bit;
    unsigned long all;
} _USARTx_CTRL1;
/***************************************************************************************************
 *      USARTx_CTRL2 - USART Control Register 2 Register
***************************************************************************************************/
struct _USARTx_CTRL2_bits
{
    unsigned long ADDR              :4;
    unsigned long Reserved1         :1;
    unsigned long STPB              :2;
    unsigned long Reserved2         :1;
    unsigned long CLKEN             :1;
    unsigned long CLKPOL            :1;
    unsigned long CLKPHA            :1;
    unsigned long LBCLK             :1;
    unsigned long LINMEN            :1;
    unsigned long LINBDIEN          :1;
    unsigned long LINBDL            :1;
    unsigned long RTOEN             :1;
    unsigned long RTOCF             :1;
    unsigned long RTOITE            :1;
    unsigned long PEFLOSE           :1;
    unsigned long NEFLOSE           :1;
    unsigned long FEFLOSE           :1;
    unsigned long Reserved3         :11;
};
typedef union
{
    struct _USARTx_CTRL2_bits bit;
    unsigned long all;
} _USARTx_CTRL2;
/***************************************************************************************************
 *      USARTx_CTRL3 - USART Control Register 3 Register
***************************************************************************************************/
struct _USARTx_CTRL3_bits
{
    unsigned long CTSEN             :1;
    unsigned long CTSIEN            :1;
    unsigned long RTSEN             :1;
    unsigned long HDMEN             :1;
    unsigned long DMATXEN           :1;
    unsigned long DMARXEN           :1;
    unsigned long ERRIEN            :1;
    unsigned long IRDAMEN           :1;
    unsigned long IRDALP            :1;
    unsigned long SCMEN             :1;
    unsigned long SCNACK            :1;
    unsigned long Reserved1         :21;
};
typedef union
{
    struct _USARTx_CTRL3_bits bit;
    unsigned long all;
} _USARTx_CTRL3;
/***************************************************************************************************
 *      USARTx_STS - USART Status Register
***************************************************************************************************/
struct _USARTx_STS_bits
{
    unsigned long TXFF              :1;
    unsigned long RXFF              :1;
    unsigned long TXFE              :1;
    unsigned long RXFE              :1;
    unsigned long RXFT              :1;
    unsigned long TXFT              :1;
    unsigned long IDLEF             :1;
    unsigned long TXDE              :1;
    unsigned long TXC               :1;
    unsigned long RXDNE             :1;
    unsigned long CTSF              :1;
    unsigned long LINBDF            :1;
    unsigned long PEF               :1;
    unsigned long OREF              :1;
    unsigned long NEF               :1;
    unsigned long FEF               :1;
    unsigned long RTOF              :1;
    unsigned long PELOSEF           :1;
    unsigned long NELOSEF           :1;
    unsigned long FELOSEF           :1;
    unsigned long Reserved1         :12;
};
typedef union
{
    struct _USARTx_STS_bits bit;
    unsigned long all;
} _USARTx_STS;
/***************************************************************************************************
 *      USARTx_DAT - USART Data Register
***************************************************************************************************/
struct _USARTx_DAT_bits
{
    unsigned long DATV              :9;
    unsigned long Reserved1         :23;
};
typedef union
{
    struct _USARTx_DAT_bits bit;
    unsigned long all;
} _USARTx_DAT;
/***************************************************************************************************
 *      USARTx_BRCF - USART Baud Rate Register
***************************************************************************************************/
struct _USARTx_BRCF_bits
{
    unsigned long DIV_Decimal       :4;
    unsigned long DIV_Integer       :12;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _USARTx_BRCF_bits bit;
    unsigned long all;
} _USARTx_BRCF;
/***************************************************************************************************
 *      USARTx_GTP - USART Guard Time And Prescaler Register
***************************************************************************************************/
struct _USARTx_GTP_bits
{
    unsigned long PSCV              :8;
    unsigned long GTV               :8;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _USARTx_GTP_bits bit;
    unsigned long all;
} _USARTx_GTP;
/***************************************************************************************************
 *      USARTx_FIFO - USART FIFO Register
***************************************************************************************************/
struct _USARTx_FIFO_bits
{
    unsigned long EN                :1;
    unsigned long CLR               :1;
    unsigned long TXFTCFG           :3;
    unsigned long RXFTCFG           :3;
    unsigned long TXFFITE           :1;
    unsigned long RXFFITE           :1;
    unsigned long TXFEITE           :1;
    unsigned long RXFEITE           :1;
    unsigned long RXFTITE           :1;
    unsigned long TXFTITE           :1;
    unsigned long RXCNT             :4;
    unsigned long TXCNT             :4;
    unsigned long Reserved1         :10;
};
typedef union
{
    struct _USARTx_FIFO_bits bit;
    unsigned long all;
} _USARTx_FIFO;
/***************************************************************************************************
 *      USARTx_IFW - USART Idle Frame Width Register
***************************************************************************************************/
struct _USARTx_IFW_bits
{
    unsigned long WIDTH             :16;
    unsigned long Reserved1         :10;
};
typedef union
{
    struct _USARTx_IFW_bits bit;
    unsigned long all;
} _USARTx_IFW;
/***************************************************************************************************
 *      USARTx_RTO - USART Receive Timeout Width Register
***************************************************************************************************/
struct _USARTx_RTO_bits
{
    unsigned long TIME              :28;
    unsigned long Reserved1         :4;
};
typedef union
{
    struct _USARTx_RTO_bits bit;
    unsigned long all;
} _USARTx_RTO;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _USARTx_CTRL1 _USART1_CTRL1 @(USART1_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _USART1_CTRL2 @(USART1_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _USART1_CTRL3 @(USART1_BASE + 0x0008);
extern volatile _USARTx_STS _USART1_STS @(USART1_BASE + 0x000C);
extern volatile _USARTx_DAT _USART1_DAT @(USART1_BASE + 0x0010);
extern volatile _USARTx_BRCF _USART1_BRCF @(USART1_BASE + 0x0014);
extern volatile _USARTx_GTP _USART1_GTP @(USART1_BASE + 0x0018);
extern volatile _USARTx_FIFO _USART1_FIFO @(USART1_BASE + 0x001C);
extern volatile _USARTx_IFW _USART1_IFW @(USART1_BASE + 0x0020);
extern volatile _USARTx_RTO _USART1_RTO @(USART1_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _USART2_CTRL1 @(USART2_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _USART2_CTRL2 @(USART2_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _USART2_CTRL3 @(USART2_BASE + 0x0008);
extern volatile _USARTx_STS _USART2_STS @(USART2_BASE + 0x000C);
extern volatile _USARTx_DAT _USART2_DAT @(USART2_BASE + 0x0010);
extern volatile _USARTx_BRCF _USART2_BRCF @(USART2_BASE + 0x0014);
extern volatile _USARTx_GTP _USART2_GTP @(USART2_BASE + 0x0018);
extern volatile _USARTx_FIFO _USART2_FIFO @(USART2_BASE + 0x001C);
extern volatile _USARTx_IFW _USART2_IFW @(USART2_BASE + 0x0020);
extern volatile _USARTx_RTO _USART2_RTO @(USART2_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _USART3_CTRL1 @(USART3_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _USART3_CTRL2 @(USART3_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _USART3_CTRL3 @(USART3_BASE + 0x0008);
extern volatile _USARTx_STS _USART3_STS @(USART3_BASE + 0x000C);
extern volatile _USARTx_DAT _USART3_DAT @(USART3_BASE + 0x0010);
extern volatile _USARTx_BRCF _USART3_BRCF @(USART3_BASE + 0x0014);
extern volatile _USARTx_GTP _USART3_GTP @(USART3_BASE + 0x0018);
extern volatile _USARTx_FIFO _USART3_FIFO @(USART3_BASE + 0x001C);
extern volatile _USARTx_IFW _USART3_IFW @(USART3_BASE + 0x0020);
extern volatile _USARTx_RTO _USART3_RTO @(USART3_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _USART4_CTRL1 @(USART4_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _USART4_CTRL2 @(USART4_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _USART4_CTRL3 @(USART4_BASE + 0x0008);
extern volatile _USARTx_STS _USART4_STS @(USART4_BASE + 0x000C);
extern volatile _USARTx_DAT _USART4_DAT @(USART4_BASE + 0x0010);
extern volatile _USARTx_BRCF _USART4_BRCF @(USART4_BASE + 0x0014);
extern volatile _USARTx_GTP _USART4_GTP @(USART4_BASE + 0x0018);
extern volatile _USARTx_FIFO _USART4_FIFO @(USART4_BASE + 0x001C);
extern volatile _USARTx_IFW _USART4_IFW @(USART4_BASE + 0x0020);
extern volatile _USARTx_RTO _USART4_RTO @(USART4_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _UART5_CTRL1 @(UART5_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _UART5_CTRL2 @(UART5_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _UART5_CTRL3 @(UART5_BASE + 0x0008);
extern volatile _USARTx_STS _UART5_STS @(UART5_BASE + 0x000C);
extern volatile _USARTx_DAT _UART5_DAT @(UART5_BASE + 0x0010);
extern volatile _USARTx_BRCF _UART5_BRCF @(UART5_BASE + 0x0014);
extern volatile _USARTx_GTP _UART5_GTP @(UART5_BASE + 0x0018);
extern volatile _USARTx_FIFO _UART5_FIFO @(UART5_BASE + 0x001C);
extern volatile _USARTx_IFW _UART5_IFW @(UART5_BASE + 0x0020);
extern volatile _USARTx_RTO _UART5_RTO @(UART5_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _UART6_CTRL1 @(UART6_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _UART6_CTRL2 @(UART6_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _UART6_CTRL3 @(UART6_BASE + 0x0008);
extern volatile _USARTx_STS _UART6_STS @(UART6_BASE + 0x000C);
extern volatile _USARTx_DAT _UART6_DAT @(UART6_BASE + 0x0010);
extern volatile _USARTx_BRCF _UART6_BRCF @(UART6_BASE + 0x0014);
extern volatile _USARTx_GTP _UART6_GTP @(UART6_BASE + 0x0018);
extern volatile _USARTx_FIFO _UART6_FIFO @(UART6_BASE + 0x001C);
extern volatile _USARTx_IFW _UART6_IFW @(UART6_BASE + 0x0020);
extern volatile _USARTx_RTO _UART6_RTO @(UART6_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _UART7_CTRL1 @(UART7_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _UART7_CTRL2 @(UART7_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _UART7_CTRL3 @(UART7_BASE + 0x0008);
extern volatile _USARTx_STS _UART7_STS @(UART7_BASE + 0x000C);
extern volatile _USARTx_DAT _UART7_DAT @(UART7_BASE + 0x0010);
extern volatile _USARTx_BRCF _UART7_BRCF @(UART7_BASE + 0x0014);
extern volatile _USARTx_GTP _UART7_GTP @(UART7_BASE + 0x0018);
extern volatile _USARTx_FIFO _UART7_FIFO @(UART7_BASE + 0x001C);
extern volatile _USARTx_IFW _UART7_IFW @(UART7_BASE + 0x0020);
extern volatile _USARTx_RTO _UART7_RTO @(UART7_BASE + 0x0024);

extern volatile _USARTx_CTRL1 _UART8_CTRL1 @(UART8_BASE + 0x0000);
extern volatile _USARTx_CTRL2 _UART8_CTRL2 @(UART8_BASE + 0x0004);
extern volatile _USARTx_CTRL3 _UART8_CTRL3 @(UART8_BASE + 0x0008);
extern volatile _USARTx_STS _UART8_STS @(UART8_BASE + 0x000C);
extern volatile _USARTx_DAT _UART8_DAT @(UART8_BASE + 0x0010);
extern volatile _USARTx_BRCF _UART8_BRCF @(UART8_BASE + 0x0014);
extern volatile _USARTx_GTP _UART8_GTP @(UART8_BASE + 0x0018);
extern volatile _USARTx_FIFO _UART8_FIFO @(UART8_BASE + 0x001C);
extern volatile _USARTx_IFW _UART8_IFW @(UART8_BASE + 0x0020);
extern volatile _USARTx_RTO _UART8_RTO @(UART8_BASE + 0x0024);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_USART_H__ */
