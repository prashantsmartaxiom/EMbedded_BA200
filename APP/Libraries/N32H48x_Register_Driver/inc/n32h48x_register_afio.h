/***********************************************************************************************************************
File Name: n32h48x_register_afio.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU AFIO register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/11/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_AFIO_H__
#define __N32H48x_REG_AFIO_H__

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
 *      AFIOx_RMP_CFG - AFIO remap configuration register
***************************************************************************************************/
struct _AFIOx_RMP_CFG_bits
{
    unsigned long Reserved1         :8;
    unsigned long SPI1_NSS          :1;
    unsigned long SPI2_NSS          :1;
    unsigned long SPI3_NSS          :1;
    unsigned long SPI4_NSS          :1;
    unsigned long SPI5_NSS          :1;
    unsigned long SPI6_NSS          :1;
    unsigned long SWJ_CFG           :3;
    unsigned long IO_FILTER         :6;
    unsigned long FEMC_NADV         :1;
    unsigned long EXTI_FILTER       :1;
    unsigned long XSPI_XIP_WR_BE    :1;
    unsigned long XSPI_HALF_DUPLEX  :1;
    unsigned long XSPI_DUAL_QUAD    :1;
    unsigned long ETH_MODE          :1;
    unsigned long XSPI_NSS_IN       :1;
    unsigned long XSPI_NSS_SEL      :1;
    unsigned long XSPI_FLASH2       :1;
};
typedef union
{
    struct _AFIOx_RMP_CFG_bits bit;
    unsigned long all;
} _AFIOx_RMP_CFG;
/***************************************************************************************************
 *      AFIOx_EXTI_CFG1 - AFIO external interrupt configuration 1 register
***************************************************************************************************/
struct _AFIOx_EXTI_CFG1_bits
{
    unsigned long EXTI0             :7;
    unsigned long Reserved1         :1;
    unsigned long EXTI1             :7;
    unsigned long Reserved2         :1;
    unsigned long EXTI2             :7;
    unsigned long Reserved3         :1;
    unsigned long EXTI3             :7;
    unsigned long Reserved4         :1;
};
typedef union
{
    struct _AFIOx_EXTI_CFG1_bits bit;
    unsigned long all;
} _AFIOx_EXTI_CFG1;
/***************************************************************************************************
 *      AFIOx_EXTI_CFG2 - AFIO external interrupt configuration 2 register
***************************************************************************************************/
struct _AFIOx_EXTI_CFG2_bits
{
    unsigned long EXTI4             :7;
    unsigned long Reserved1         :1;
    unsigned long EXTI5             :7;
    unsigned long Reserved2         :1;
    unsigned long EXTI6             :7;
    unsigned long Reserved3         :1;
    unsigned long EXTI7             :7;
    unsigned long Reserved4         :1;
};
typedef union
{
    struct _AFIOx_EXTI_CFG2_bits bit;
    unsigned long all;
} _AFIOx_EXTI_CFG2;
/***************************************************************************************************
 *      AFIOx_EXTI_CFG3 - AFIO external interrupt configuration 3 register
***************************************************************************************************/
struct _AFIOx_EXTI_CFG3_bits
{
    unsigned long EXTI8             :7;
    unsigned long Reserved1         :1;
    unsigned long EXTI9             :7;
    unsigned long Reserved2         :1;
    unsigned long EXTI10            :7;
    unsigned long Reserved3         :1;
    unsigned long EXTI11            :7;
    unsigned long Reserved4         :1;
};
typedef union
{
    struct _AFIOx_EXTI_CFG3_bits bit;
    unsigned long all;
} _AFIOx_EXTI_CFG3;
/***************************************************************************************************
 *      AFIOx_EXTI_CFG4 - AFIO external interrupt configuration 4 register
***************************************************************************************************/
struct _AFIOx_EXTI_CFG4_bits
{
    unsigned long EXTI12            :7;
    unsigned long Reserved1         :1;
    unsigned long EXTI13            :7;
    unsigned long Reserved2         :1;
    unsigned long EXTI14            :7;
    unsigned long Reserved3         :1;
    unsigned long EXTI15            :7;
    unsigned long Reserved4         :1;
};
typedef union
{
    struct _AFIOx_EXTI_CFG4_bits bit;
    unsigned long all;
} _AFIOx_EXTI_CFG4;
/***************************************************************************************************
 *      AFIOx_TOL5V_CFG1 - AFIO port 5V tolerance configuration 1 register
***************************************************************************************************/
struct _AFIOx_TOL5V_CFG1_bits
{
    unsigned long PA0TOLEN          :1;
    unsigned long PA1TOLEN          :1;
    unsigned long PA2TOLEN          :1;
    unsigned long PA3TOLEN          :1;
    unsigned long PA4TOLEN          :1;
    unsigned long PA5TOLEN          :1;
    unsigned long PA6TOLEN          :1;
    unsigned long PA7TOLEN          :1;
    unsigned long PA8TOLEN          :1;
    unsigned long PA9TOLEN          :1;
    unsigned long PA10TOLEN         :1;
    unsigned long Reserved1         :5;
    unsigned long PB0TOLEN          :1;
    unsigned long PB1TOLEN          :1;
    unsigned long PB2TOLEN          :1;
    unsigned long Reserved2         :7;
    unsigned long PB10TOLEN         :1;
    unsigned long PB11TOLEN         :1;
    unsigned long PB12TOLEN         :1;
    unsigned long PB13TOLEN         :1;
    unsigned long PB14TOLEN         :1;
    unsigned long PB15TOLEN         :1;
};
typedef union
{
    struct _AFIOx_TOL5V_CFG1_bits bit;
    unsigned long all;
} _AFIOx_TOL5V_CFG1;
/***************************************************************************************************
 *      AFIOx_TOL5V_CFG2 - AFIO port 5V tolerance configuration 2 register
***************************************************************************************************/
struct _AFIOx_TOL5V_CFG2_bits
{
    unsigned long PC0TOLEN          :1;
    unsigned long PC1TOLEN          :1;
    unsigned long PC2TOLEN          :1;
    unsigned long PC3TOLEN          :1;
    unsigned long PC4TOLEN          :1;
    unsigned long PC5TOLEN          :1;
    unsigned long Reserved1         :3;
    unsigned long PC9TOLEN          :1;
    unsigned long Reserved2         :14;
    unsigned long PD8TOLEN          :1;
    unsigned long PD9TOLEN          :1;
    unsigned long PD10TOLEN         :1;
    unsigned long PD11TOLEN         :1;
    unsigned long PD12TOLEN         :1;
    unsigned long PD13TOLEN         :1;
    unsigned long PD14TOLEN         :1;
    unsigned long PD15TOLEN         :1;
};
typedef union
{
    struct _AFIOx_TOL5V_CFG2_bits bit;
    unsigned long all;
} _AFIOx_TOL5V_CFG2;
/***************************************************************************************************
 *      AFIOx_TOL5V_CFG3 - AFIO port 5V tolerance configuration 3 register
***************************************************************************************************/
struct _AFIOx_TOL5V_CFG3_bits
{
    unsigned long PE0TOLEN          :1;
    unsigned long PE1TOLEN          :1;
    unsigned long PE2TOLEN          :1;
    unsigned long PE3TOLEN          :1;
    unsigned long PE4TOLEN          :1;
    unsigned long PE5TOLEN          :1;
    unsigned long PE6TOLEN          :1;
    unsigned long PE7TOLEN          :1;
    unsigned long PE8TOLEN          :1;
    unsigned long PE9TOLEN          :1;
    unsigned long PE10TOLEN         :1;
    unsigned long PE11TOLEN         :1;
    unsigned long PE12TOLEN         :1;
    unsigned long PE13TOLEN         :1;
    unsigned long PE14TOLEN         :1;
    unsigned long PE15TOLEN         :1;
    unsigned long PF0TOLEN          :1;
    unsigned long PF1TOLEN          :1;
    unsigned long PF2TOLEN          :1;
    unsigned long PF3TOLEN          :1;
    unsigned long PF4TOLEN          :1;
    unsigned long PF5TOLEN          :1;
    unsigned long PF6TOLEN          :1;
    unsigned long PF7TOLEN          :1;
    unsigned long PF8TOLEN          :1;
    unsigned long PF9TOLEN          :1;
    unsigned long PF10TOLEN         :1;
    unsigned long Reserved1         :5;
};
typedef union
{
    struct _AFIOx_TOL5V_CFG3_bits bit;
    unsigned long all;
} _AFIOx_TOL5V_CFG3;
/***************************************************************************************************
 *      AFIOx_TOL5V_CFG4 - AFIO port 5V tolerance configuration 4 register
***************************************************************************************************/
struct _AFIOx_TOL5V_CFG4_bits
{
    unsigned long PG0TOLEN          :1;
    unsigned long PG1TOLEN          :1;
    unsigned long PG2TOLEN          :1;
    unsigned long PG3TOLEN          :1;
    unsigned long PG4TOLEN          :1;
    unsigned long PG5TOLEN          :1;
    unsigned long PG6TOLEN          :1;
    unsigned long PG7TOLEN          :1;
    unsigned long PG8TOLEN          :1;
    unsigned long PG9TOLEN          :1;
    unsigned long PG10TOLEN         :1;
    unsigned long PG11TOLEN         :1;
    unsigned long PG12TOLEN         :1;
    unsigned long PG13TOLEN         :1;
    unsigned long PG14TOLEN         :1;
    unsigned long PG15TOLEN         :1;
    unsigned long PH0TOLEN          :1;
    unsigned long PH1TOLEN          :1;
    unsigned long PH2TOLEN          :1;
    unsigned long PH3TOLEN          :1;
    unsigned long PH4TOLEN          :1;
    unsigned long PH5TOLEN          :1;
    unsigned long PH6TOLEN          :1;
    unsigned long Reserved1         :9;
};
typedef union
{
    struct _AFIOx_TOL5V_CFG4_bits bit;
    unsigned long all;
} _AFIOx_TOL5V_CFG4;
/***************************************************************************************************
 *      AFIOx_FILTER_CFG1 - AFIO port digital filter configuration 1 register
***************************************************************************************************/
struct _AFIOx_FILTER_CFG1_bits
{
    unsigned long PA_FILTER         :16;
    unsigned long PB_FILTER         :16;
};
typedef union
{
    struct _AFIOx_FILTER_CFG1_bits bit;
    unsigned long all;
} _AFIOx_FILTER_CFG1;
/***************************************************************************************************
 *      AFIOx_FILTER_CFG2 - AFIO port digital filter configuration 2 register
***************************************************************************************************/
struct _AFIOx_FILTER_CFG2_bits
{
    unsigned long PC_FILTER         :16;
    unsigned long PD_FILTER         :16;
};
typedef union
{
    struct _AFIOx_FILTER_CFG2_bits bit;
    unsigned long all;
} _AFIOx_FILTER_CFG2;
/***************************************************************************************************
 *      AFIOx_FILTER_CFG3 - AFIO port digital filter configuration 3 register
***************************************************************************************************/
struct _AFIOx_FILTER_CFG3_bits
{
    unsigned long PE_FILTER         :16;
    unsigned long PF_FILTER         :16;
};
typedef union
{
    struct _AFIOx_FILTER_CFG3_bits bit;
    unsigned long all;
} _AFIOx_FILTER_CFG3;
/***************************************************************************************************
 *      AFIOx_FILTER_CFG4 - AFIO port digital filter configuration 4 register
***************************************************************************************************/
struct _AFIOx_FILTER_CFG4_bits
{
    unsigned long PG_FILTER         :16;
    unsigned long PH_FILTER         :7;
    unsigned long Reserved1         :9;
};
typedef union
{
    struct _AFIOx_FILTER_CFG4_bits bit;
    unsigned long all;
} _AFIOx_FILTER_CFG4;
/***************************************************************************************************
 *      AFIOx_EMC_CFG - AFIO EMC configuration register
***************************************************************************************************/
struct _AFIOx_EMC_CFG_bits
{
    unsigned long CLAMP1_RST        :1;
    unsigned long CLAMP2_RST        :1;
    unsigned long CLAMP3_RST        :1;
    unsigned long CLAMP4_RST        :1;
    unsigned long GBN1_RST          :1;
    unsigned long GBN2_RST          :1;
    unsigned long GBN3_RST          :1;
    unsigned long GBN4_RST          :1;
    unsigned long GB1_RST           :1;
    unsigned long GB2_RST           :1;
    unsigned long GB3_RST           :1;
    unsigned long GB4_RST           :1;
    unsigned long CLAMP1_DET        :1;
    unsigned long CLAMP2_DET        :1;
    unsigned long CLAMP3_DET        :1;
    unsigned long CLAMP4_DET        :1;
    unsigned long GBN1_DET          :1;
    unsigned long GBN2_DET          :1;
    unsigned long GBN3_DET          :1;
    unsigned long GBN4_DET          :1;
    unsigned long GB1_DET           :1;
    unsigned long GB2_DET           :1;
    unsigned long GB3_DET           :1;
    unsigned long GB4_DET           :1;
    unsigned long XSPI_RXDS         :2;
    unsigned long XSPI_WR_BE        :1;
    unsigned long XSPI_RD_BE        :1;
    unsigned long XSPI_EXTEND       :2;
    unsigned long Reserved1         :2;
};
typedef union
{
    struct _AFIOx_EMC_CFG_bits bit;
    unsigned long all;
} _AFIOx_EMC_CFG;
/***************************************************************************************************
 *      AFIOx_EMC_CNT - AFIO EMC count register
***************************************************************************************************/
struct _AFIOx_EMC_CNT_bits
{
    unsigned long EMC_CNT           :10;
    unsigned long XSPI_NSS_HIGH     :4;
    unsigned long PC_RD_DELAY_EN    :1;
    unsigned long Reserved1         :17;
};
typedef union
{
    struct _AFIOx_EMC_CNT_bits bit;
    unsigned long all;
} _AFIOx_EMC_CNT;
/***************************************************************************************************
 *      AFIOx_SHRT_EXEV_CFG1 - AFIO SHRTIM1 external event configuration 1 register
***************************************************************************************************/
struct _AFIOx_SHRT_EXEV_CFG1_bits
{
    unsigned long SHRT1_EXEV1       :7;
    unsigned long Reserved1         :1;
    unsigned long SHRT1_EXEV2       :7;
    unsigned long Reserved2         :1;
    unsigned long SHRT1_EXEV3       :7;
    unsigned long Reserved3         :1;
    unsigned long SHRT1_EXEV4       :7;
    unsigned long Reserved4         :1;
};
typedef union
{
    struct _AFIOx_SHRT_EXEV_CFG1_bits bit;
    unsigned long all;
} _AFIOx_SHRT_EXEV_CFG1;
/***************************************************************************************************
 *      AFIOx_SHRT_EXEV_CFG2 - AFIO SHRTIM1 external event configuration 2 register
***************************************************************************************************/
struct _AFIOx_SHRT_EXEV_CFG2_bits
{
    unsigned long SHRT1_EXEV5       :7;
    unsigned long Reserved1         :1;
    unsigned long SHRT1_EXEV6       :7;
    unsigned long Reserved2         :1;
    unsigned long SHRT1_EXEV7       :7;
    unsigned long Reserved3         :1;
    unsigned long SHRT1_EXEV8       :7;
    unsigned long Reserved4         :1;
};
typedef union
{
    struct _AFIOx_SHRT_EXEV_CFG2_bits bit;
    unsigned long all;
} _AFIOx_SHRT_EXEV_CFG2;
/***************************************************************************************************
 *      AFIOx_SHRT_EXEV_CFG3 - AFIO SHRTIM1 external event configuration 3 register
***************************************************************************************************/
struct _AFIOx_SHRT_EXEV_CFG3_bits
{
    unsigned long SHRT1_EXEV9       :7;
    unsigned long Reserved1         :1;
    unsigned long SHRT1_EXEV10      :7;
    unsigned long Reserved2         :17;
};
typedef union
{
    struct _AFIOx_SHRT_EXEV_CFG3_bits bit;
    unsigned long all;
} _AFIOx_SHRT_EXEV_CFG3;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _AFIOx_RMP_CFG _AFIO_RMP_CFG @(AFIO_BASE + 0x0000);
extern volatile _AFIOx_EXTI_CFG1 _AFIO_EXTI_CFG1 @(AFIO_BASE + 0x0004);
extern volatile _AFIOx_EXTI_CFG2 _AFIO_EXTI_CFG2 @(AFIO_BASE + 0x0008);
extern volatile _AFIOx_EXTI_CFG3 _AFIO_EXTI_CFG3 @(AFIO_BASE + 0x000C);
extern volatile _AFIOx_EXTI_CFG4 _AFIO_EXTI_CFG4 @(AFIO_BASE + 0x0010);
extern volatile _AFIOx_TOL5V_CFG1 _AFIO_TOL5V_CFG1 @(AFIO_BASE + 0x0014);
extern volatile _AFIOx_TOL5V_CFG2 _AFIO_TOL5V_CFG2 @(AFIO_BASE + 0x0018);
extern volatile _AFIOx_TOL5V_CFG3 _AFIO_TOL5V_CFG3 @(AFIO_BASE + 0x001C);
extern volatile _AFIOx_TOL5V_CFG4 _AFIO_TOL5V_CFG4 @(AFIO_BASE + 0x0020);
extern volatile _AFIOx_FILTER_CFG1 _AFIO_FILTER_CFG1 @(AFIO_BASE + 0x0024);
extern volatile _AFIOx_FILTER_CFG2 _AFIO_FILTER_CFG2 @(AFIO_BASE + 0x0028);
extern volatile _AFIOx_FILTER_CFG3 _AFIO_FILTER_CFG3 @(AFIO_BASE + 0x002C);
extern volatile _AFIOx_FILTER_CFG4 _AFIO_FILTER_CFG4 @(AFIO_BASE + 0x0030);
extern volatile _AFIOx_EMC_CFG _AFIO_EMC_CFG @(AFIO_BASE + 0x0034);
extern volatile _AFIOx_EMC_CNT _AFIO_EMC_CNT @(AFIO_BASE + 0x0038);
extern volatile _AFIOx_SHRT_EXEV_CFG1 _AFIO_SHRT_EXEV_CFG1 @(AFIO_BASE + 0x003C);
extern volatile _AFIOx_SHRT_EXEV_CFG2 _AFIO_SHRT_EXEV_CFG2 @(AFIO_BASE + 0x0040);
extern volatile _AFIOx_SHRT_EXEV_CFG3 _AFIO_SHRT_EXEV_CFG3 @(AFIO_BASE + 0x0044);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_AFIO_H__ */
