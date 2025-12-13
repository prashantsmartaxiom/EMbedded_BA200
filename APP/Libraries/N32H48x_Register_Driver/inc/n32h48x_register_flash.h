/***********************************************************************************************************************
File Name: n32h48x_register_flash.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU FLASH register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    11/20/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_FLASH_H__
#define __N32H48x_REG_FLASH_H__

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
 *      FLASHx_AC - Flash access control register
***************************************************************************************************/
struct _FLASHx_AC_bits
{
    unsigned long LATENCY       :3;
    unsigned long Reserved1     :1;
    unsigned long ICAHEN        :1;
    unsigned long ICAHRST       :1;
    unsigned long PRFTBFS       :1;
    unsigned long PRFTBFE       :1;
    unsigned long Reserved2     :24;
};
typedef union
{
    struct _FLASHx_AC_bits bit;
    unsigned long all;
} _FLASHx_AC;
/***********************************************************************************************************************
 *      FLASHx_CTRL - Flash control register
***********************************************************************************************************************/
struct _FLASHx_CTRL_bits
{
    unsigned long LOCK          :1;
    unsigned long PG            :1;
    unsigned long PER           :1;
    unsigned long MER           :1;
    unsigned long START         :1;
    unsigned long OPTPG         :1;
    unsigned long OPTER         :1;
    unsigned long OPTWE         :1;
    unsigned long EOPITE        :1;
    unsigned long ERRITE        :1;
    unsigned long FERRITE       :1;
    unsigned long EC1ERRITE     :1;
    unsigned long JSERRITE      :1;
    unsigned long EC2ERRITE     :1;
    unsigned long DECCERRITE    :1;
    unsigned long RPADDERRITE   :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _FLASHx_CTRL_bits bit;
    unsigned long all;
} _FLASHx_CTRL;
/***********************************************************************************************************************
 *      FLASHx_STS - Flash status register
***********************************************************************************************************************/
struct _FLASHx_STS_bits
{
    unsigned long EOP           :1;
    unsigned long BUSY          :1;
    unsigned long PGERR         :1;
    unsigned long WRPERR        :1;
    unsigned long PVERR         :1;
    unsigned long EVERR         :1;
    unsigned long ECC1ERR       :1;
    unsigned long RDKEYERR      :1;
    unsigned long RDXKEYERR     :1;
    unsigned long NRDKEYEN      :1;
    unsigned long JSERR         :1;
    unsigned long RTPDKEYERR    :1;
    unsigned long ECC2ERR       :1;
    unsigned long DECCRDF       :1;
    unsigned long DECCERR       :1;
    unsigned long FWORDF        :1;
    unsigned long RPADDERR      :1;
    unsigned long Reserved1     :15;
};
typedef union
{
    struct _FLASHx_STS_bits bit;
    unsigned long all;
} _FLASHx_STS;
/***********************************************************************************************************************
 *      FLASHx_ADD - Flash address register
***********************************************************************************************************************/
struct _FLASHx_ADD_bits
{
    unsigned long FADD          :32;
};
typedef union
{
    struct _FLASHx_ADD_bits bit;
    unsigned long all;
} _FLASHx_ADD;
/***********************************************************************************************************************
 *      FLASHx_KEY - Flash key register
***********************************************************************************************************************/
struct _FLASHx_KEY_bits
{
    unsigned long FKEY          :32;
};
typedef union
{
    struct _FLASHx_KEY_bits bit;
    unsigned long all;
} _FLASHx_KEY;
/***********************************************************************************************************************
 *      FLASHx_OPTKEY - Flash option key register
***********************************************************************************************************************/
struct _FLASHx_OPTKEY_bits
{
    unsigned long OPTKEY        :32;
};
typedef union
{
    struct _FLASHx_OPTKEY_bits bit;
    unsigned long all;
} _FLASHx_OPTKEY;
/***********************************************************************************************************************
 *      FLASHx_OB - Flash option register
***********************************************************************************************************************/
struct _FLASHx_OB_bits
{
    unsigned long RDPRT1        :1;
    unsigned long RDPRT2        :1;
    unsigned long WDG_SW        :1;
    unsigned long nRST_STOP     :1;
    unsigned long nRST_STDBY    :1;
    unsigned long IWDGSTOPFRZ   :1;
    unsigned long Reserved1     :1;
    unsigned long IWDGSTDBYFR   :1;
    unsigned long IWDGSLEEPFRZ  :1;
    unsigned long Reserved2     :3;
    unsigned long Data0         :8;
    unsigned long Data1         :8;
    unsigned long nBOOT0        :1;
    unsigned long nBOOT1        :1;
    unsigned long nSWBOOT0      :1;
    unsigned long OBERR         :1;
};
typedef union
{
    struct _FLASHx_OB_bits bit;
    unsigned long all;
} _FLASHx_OB;
/***********************************************************************************************************************
 *      FLASHx_WRP - Flash write protection register
***********************************************************************************************************************/
struct _FLASHx_WRP_bits
{
    unsigned long WRPT          :32;
};
typedef union
{
    struct _FLASHx_WRP_bits bit;
    unsigned long all;
} _FLASHx_WRP;
/***********************************************************************************************************************
 *      FLASHx_ECC - Flash ECC register
***********************************************************************************************************************/
struct _FLASHx_ECC_bits
{
    unsigned long ECCLW         :6;
    unsigned long Reserved1     :2;
    unsigned long ECCHW         :6;
    unsigned long Reserved2     :18;
};
typedef union
{
    struct _FLASHx_ECC_bits bit;
    unsigned long all;
} _FLASHx_ECC;
/***********************************************************************************************************************
 *      FLASHx_RDN - Flash RDN register
***********************************************************************************************************************/
struct _FLASHx_RDN_bits
{
    unsigned long FLASH_RDN0    :9;
    unsigned long Reserved1     :7;
    unsigned long FLASH_RDN1    :9;
    unsigned long Reserved2     :7;
};
typedef union
{
    struct _FLASHx_RDN_bits bit;
    unsigned long all;
} _FLASHx_RDN;
/***********************************************************************************************************************
 *      FLASHx_CAHR - Flash CAHR register
***********************************************************************************************************************/
struct _FLASHx_CAHR_bits
{
    unsigned long LOCKSTRT      :4;
    unsigned long LOCKSTOP      :4;
    unsigned long Reserved1     :24;
};
typedef union
{
    struct _FLASHx_CAHR_bits bit;
    unsigned long all;
} _FLASHx_CAHR;
/***********************************************************************************************************************
 *      FLASHx_ROWP - Flash row progtamming register
***********************************************************************************************************************/
struct _FLASHx_ROWP_bits
{
    unsigned long RPEN          :1;
    unsigned long RPNUM         :4;
    unsigned long RPADD         :17;
    unsigned long RPAREA        :1;
    unsigned long Reserved1     :9;
};
typedef union
{
    struct _FLASHx_ROWP_bits bit;
    unsigned long all;
} _FLASHx_ROWP;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _FLASHx_AC _FLASH_AC @(FLASH_R_BASE + 0x0000);
extern volatile _FLASHx_CTRL _FLASH_CTRL @(FLASH_R_BASE + 0x0004);
extern volatile _FLASHx_STS _FLASH_STS @(FLASH_R_BASE + 0x0008);
extern volatile _FLASHx_ADD _FLASH_ADD @(FLASH_R_BASE + 0x000C);
extern volatile _FLASHx_KEY _FLASH_KEY @(FLASH_R_BASE + 0x0010);
extern volatile _FLASHx_OPTKEY _FLASH_OPTKEY @(FLASH_R_BASE + 0x0014);
extern volatile _FLASHx_OB _FLASH_OB @(FLASH_R_BASE + 0x0018);
extern volatile _FLASHx_WRP _FLASH_WRP @(FLASH_R_BASE + 0x001C);
extern volatile _FLASHx_ECC _FLASH_ECC @(FLASH_R_BASE + 0x0020);
extern volatile _FLASHx_RDN _FLASH_RDN @(FLASH_R_BASE + 0x002C);
extern volatile _FLASHx_CAHR _FLASH_CAHR @(FLASH_R_BASE + 0x0030);
extern volatile _FLASHx_ROWP _FLASH_ROWP @(FLASH_R_BASE + 0x0034);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_FLASH_H__ */
