/***********************************************************************************************************************
File Name: n32h48x_register_spi.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU SPI and I2S register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/24/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_SPI_H__
#define __N32H48x_REG_SPI_H__

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
 *      SPIx_CTRL1 - SPI Control Register 1
***************************************************************************************************/
struct _SPIx_CTRL1_bits
{
    unsigned long BR                :3;
    unsigned long Reserved1         :1;
    unsigned long CLKPOL            :1;
    unsigned long CLKPHA            :1;
    unsigned long MSEL              :1;
    unsigned long LSBFF             :1;
    unsigned long DATFF             :1;
    unsigned long CRCNEXT           :1;
    unsigned long SSOEN             :1;
    unsigned long SSEL              :1;
    unsigned long SSMEN             :1;
    unsigned long RONLY             :1;
    unsigned long BIDIROEN          :1;
    unsigned long BIDIRMODE         :1;
    unsigned long Reserved2         :16;
};
typedef union
{
    struct _SPIx_CTRL1_bits bit;
    unsigned long all;
} _SPIx_CTRL1;
/***************************************************************************************************
 *      SPIx_CTRL2 - SPI Control Register 2
***************************************************************************************************/
struct _SPIx_CTRL2_bits
{
    unsigned long SPIEN             :1;
    unsigned long RDMAEN            :1;
    unsigned long TDMAEN            :1;
    unsigned long CRCEN             :1;
    unsigned long TEINTEN           :1;
    unsigned long RNEINTEN          :1;
    unsigned long ERRINTEN          :1;
    unsigned long SS_POL            :1;
    unsigned long FIFOEN            :1;
    unsigned long FIFOCLR           :1;
    unsigned long TXFHEINTEN        :1;
    unsigned long RXFHFINTEN        :1;
    unsigned long RXFFUINTEN        :1;
    unsigned long CRCNMISEN         :1;
    unsigned long RXCPINTEN         :1;
    unsigned long Reserved1         :17;
};
typedef union
{
    struct _SPIx_CTRL2_bits bit;
    unsigned long all;
} _SPIx_CTRL2;
/***************************************************************************************************
 *      SPIx_STS - SPI Status Register
***************************************************************************************************/
struct _SPIx_STS_bits
{
    unsigned long TE                :1;
    unsigned long RNE               :1;
    unsigned long BUSY              :1;
    unsigned long CRCERR            :1;
    unsigned long MODERR            :1;
    unsigned long OVER              :1;
    unsigned long UNDER             :1;
    unsigned long CHSIDE            :1;
    unsigned long TXFIFHE           :1;
    unsigned long RXFIFHF           :1;
    unsigned long TXFIFFU           :1;
    unsigned long RXFIFFU           :1;
    unsigned long RXTSCP            :1;
    unsigned long Reserved1         :19;
};
typedef union
{
    struct _SPIx_STS_bits bit;
    unsigned long all;
} _SPIx_STS;
/***************************************************************************************************
 *      SPIx_DAT - SPI Data Register
***************************************************************************************************/
struct _SPIx_DAT_bits
{
    unsigned long DAT               :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _SPIx_DAT_bits bit;
    unsigned long all;
} _SPIx_DAT;
/***************************************************************************************************
 *      SPIx_CRCTDAT - SPI Transmit CRC Register
***************************************************************************************************/
struct _SPIx_CRCTDAT_bits
{
    unsigned long CRCTDAT           :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _SPIx_CRCTDAT_bits bit;
    unsigned long all;
} _SPIx_CRCTDAT;
/***************************************************************************************************
 *      SPIx_CRCRDAT - SPI Receive CRC Register
***************************************************************************************************/
struct _SPIx_CRCRDAT_bits
{
    unsigned long CRCRDAT           :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _SPIx_CRCRDAT_bits bit;
    unsigned long all;
} _SPIx_CRCRDAT;
/***************************************************************************************************
 *      SPIx_CRCPOLY - SPI CRC Polynomial Register
***************************************************************************************************/
struct _SPIx_CRCPOLY_bits
{
    unsigned long CRCPOLY           :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _SPIx_CRCPOLY_bits bit;
    unsigned long all;
} _SPIx_CRCPOLY;
/***************************************************************************************************
 *      SPIx_I2Sx_CFGR - SPI_I2S Configuration Register
***************************************************************************************************/
struct _SPIx_I2Sx_CFGR_bits
{
    unsigned long I2SE              :1;
    unsigned long I2SMOD            :1;
    unsigned long I2SSTD            :2;
    unsigned long I2SCFG            :2;
    unsigned long DATLEN            :2;
    unsigned long CHLEN             :1;
    unsigned long PCMSYNC           :1;
    unsigned long CLKPOL            :1;
    unsigned long PCMBYPASS         :1;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _SPIx_I2Sx_CFGR_bits bit;
    unsigned long all;
} _SPIx_I2Sx_CFGR;
/***************************************************************************************************
 *      SPIx_I2Sx_PREDIV - SPI_I2S Prescaler Register
***************************************************************************************************/
struct _SPIx_I2Sx_PREDIV_bits
{
    unsigned long LDIV              :10;
    unsigned long ODDEVEN           :1;
    unsigned long MCLKOEN           :1;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _SPIx_I2Sx_PREDIV_bits bit;
    unsigned long all;
} _SPIx_I2Sx_PREDIV;
/***************************************************************************************************
 *      SPIx_RX_FIFO - SPI Receive FIFO Register
***************************************************************************************************/
struct _SPIx_RX_FIFO_bits
{
    unsigned long RXFIFDAT          :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _SPIx_RX_FIFO_bits bit;
    unsigned long all;
} _SPIx_RX_FIFO;
/***************************************************************************************************
 *      SPIx_FIFO_NUM - SPI FIFO Configuration Register
***************************************************************************************************/
struct _SPIx_FIFO_NUM_bits
{
    unsigned long TXFBSNUM          :3;
    unsigned long Reserved1         :1;
    unsigned long RXFBSNUM          :3;
    unsigned long Reserved2         :25;
};
typedef union
{
    struct _SPIx_FIFO_NUM_bits bit;
    unsigned long all;
} _SPIx_FIFO_NUM;
/***************************************************************************************************
 *      SPIx_FIFO_CNT - SPI FIFO Count Register
***************************************************************************************************/
struct _SPIx_FIFO_CNT_bits
{
    unsigned long TXFIFCNT          :4;
    unsigned long RXFIFCNT          :4;
    unsigned long Reserved1         :24;
};
typedef union
{
    struct _SPIx_FIFO_CNT_bits bit;
    unsigned long all;
} _SPIx_FIFO_CNT;
/***************************************************************************************************
 *      SPIx_TRANS_NUM - SPI Transfer Number Configuration Register
***************************************************************************************************/
struct _SPIx_TRANS_NUM_bits
{
    unsigned long TRANSNUM          :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _SPIx_TRANS_NUM_bits bit;
    unsigned long all;
} _SPIx_TRANS_NUM;
/***************************************************************************************************
 *      SPIx_CR3 - SPI RX Sample Delay Register
***************************************************************************************************/
struct _SPIx_CR3_bits
{
    unsigned long DELAYTIME         :4;
    unsigned long Reserved1         :28;
};
typedef union
{
    struct _SPIx_CR3_bits bit;
    unsigned long all;
} _SPIx_CR3;
/***************************************************************************************************
 *      I2Sx_CTRL2 - I2S_EXT Control Register
***************************************************************************************************/
struct _I2Sx_CTRL2_bits
{
    unsigned long Reserved1         :1;
    unsigned long RDMAEN            :1;
    unsigned long TDMAEN            :1;
    unsigned long Reserved2         :1;
    unsigned long TEINTEN           :1;
    unsigned long RNEINTEN          :1;
    unsigned long ERRINTEN          :1;
    unsigned long Reserved3         :25;
};
typedef union
{
    struct _I2Sx_CTRL2_bits bit;
    unsigned long all;
} _I2Sx_CTRL2;
/***************************************************************************************************
 *      I2Sx_STS - I2S_EXT Status Register
***************************************************************************************************/
struct _I2Sx_STS_bits
{
    unsigned long TE                :1;
    unsigned long RNE               :1;
    unsigned long BUSY              :1;
    unsigned long Reserved1         :2;
    unsigned long OVER              :1;
    unsigned long UNDER             :1;
    unsigned long CHSIDE            :1;
    unsigned long Reserved2         :24;
};
typedef union
{
    struct _I2Sx_STS_bits bit;
    unsigned long all;
} _I2Sx_STS;
/***************************************************************************************************
 *      I2Sx_DAT - I2S_EXT Data Register
***************************************************************************************************/
struct _I2Sx_DAT_bits
{
    unsigned long DAT               :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _I2Sx_DAT_bits bit;
    unsigned long all;
} _I2Sx_DAT;
/***************************************************************************************************
 *      I2Sx_CFGR - I2S_EXT Configuration Register
***************************************************************************************************/
struct _I2Sx_CFGR_bits
{
    unsigned long I2SE              :1;
    unsigned long I2SMOD            :1;
    unsigned long I2SSTD            :2;
    unsigned long I2SCFG            :2;
    unsigned long DATLEN            :2;
    unsigned long CHLEN             :1;
    unsigned long PCMSYNC           :1;
    unsigned long CLKPOL            :1;
    unsigned long Reserved1         :21;
};
typedef union
{
    struct _I2Sx_CFGR_bits bit;
    unsigned long all;
} _I2Sx_CFGR;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _SPIx_CTRL1 _SPI1_CTRL1 @(SPI1_BASE + 0x0000);
extern volatile _SPIx_CTRL2 _SPI1_CTRL2 @(SPI1_BASE + 0x0004);
extern volatile _SPIx_STS _SPI1_STS @(SPI1_BASE + 0x0008);
extern volatile _SPIx_DAT _SPI1_DAT @(SPI1_BASE + 0x000C);
extern volatile _SPIx_CRCTDAT _SPI1_CRCTDAT @(SPI1_BASE + 0x0010);
extern volatile _SPIx_CRCRDAT _SPI1_CRCRDAT @(SPI1_BASE + 0x0014);
extern volatile _SPIx_CRCPOLY _SPI1_CRCPOLY @(SPI1_BASE + 0x0018);
extern volatile _SPIx_RX_FIFO _SPI1_RX_FIFO @(SPI1_BASE + 0x0024);
extern volatile _SPIx_FIFO_NUM _SPI1_FIFO_NUM @(SPI1_BASE + 0x0028);
extern volatile _SPIx_FIFO_CNT _SPI1_FIFO_CNT @(SPI1_BASE + 0x0030);
extern volatile _SPIx_TRANS_NUM _SPI1_TRANS_NUM @(SPI1_BASE + 0x0034);
extern volatile _SPIx_CR3 _SPI1_CR3 @(SPI1_BASE + 0x0038);

