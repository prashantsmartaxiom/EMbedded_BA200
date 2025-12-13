/***********************************************************************************************************************
File Name: n32h48x_register_i2c.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU I2C register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/25/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_I2C_H__
#define __N32H48x_REG_I2C_H__

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
 *      I2Cx_CTRL1 - I2C Control Register 1
***************************************************************************************************/
struct _I2Cx_CTRL1_bits
{
    unsigned long EN                :1;
    unsigned long SMMODE            :1;
    unsigned long SMBTYPE           :1;
    unsigned long ARPEN             :1;
    unsigned long PECEN             :1;
    unsigned long GCEN              :1;
    unsigned long NOEXTEND          :1;
    unsigned long STARTGEN          :1;
    unsigned long STOPGEN           :1;
    unsigned long ACKEN             :1;
    unsigned long ACKPOS            :1;
    unsigned long PEC               :1;
    unsigned long ALERT             :1;
    unsigned long SWRESET           :1;
    unsigned long MTHALL            :1;
    unsigned long NMENA             :1;
    unsigned long FIFOEN            :1;
    unsigned long FIFOCLR           :1;
    unsigned long Reserved1         :2;
    unsigned long THRFIFOF          :3;
    unsigned long Reserved2         :1;
    unsigned long THRFIFOE          :3;
    unsigned long HTC               :2;
    unsigned long LTC               :2;
    unsigned long Reserved3         :1;
};
typedef union
{
    struct _I2Cx_CTRL1_bits bit;
    unsigned long all;
} _I2Cx_CTRL1;
/***************************************************************************************************
 *      I2Cx_CTRL2 - I2C Control Register 2
***************************************************************************************************/
struct _I2Cx_CTRL2_bits
{
    unsigned long CLKFREQ           :8;
    unsigned long DMALAST           :1;
    unsigned long Reserved1         :3;
    unsigned long BUFINTEN          :1;
    unsigned long EVTINTEN          :1;
    unsigned long ERRINTEN          :1;
    unsigned long DMAEN             :1;
    unsigned long FIFOFINTEN        :1;
    unsigned long FIFOEINTEN        :1;
    unsigned long FIFOHFINTEN       :1;
    unsigned long FIFOHEINTEN       :1;
    unsigned long FIFORDEIEN        :1;
    unsigned long FIFOWREIEN        :1;
    unsigned long DMAETOEIEN        :1;
    unsigned long DMAFIFOEN         :1;
    unsigned long FIFONEINTEN       :1;
    unsigned long FIFONFINTEN       :1;
    unsigned long SDATOLIEN         :1;
    unsigned long SCLTOHIEN         :1;
    unsigned long SCLTOLIEN         :1;
    unsigned long TOHEN             :1;
    unsigned long TOLEN             :1;
    unsigned long Reserved2         :1;
};
typedef union
{
    struct _I2Cx_CTRL2_bits bit;
    unsigned long all;
} _I2Cx_CTRL2;
/***************************************************************************************************
 *      I2Cx_OADDR1 - I2C Own Address Register 1
***************************************************************************************************/
struct _I2Cx_OADDR1_bits
{
    unsigned long ADDR0             :1;
    unsigned long ADDR              :7;
    unsigned long ADDR1             :2;
    unsigned long Reserved1         :5;
    unsigned long ADDRMODE          :1;
    unsigned long Reserved2         :16;
};
typedef union
{
    struct _I2Cx_OADDR1_bits bit;
    unsigned long all;
} _I2Cx_OADDR1;
/***************************************************************************************************
 *      I2Cx_OADDR2 - I2C Own Address Register 2
***************************************************************************************************/
struct _I2Cx_OADDR2_bits
{
    unsigned long DUALEN            :1;
    unsigned long ADDR2             :7;
    unsigned long Reserved1         :24;
};
typedef union
{
    struct _I2Cx_OADDR2_bits bit;
    unsigned long all;
} _I2Cx_OADDR2;
/***************************************************************************************************
 *      I2Cx_DAT - I2C Data Register
***************************************************************************************************/
struct _I2Cx_DAT_bits
{
    unsigned long DATA              :8;
    unsigned long Reserved1         :24;
};
typedef union
{
    struct _I2Cx_DAT_bits bit;
    unsigned long all;
} _I2Cx_DAT;
/***************************************************************************************************
 *      I2Cx_STS1 - I2C Status Register 1
***************************************************************************************************/
struct _I2Cx_STS1_bits
{
    unsigned long STARTBF           :1;
    unsigned long ADDRF             :1;
    unsigned long BSF               :1;
    unsigned long STOPF             :1;
    unsigned long RXDATNE           :1;
    unsigned long TXDATE            :1;
    unsigned long ADDR10F           :1;
    unsigned long Reserved1         :1;
    unsigned long ACKFAIL           :1;
    unsigned long ARLOST            :1;
    unsigned long BUSERR            :1;
    unsigned long OVERRUN           :1;
    unsigned long PECERR            :1;
    unsigned long Reserved2         :1;
    unsigned long SMBALERT          :1;
    unsigned long Reserved3         :1;
    unsigned long FIFOF             :1;
    unsigned long FIFOE             :1;
    unsigned long FIFOHF            :1;
    unsigned long FIFOHE            :1;
    unsigned long FIFORDERR         :1;
    unsigned long FIFOWRERR         :1;
    unsigned long DMAETOERR         :1;
    unsigned long SDALTO            :1;
    unsigned long SCLHTO            :1;
    unsigned long SCLLTO            :1;
    unsigned long Reserved4         :6;
};
typedef union
{
    struct _I2Cx_STS1_bits bit;
    unsigned long all;
} _I2Cx_STS1;
/***************************************************************************************************
 *      I2Cx_STS2 - I2C Status Register 2
***************************************************************************************************/
struct _I2Cx_STS2_bits
{
    unsigned long BUSY              :1;
    unsigned long MSMODE            :1;
    unsigned long TRF               :1;
    unsigned long Reserved1         :1;
    unsigned long GCALLADDR         :1;
    unsigned long DUALFLAG          :1;
    unsigned long SMBDADDR          :1;
    unsigned long SMBHADDR          :1;
    unsigned long PECVAL            :8;
    unsigned long Reserved2         :16;
};
typedef union
{
    struct _I2Cx_STS2_bits bit;
    unsigned long all;
} _I2Cx_STS2;
/***************************************************************************************************
 *      I2Cx_CLKCTRL - I2C Clock Control Register
***************************************************************************************************/
struct _I2Cx_CLKCTRL_bits
{
    unsigned long CLKCTRL           :12;
    unsigned long Reserved1         :2;
    unsigned long FSMODE            :1;
    unsigned long DUTY              :1;
    unsigned long Reserved2         :16;
};
typedef union
{
    struct _I2Cx_CLKCTRL_bits bit;
    unsigned long all;
} _I2Cx_CLKCTRL;
/***************************************************************************************************
 *      I2Cx_TMRISE - I2C Rise Time Register
***************************************************************************************************/
struct _I2Cx_TMRISE_bits
{
    unsigned long TMRISE            :8;
    unsigned long Reserved1         :24;
};
typedef union
{
    struct _I2Cx_TMRISE_bits bit;
    unsigned long all;
} _I2Cx_TMRISE;
/***************************************************************************************************
 *      I2Cx_BYTENUM - I2C Filter Control Register
***************************************************************************************************/
struct _I2Cx_BYTENUM_bits
{
    unsigned long BYTENUM           :14;
    unsigned long STASTOLB          :1;
    unsigned long BTNUMEN           :1;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _I2Cx_BYTENUM_bits bit;
    unsigned long all;
} _I2Cx_BYTENUM;
/***************************************************************************************************
 *      I2Cx_GFLTRCTRL - I2C Digital Filter Control Register
***************************************************************************************************/
struct _I2Cx_GFLTRCTRL_bits
{
    unsigned long SDADFW            :4;
    unsigned long SCLDFW            :4;
    unsigned long SDAAFW            :2;
    unsigned long Reserved1         :1;
    unsigned long SDAAFENN          :1;
    unsigned long SCLAFW            :2;
    unsigned long Reserved2         :1;
    unsigned long SCLAFENN          :1;
    unsigned long Reserved3         :16;
};
typedef union
{
    struct _I2Cx_GFLTRCTRL_bits bit;
    unsigned long all;
} _I2Cx_GFLTRCTRL;
/***************************************************************************************************
 *      I2Cx_FIFODAT - I2C FIFO Data Register
***************************************************************************************************/
struct _I2Cx_FIFODAT_bits
{
    unsigned long FIFODAT           :8;
    unsigned long Reserved1         :24;
};
typedef union
{
    struct _I2Cx_FIFODAT_bits bit;
    unsigned long all;
} _I2Cx_FIFODAT;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _I2Cx_CTRL1 _I2C1_CTRL1 @(I2C1_BASE + 0x0000);
extern volatile _I2Cx_CTRL2 _I2C1_CTRL2 @(I2C1_BASE + 0x0004);
extern volatile _I2Cx_OADDR1 _I2C1_OADDR1 @(I2C1_BASE + 0x0008);
extern volatile _I2Cx_OADDR2 _I2C1_OADDR2 @(I2C1_BASE + 0x000C);
extern volatile _I2Cx_DAT _I2C1_DAT @(I2C1_BASE + 0x0010);
extern volatile _I2Cx_STS1 _I2C1_STS1 @(I2C1_BASE + 0x0014);
extern volatile _I2Cx_STS2 _I2C1_STS2 @(I2C1_BASE + 0x0018);
extern volatile _I2Cx_CLKCTRL _I2C1_CLKCTRL @(I2C1_BASE + 0x001C);
extern volatile _I2Cx_TMRISE _I2C1_TMRISE @(I2C1_BASE + 0x0020);
extern volatile _I2Cx_BYTENUM _I2C1_BYTENUM @(I2C1_BASE + 0x0024);
extern volatile _I2Cx_GFLTRCTRL _I2C1_GFLTRCTRL @(I2C1_BASE + 0x0028);
extern volatile _I2Cx_FIFODAT _I2C1_FIFODAT @(I2C1_BASE + 0x002C);

