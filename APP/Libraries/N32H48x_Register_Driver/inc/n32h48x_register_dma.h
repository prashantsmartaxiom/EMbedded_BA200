/***********************************************************************************************************************
File Name: n32h48x_register_dma.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU DMA register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/26/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_DMA_H__
#define __N32H48x_REG_DMA_H__

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
 *      DMAx_CHCFGy - DMA Channel x Configuration Register
***************************************************************************************************/
struct _DMAx_CHCFGy_bits
{
    unsigned long CHEN              :1;
    unsigned long TXCIE             :1;
    unsigned long HTXIE             :1;
    unsigned long ERRIE             :1;
    unsigned long DIR               :1;
    unsigned long CIRC              :1;
    unsigned long PINC              :1;
    unsigned long MINC              :1;
    unsigned long PSIZE             :2;
    unsigned long MSIZE             :2;
    unsigned long PRIOLVL           :2;
    unsigned long MEM2MEM           :1;
    unsigned long BURST_MODE        :1;
    unsigned long BLEN              :4;
    unsigned long Reserved1         :12;
};
typedef union
{
    struct _DMAx_CHCFGy_bits bit;
    unsigned long all;
} _DMAx_CHCFGy;
/***************************************************************************************************
 *      DMAx_MADDRy - DMA Channel x Memory Address Register
***************************************************************************************************/
struct _DMAx_MADDRy_bits
{
    unsigned long ADDR              :32;
};
typedef union
{
    struct _DMAx_MADDRy_bits bit;
    unsigned long all;
} _DMAx_MADDRy;
/***************************************************************************************************
 *      DMAx_PADDRy - DMA Channel x Peripheral Address Register
***************************************************************************************************/
struct _DMAx_PADDRy_bits
{
    unsigned long ADDR              :32;
};
typedef union
{
    struct _DMAx_PADDRy_bits bit;
    unsigned long all;
} _DMAx_PADDRy;
/***************************************************************************************************
 *      DMAx_CHSELy - DMA Channel x Request Select Register
***************************************************************************************************/
struct _DMAx_CHSELy_bits
{
    unsigned long CH_SEL            :8;
    unsigned long Reserved1         :24;
};
typedef union
{
    struct _DMAx_CHSELy_bits bit;
    unsigned long all;
} _DMAx_CHSELy;
/***************************************************************************************************
 *      DMAx_TXNUMy - DMA Channel x Transfer Number Register
***************************************************************************************************/
struct _DMAx_TXNUMy_bits
{
    unsigned long NDTX              :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _DMAx_TXNUMy_bits bit;
    unsigned long all;
} _DMAx_TXNUMy;
/***************************************************************************************************
 *      DMAx_INTCLR - DMA Interrupt Flag Clear Register
***************************************************************************************************/
struct _DMAx_INTCLR_bits
{
    unsigned long CGLBF1            :1;
    unsigned long CTXCF1            :1;
    unsigned long CHTXF1            :1;
    unsigned long CERRF1            :1;
    unsigned long CGLBF2            :1;
    unsigned long CTXCF2            :1;
    unsigned long CHTXF2            :1;
    unsigned long CERRF2            :1;
    unsigned long CGLBF3            :1;
    unsigned long CTXCF3            :1;
    unsigned long CHTXF3            :1;
    unsigned long CERRF3            :1;
    unsigned long CGLBF4            :1;
    unsigned long CTXCF4            :1;
    unsigned long CHTXF4            :1;
    unsigned long CERRF4            :1;
    unsigned long CGLBF5            :1;
    unsigned long CTXCF5            :1;
    unsigned long CHTXF5            :1;
    unsigned long CERRF5            :1;
    unsigned long CGLBF6            :1;
    unsigned long CTXCF6            :1;
    unsigned long CHTXF6            :1;
    unsigned long CERRF6            :1;
    unsigned long CGLBF7            :1;
    unsigned long CTXCF7            :1;
    unsigned long CHTXF7            :1;
    unsigned long CERRF7            :1;
    unsigned long CGLBF8            :1;
    unsigned long CTXCF8            :1;
    unsigned long CHTXF8            :1;
    unsigned long CERRF8            :1;
};
typedef union
{
    struct _DMAx_INTCLR_bits bit;
    unsigned long all;
} _DMAx_INTCLR;
/***************************************************************************************************
 *      DMAx_INTSTS - DMA Interrupt Status Register
***************************************************************************************************/
struct _DMAx_INTSTS_bits
{
    unsigned long GLBF1             :1;
    unsigned long TXCF1             :1;
    unsigned long HTXF1             :1;
    unsigned long ERRF1             :1;
    unsigned long GLBF2             :1;
    unsigned long TXCF2             :1;
    unsigned long HTXF2             :1;
    unsigned long ERRF2             :1;
    unsigned long GLBF3             :1;
    unsigned long TXCF3             :1;
    unsigned long HTXF3             :1;
    unsigned long ERRF3             :1;
    unsigned long GLBF4             :1;
    unsigned long TXCF4             :1;
    unsigned long HTXF4             :1;
    unsigned long ERRF4             :1;
    unsigned long GLBF5             :1;
    unsigned long TXCF5             :1;
    unsigned long HTXF5             :1;
    unsigned long ERRF5             :1;
    unsigned long GLBF6             :1;
    unsigned long TXCF6             :1;
    unsigned long HTXF6             :1;
    unsigned long ERRF6             :1;
    unsigned long GLBF7             :1;
    unsigned long TXCF7             :1;
    unsigned long HTXF7             :1;
    unsigned long ERRF7             :1;
    unsigned long GLBF8             :1;
    unsigned long TXCF8             :1;
    unsigned long HTXF8             :1;
    unsigned long ERRF8             :1;
};
typedef union
{
    struct _DMAx_INTSTS_bits bit;
    unsigned long all;
} _DMAx_INTSTS;
/***************************************************************************************************
 *      DMAx_BURSTEN - DMA Burst Transfer Enable Register
***************************************************************************************************/
struct _DMAx_BURSTEN_bits
{
    unsigned long BURST_BYPASS      :1;
    unsigned long Reserved1         :31;
};
typedef union
{
    struct _DMAx_BURSTEN_bits bit;
    unsigned long all;
} _DMAx_BURSTEN;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _DMAx_CHCFGy _DMA1_CHCFG1 @(DMA1_CH1_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG2 @(DMA1_CH2_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG3 @(DMA1_CH3_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG4 @(DMA1_CH4_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG5 @(DMA1_CH5_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG6 @(DMA1_CH6_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG7 @(DMA1_CH7_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA1_CHCFG8 @(DMA1_CH8_BASE + 0x0000);
extern volatile _DMAx_MADDRy _DMA1_MADDR1 @(DMA1_CH1_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR2 @(DMA1_CH2_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR3 @(DMA1_CH3_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR4 @(DMA1_CH4_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR5 @(DMA1_CH5_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR6 @(DMA1_CH6_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR7 @(DMA1_CH7_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA1_MADDR8 @(DMA1_CH8_BASE + 0x0004);
extern volatile _DMAx_PADDRy _DMA1_PADDR1 @(DMA1_CH1_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR2 @(DMA1_CH2_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR3 @(DMA1_CH3_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR4 @(DMA1_CH4_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR5 @(DMA1_CH5_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR6 @(DMA1_CH6_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR7 @(DMA1_CH7_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA1_PADDR8 @(DMA1_CH8_BASE + 0x0008);
extern volatile _DMAx_CHSELy _DMA1_CHSEL1 @(DMA1_CH1_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL2 @(DMA1_CH2_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL3 @(DMA1_CH3_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL4 @(DMA1_CH4_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL5 @(DMA1_CH5_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL6 @(DMA1_CH6_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL7 @(DMA1_CH7_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA1_CHSEL8 @(DMA1_CH8_BASE + 0x000C);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM1 @(DMA1_CH1_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM2 @(DMA1_CH2_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM3 @(DMA1_CH3_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM4 @(DMA1_CH4_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM5 @(DMA1_CH5_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM6 @(DMA1_CH6_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM7 @(DMA1_CH7_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA1_TXNUM8 @(DMA1_CH8_BASE + 0x0010);
extern volatile _DMAx_INTCLR _DMA1_INTCLR @(DMA1_BASE + 0x00A0);
extern volatile _DMAx_INTSTS _DMA1_INTSTS @(DMA1_BASE + 0x00A4);
extern volatile _DMAx_BURSTEN _DMA1_BURSTEN @(DMA1_BASE + 0x00A8);

extern volatile _DMAx_CHCFGy _DMA2_CHCFG1 @(DMA2_CH1_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG2 @(DMA2_CH2_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG3 @(DMA2_CH3_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG4 @(DMA2_CH4_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG5 @(DMA2_CH5_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG6 @(DMA2_CH6_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG7 @(DMA2_CH7_BASE + 0x0000);
extern volatile _DMAx_CHCFGy _DMA2_CHCFG8 @(DMA2_CH8_BASE + 0x0000);
extern volatile _DMAx_MADDRy _DMA2_MADDR1 @(DMA2_CH1_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR2 @(DMA2_CH2_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR3 @(DMA2_CH3_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR4 @(DMA2_CH4_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR5 @(DMA2_CH5_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR6 @(DMA2_CH6_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR7 @(DMA2_CH7_BASE + 0x0004);
extern volatile _DMAx_MADDRy _DMA2_MADDR8 @(DMA2_CH8_BASE + 0x0004);
extern volatile _DMAx_PADDRy _DMA2_PADDR1 @(DMA2_CH1_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR2 @(DMA2_CH2_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR3 @(DMA2_CH3_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR4 @(DMA2_CH4_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR5 @(DMA2_CH5_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR6 @(DMA2_CH6_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR7 @(DMA2_CH7_BASE + 0x0008);
extern volatile _DMAx_PADDRy _DMA2_PADDR8 @(DMA2_CH8_BASE + 0x0008);
extern volatile _DMAx_CHSELy _DMA2_CHSEL1 @(DMA2_CH1_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL2 @(DMA2_CH2_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL3 @(DMA2_CH3_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL4 @(DMA2_CH4_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL5 @(DMA2_CH5_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL6 @(DMA2_CH6_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL7 @(DMA2_CH7_BASE + 0x000C);
extern volatile _DMAx_CHSELy _DMA2_CHSEL8 @(DMA2_CH8_BASE + 0x000C);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM1 @(DMA2_CH1_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM2 @(DMA2_CH2_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM3 @(DMA2_CH3_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM4 @(DMA2_CH4_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM5 @(DMA2_CH5_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM6 @(DMA2_CH6_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM7 @(DMA2_CH7_BASE + 0x0010);
extern volatile _DMAx_TXNUMy _DMA2_TXNUM8 @(DMA2_CH8_BASE + 0x0010);
extern volatile _DMAx_INTCLR _DMA2_INTCLR @(DMA2_BASE + 0x00A0);
extern volatile _DMAx_INTSTS _DMA2_INTSTS @(DMA2_BASE + 0x00A4);
extern volatile _DMAx_BURSTEN _DMA2_BURSTEN @(DMA2_BASE + 0x00A8);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_DMA_H__ */