extern volatile _SPIx_CTRL1 _SPI2_CTRL1 @(SPI2_BASE + 0x0000);
extern volatile _SPIx_CTRL2 _SPI2_CTRL2 @(SPI2_BASE + 0x0004);
extern volatile _SPIx_STS _SPI2_STS @(SPI2_BASE + 0x0008);
extern volatile _SPIx_DAT _SPI2_DAT @(SPI2_BASE + 0x000C);
extern volatile _SPIx_CRCTDAT _SPI2_CRCTDAT @(SPI2_BASE + 0x0010);
extern volatile _SPIx_CRCRDAT _SPI2_CRCRDAT @(SPI2_BASE + 0x0014);
extern volatile _SPIx_CRCPOLY _SPI2_CRCPOLY @(SPI2_BASE + 0x0018);
extern volatile _SPIx_RX_FIFO _SPI2_RX_FIFO @(SPI2_BASE + 0x0024);
extern volatile _SPIx_FIFO_NUM _SPI2_FIFO_NUM @(SPI2_BASE + 0x0028);
extern volatile _SPIx_FIFO_CNT _SPI2_FIFO_CNT @(SPI2_BASE + 0x0030);
extern volatile _SPIx_TRANS_NUM _SPI2_TRANS_NUM @(SPI2_BASE + 0x0034);
extern volatile _SPIx_CR3 _SPI2_CR3 @(SPI2_BASE + 0x0038);
extern volatile _SPIx_I2Sx_CFGR _SPI2_I2S2_CFGR @(SPI2_BASE + 0x001C);
extern volatile _SPIx_I2Sx_PREDIV _SPI2_I2S2_PREDIV @(SPI2_BASE + 0x0020);
extern volatile _I2Sx_CTRL2 _I2S2_CTRL2 @(SPI2_BASE + 0x0204);
extern volatile _I2Sx_STS _I2S2_STS @(SPI2_BASE + 0x0208);
extern volatile _I2Sx_DAT _I2S2_DAT @(SPI2_BASE + 0x020C);
extern volatile _I2Sx_CFGR _I2S2_CFGR @(SPI2_BASE + 0x021C);