extern volatile _I2Cx_CTRL1 _I2C2_CTRL1 @(I2C2_BASE + 0x0000);
extern volatile _I2Cx_CTRL2 _I2C2_CTRL2 @(I2C2_BASE + 0x0004);
extern volatile _I2Cx_OADDR1 _I2C2_OADDR1 @(I2C2_BASE + 0x0008);
extern volatile _I2Cx_OADDR2 _I2C2_OADDR2 @(I2C2_BASE + 0x000C);
extern volatile _I2Cx_DAT _I2C2_DAT @(I2C2_BASE + 0x0010);
extern volatile _I2Cx_STS1 _I2C2_STS1 @(I2C2_BASE + 0x0014);
extern volatile _I2Cx_STS2 _I2C2_STS2 @(I2C2_BASE + 0x0018);
extern volatile _I2Cx_CLKCTRL _I2C2_CLKCTRL @(I2C2_BASE + 0x001C);
extern volatile _I2Cx_TMRISE _I2C2_TMRISE @(I2C2_BASE + 0x0020);
extern volatile _I2Cx_BYTENUM _I2C2_BYTENUM @(I2C2_BASE + 0x0024);
extern volatile _I2Cx_GFLTRCTRL _I2C2_GFLTRCTRL @(I2C2_BASE + 0x0028);
extern volatile _I2Cx_FIFODAT _I2C2_FIFODAT @(I2C2_BASE + 0x002C);