extern volatile _SPIx_CTRL1 _SPI3_CTRL1 @(SPI3_BASE + 0x0000);
extern volatile _SPIx_CTRL2 _SPI3_CTRL2 @(SPI3_BASE + 0x0004);
extern volatile _SPIx_STS _SPI3_STS @(SPI3_BASE + 0x0008);
extern volatile _SPIx_DAT _SPI3_DAT @(SPI3_BASE + 0x000C);
extern volatile _SPIx_CRCTDAT _SPI3_CRCTDAT @(SPI3_BASE + 0x0010);
extern volatile _SPIx_CRCRDAT _SPI3_CRCRDAT @(SPI3_BASE + 0x0014);
extern volatile _SPIx_CRCPOLY _SPI3_CRCPOLY @(SPI3_BASE + 0x0018);
extern volatile _SPIx_RX_FIFO _SPI3_RX_FIFO @(SPI3_BASE + 0x0024);
extern volatile _SPIx_FIFO_NUM _SPI3_FIFO_NUM @(SPI3_BASE + 0x0028);
extern volatile _SPIx_FIFO_CNT _SPI3_FIFO_CNT @(SPI3_BASE + 0x0030);
extern volatile _SPIx_TRANS_NUM _SPI3_TRANS_NUM @(SPI3_BASE + 0x0034);
extern volatile _SPIx_CR3 _SPI3_CR3 @(SPI3_BASE + 0x0038);
extern volatile _SPIx_I2Sx_CFGR _SPI3_I2S3_CFGR @(SPI3_BASE + 0x001C);
extern volatile _SPIx_I2Sx_PREDIV _SPI3_I2S3_PREDIV @(SPI3_BASE + 0x0020);
extern volatile _I2Sx_CTRL2 _I2S3_CTRL2 @(SPI3_BASE + 0x0204);
extern volatile _I2Sx_STS _I2S3_STS @(SPI3_BASE + 0x0208);
extern volatile _I2Sx_DAT _I2S3_DAT @(SPI3_BASE + 0x020C);
extern volatile _I2Sx_CFGR _I2S3_CFGR @(SPI3_BASE + 0x021C);

extern volatile _SPIx_CTRL1 _SPI4_CTRL1 @(SPI4_BASE + 0x0000);
extern volatile _SPIx_CTRL2 _SPI4_CTRL2 @(SPI4_BASE + 0x0004);
extern volatile _SPIx_STS _SPI4_STS @(SPI4_BASE + 0x0008);
extern volatile _SPIx_DAT _SPI4_DAT @(SPI4_BASE + 0x000C);
extern volatile _SPIx_CRCTDAT _SPI4_CRCTDAT @(SPI4_BASE + 0x0010);
extern volatile _SPIx_CRCRDAT _SPI4_CRCRDAT @(SPI4_BASE + 0x0014);
extern volatile _SPIx_CRCPOLY _SPI4_CRCPOLY @(SPI4_BASE + 0x0018);
extern volatile _SPIx_RX_FIFO _SPI4_RX_FIFO @(SPI4_BASE + 0x0024);
extern volatile _SPIx_FIFO_NUM _SPI4_FIFO_NUM @(SPI4_BASE + 0x0028);
extern volatile _SPIx_FIFO_CNT _SPI4_FIFO_CNT @(SPI4_BASE + 0x0030);
extern volatile _SPIx_TRANS_NUM _SPI4_TRANS_NUM @(SPI4_BASE + 0x0034);
extern volatile _SPIx_CR3 _SPI4_CR3 @(SPI4_BASE + 0x0038);