extern volatile _I2Cx_CTRL1 _I2C3_CTRL1 @(I2C3_BASE + 0x0000);
extern volatile _I2Cx_CTRL2 _I2C3_CTRL2 @(I2C3_BASE + 0x0004);
extern volatile _I2Cx_OADDR1 _I2C3_OADDR1 @(I2C3_BASE + 0x0008);
extern volatile _I2Cx_OADDR2 _I2C3_OADDR2 @(I2C3_BASE + 0x000C);
extern volatile _I2Cx_DAT _I2C3_DAT @(I2C3_BASE + 0x0010);
extern volatile _I2Cx_STS1 _I2C3_STS1 @(I2C3_BASE + 0x0014);
extern volatile _I2Cx_STS2 _I2C3_STS2 @(I2C3_BASE + 0x0018);
extern volatile _I2Cx_CLKCTRL _I2C3_CLKCTRL @(I2C3_BASE + 0x001C);
extern volatile _I2Cx_TMRISE _I2C3_TMRISE @(I2C3_BASE + 0x0020);
extern volatile _I2Cx_BYTENUM _I2C3_BYTENUM @(I2C3_BASE + 0x0024);
extern volatile _I2Cx_GFLTRCTRL _I2C3_GFLTRCTRL @(I2C3_BASE + 0x0028);
extern volatile _I2Cx_FIFODAT _I2C3_FIFODAT @(I2C3_BASE + 0x002C);

extern volatile _I2Cx_CTRL1 _I2C4_CTRL1 @(I2C4_BASE + 0x0000);
extern volatile _I2Cx_CTRL2 _I2C4_CTRL2 @(I2C4_BASE + 0x0004);
extern volatile _I2Cx_OADDR1 _I2C4_OADDR1 @(I2C4_BASE + 0x0008);
extern volatile _I2Cx_OADDR2 _I2C4_OADDR2 @(I2C4_BASE + 0x000C);
extern volatile _I2Cx_DAT _I2C4_DAT @(I2C4_BASE + 0x0010);
extern volatile _I2Cx_STS1 _I2C4_STS1 @(I2C4_BASE + 0x0014);
extern volatile _I2Cx_STS2 _I2C4_STS2 @(I2C4_BASE + 0x0018);
extern volatile _I2Cx_CLKCTRL _I2C4_CLKCTRL @(I2C4_BASE + 0x001C);
extern volatile _I2Cx_TMRISE _I2C4_TMRISE @(I2C4_BASE + 0x0020);
extern volatile _I2Cx_BYTENUM _I2C4_BYTENUM @(I2C4_BASE + 0x0024);
extern volatile _I2Cx_GFLTRCTRL _I2C4_GFLTRCTRL @(I2C4_BASE + 0x0028);
extern volatile _I2Cx_FIFODAT _I2C4_FIFODAT @(I2C4_BASE + 0x002C);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_I2C_H__ */