extern volatile _SPIx_CTRL1 _SPI5_CTRL1 @(SPI5_BASE + 0x0000);
extern volatile _SPIx_CTRL2 _SPI5_CTRL2 @(SPI5_BASE + 0x0004);
extern volatile _SPIx_STS _SPI5_STS @(SPI5_BASE + 0x0008);
extern volatile _SPIx_DAT _SPI5_DAT @(SPI5_BASE + 0x000C);
extern volatile _SPIx_CRCTDAT _SPI5_CRCTDAT @(SPI5_BASE + 0x0010);
extern volatile _SPIx_CRCRDAT _SPI5_CRCRDAT @(SPI5_BASE + 0x0014);
extern volatile _SPIx_CRCPOLY _SPI5_CRCPOLY @(SPI5_BASE + 0x0018);
extern volatile _SPIx_RX_FIFO _SPI5_RX_FIFO @(SPI5_BASE + 0x0024);
extern volatile _SPIx_FIFO_NUM _SPI5_FIFO_NUM @(SPI5_BASE + 0x0028);
extern volatile _SPIx_FIFO_CNT _SPI5_FIFO_CNT @(SPI5_BASE + 0x0030);
extern volatile _SPIx_TRANS_NUM _SPI5_TRANS_NUM @(SPI5_BASE + 0x0034);
extern volatile _SPIx_CR3 _SPI5_CR3 @(SPI5_BASE + 0x0038);

extern volatile _SPIx_CTRL1 _SPI6_CTRL1 @(SPI6_BASE + 0x0000);
extern volatile _SPIx_CTRL2 _SPI6_CTRL2 @(SPI6_BASE + 0x0004);
extern volatile _SPIx_STS _SPI6_STS @(SPI6_BASE + 0x0008);
extern volatile _SPIx_DAT _SPI6_DAT @(SPI6_BASE + 0x000C);
extern volatile _SPIx_CRCTDAT _SPI6_CRCTDAT @(SPI6_BASE + 0x0010);
extern volatile _SPIx_CRCRDAT _SPI6_CRCRDAT @(SPI6_BASE + 0x0014);
extern volatile _SPIx_CRCPOLY _SPI6_CRCPOLY @(SPI6_BASE + 0x0018);
extern volatile _SPIx_RX_FIFO _SPI6_RX_FIFO @(SPI6_BASE + 0x0024);
extern volatile _SPIx_FIFO_NUM _SPI6_FIFO_NUM @(SPI6_BASE + 0x0028);
extern volatile _SPIx_FIFO_CNT _SPI6_FIFO_CNT @(SPI6_BASE + 0x0030);
extern volatile _SPIx_TRANS_NUM _SPI6_TRANS_NUM @(SPI6_BASE + 0x0034);
extern volatile _SPIx_CR3 _SPI6_CR3 @(SPI6_BASE + 0x0038);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_SPI_H__ */
