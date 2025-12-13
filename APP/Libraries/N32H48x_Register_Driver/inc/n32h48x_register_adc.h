/***********************************************************************************************************************
File Name: n32h48x_register_adc.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU ADC register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/20/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_ADC_H__
#define __N32H48x_REG_ADC_H__

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
 *      ADCx_STS - ADC Status Register
***************************************************************************************************/
struct _ADCx_STS_bits
{
    unsigned long ENDC              :1;
    unsigned long ENDCA             :1;
    unsigned long JENDC             :1;
    unsigned long JENDCA            :1;
    unsigned long AWDG1             :1;
    unsigned long ENDCERR           :1;
    unsigned long RDY               :1;
    unsigned long PDRDY             :1;
    unsigned long EOSAMP            :1;
    unsigned long TCFLAG            :1;
    unsigned long STR               :1;
    unsigned long JSTR              :1;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_STS_bits bit;
    unsigned long all;
} _ADCx_STS;
/***************************************************************************************************
 *      ADCx_CTRL1 - ADC Control Register 1
***************************************************************************************************/
struct _ADCx_CTRL1_bits
{
    unsigned long SCANMD            :1;
    unsigned long AUTOJC            :1;
    unsigned long DCTU              :3;
    unsigned long DREGCH            :1;
    unsigned long DJCH              :1;
    unsigned long MULTMODE          :5;
    unsigned long AWD1CH            :5;
    unsigned long AWD1ERCH          :1;
    unsigned long AWD1EJCH          :1;
    unsigned long AWD1SGLEN         :1;
    unsigned long DELAY             :4;
    unsigned long Reserved1         :8;
};
typedef union
{
    struct _ADCx_CTRL1_bits bit;
    unsigned long all;
} _ADCx_CTRL1;
/***************************************************************************************************
 *      ADCx_CTRL2 - ADC Control Register 2
***************************************************************************************************/
struct _ADCx_CTRL2_bits
{
    unsigned long ON                :1;
    unsigned long CTU               :1;
    unsigned long ENCAL             :1;
    unsigned long RSTCAL            :1;
    unsigned long EXTPRSEL          :2;
    unsigned long EXTPJSEL          :2;
    unsigned long Reserved1         :2;
    unsigned long EXTJSEL           :6;
    unsigned long EXTRSEL           :6;
    unsigned long DMAMD             :2;
    unsigned long ALIG              :1;
    unsigned long TEMPEN            :1;
    unsigned long GCOMPEN           :1;
    unsigned long SWSTRJCH          :1;
    unsigned long SWSTRRCH          :1;
    unsigned long SWJSTOP           :1;
    unsigned long SWRSTOP           :1;
    unsigned long Reserved2         :1;
};
typedef union
{
    struct _ADCx_CTRL2_bits bit;
    unsigned long all;
} _ADCx_CTRL2;
/***************************************************************************************************
 *      ADCx_CTRL3 - ADC Control Register 3
***************************************************************************************************/
struct _ADCx_CTRL3_bits
{
    unsigned long RES               :2;
    unsigned long CALDIF            :1;
    unsigned long CALALD            :1;
    unsigned long CKMOD             :1;
    unsigned long BPCAL             :1;
    unsigned long DPWMOD            :1;
    unsigned long VBATMEN           :1;
    unsigned long OSR               :4;
    unsigned long OSS               :4;
    unsigned long OSRE              :1;
    unsigned long OSJE              :1;
    unsigned long OSRTRIG           :1;
    unsigned long OSRMD             :1;
    unsigned long INP1SEL           :1;
    unsigned long INN1SEL           :1;
    unsigned long INP2SEL           :1;
    unsigned long Reserved1         :9;
};
typedef union
{
    struct _ADCx_CTRL3_bits bit;
    unsigned long all;
} _ADCx_CTRL3;
/***************************************************************************************************
 *      ADCx_SAMPT1 - ADC Sampling Time Register 1
***************************************************************************************************/
struct _ADCx_SAMPT1_bits
{
    unsigned long SAMP0             :4;
    unsigned long SAMP1             :4;
    unsigned long SAMP2             :4;
    unsigned long SAMP3             :4;
    unsigned long SAMP4             :4;
    unsigned long SAMP5             :4;
    unsigned long SAMP6             :4;
    unsigned long SAMP7             :4;
};
typedef union
{
    struct _ADCx_SAMPT1_bits bit;
    unsigned long all;
} _ADCx_SAMPT1;
/***************************************************************************************************
 *      ADCx_SAMPT2 - ADC Sampling Time Register 2
***************************************************************************************************/
struct _ADCx_SAMPT2_bits
{
    unsigned long SAMP8             :4;
    unsigned long SAMP9             :4;
    unsigned long SAMP10            :4;
    unsigned long SAMP11            :4;
    unsigned long SAMP12            :4;
    unsigned long SAMP13            :4;
    unsigned long SAMP14            :4;
    unsigned long SAMP15            :4;
};
typedef union
{
    struct _ADCx_SAMPT2_bits bit;
    unsigned long all;
} _ADCx_SAMPT2;
/***************************************************************************************************
 *      ADCx_SAMPT3 - ADC Sampling Time Register 3
***************************************************************************************************/
struct _ADCx_SAMPT3_bits
{
    unsigned long SAMP16            :4;
    unsigned long SAMP17            :4;
    unsigned long SAMP18            :4;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_SAMPT3_bits bit;
    unsigned long all;
} _ADCx_SAMPT3;
/***************************************************************************************************
 *      ADCx_DIFSEL - ADC Differential Mode Selection Register
***************************************************************************************************/
struct _ADCx_DIFSEL_bits
{
    unsigned long Reserved1         :1;
    unsigned long DIFSEL            :18;
    unsigned long Reserved2         :13;
};
typedef union
{
    struct _ADCx_DIFSEL_bits bit;
    unsigned long all;
} _ADCx_DIFSEL;
/***************************************************************************************************
 *      ADCx_CALFACT - ADC Calibration Factor Register
***************************************************************************************************/
struct _ADCx_CALFACT_bits
{
    unsigned long CALFACTS          :7;
    unsigned long Reserved1         :9;
    unsigned long CALFACTD          :7;
    unsigned long Reserved2         :9;
};
typedef union
{
    struct _ADCx_CALFACT_bits bit;
    unsigned long all;
} _ADCx_CALFACT;
/***************************************************************************************************
 *      ADCx_OFFSET1 - ADC Data Offset Register 1
***************************************************************************************************/
struct _ADCx_OFFSET1_bits
{
    unsigned long OFFSCH1DAT        :12;
    unsigned long Reserved1         :12;
    unsigned long OFFSCH1DIR        :1;
    unsigned long OFFSCH1SATEN      :1;
    unsigned long OFFSCH1CH         :5;
    unsigned long OFFSCH1EN         :1;
};
typedef union
{
    struct _ADCx_OFFSET1_bits bit;
    unsigned long all;
} _ADCx_OFFSET1;
/***************************************************************************************************
 *      ADCx_OFFSET2 - ADC Data Offset Register 2
***************************************************************************************************/
struct _ADCx_OFFSET2_bits
{
    unsigned long OFFSCH1DAT        :12;
    unsigned long Reserved1         :12;
    unsigned long OFFSCH1DIR        :1;
    unsigned long OFFSCH1SATEN      :1;
    unsigned long OFFSCH1CH         :5;
    unsigned long OFFSCH1EN         :1;
};
typedef union
{
    struct _ADCx_OFFSET2_bits bit;
    unsigned long all;
} _ADCx_OFFSET2;
/***************************************************************************************************
 *      ADCx_OFFSET3 - ADC Data Offset Register 3
***************************************************************************************************/
struct _ADCx_OFFSET3_bits
{
    unsigned long OFFSCH1DAT        :12;
    unsigned long Reserved1         :12;
    unsigned long OFFSCH1DIR        :1;
    unsigned long OFFSCH1SATEN      :1;
    unsigned long OFFSCH1CH         :5;
    unsigned long OFFSCH1EN         :1;
};
typedef union
{
    struct _ADCx_OFFSET3_bits bit;
    unsigned long all;
} _ADCx_OFFSET3;
/***************************************************************************************************
 *      ADCx_OFFSET4 - ADC Data Offset Register 4
***************************************************************************************************/
struct _ADCx_OFFSET4_bits
{
    unsigned long OFFSCH1DAT        :12;
    unsigned long Reserved1         :12;
    unsigned long OFFSCH1DIR        :1;
    unsigned long OFFSCH1SATEN      :1;
    unsigned long OFFSCH1CH         :5;
    unsigned long OFFSCH1EN         :1;
};
typedef union
{
    struct _ADCx_OFFSET4_bits bit;
    unsigned long all;
} _ADCx_OFFSET4;
/***************************************************************************************************
 *      ADCx_AWD1HIGH - ADC Watchdog 1 High Threshold Register
***************************************************************************************************/
struct _ADCx_AWD1HIGH_bits
{
    unsigned long HTH1              :12;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_AWD1HIGH_bits bit;
    unsigned long all;
} _ADCx_AWD1HIGH;
/***************************************************************************************************
 *      ADCx_AWD1LOW - ADC Watchdog 1 Low Threshold Rgister
***************************************************************************************************/
struct _ADCx_AWD1LOW_bits
{
    unsigned long LTH1              :12;
    unsigned long AWDFIL            :3;
    unsigned long Reserved1         :17;
};
typedef union
{
    struct _ADCx_AWD1LOW_bits bit;
    unsigned long all;
} _ADCx_AWD1LOW;
/***************************************************************************************************
 *      ADCx_AWD2HIGH - ADC Watchdog 2 High Threshold Register
***************************************************************************************************/
struct _ADCx_AWD2HIGH_bits
{
    unsigned long HTH2              :12;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_AWD2HIGH_bits bit;
    unsigned long all;
} _ADCx_AWD2HIGH;
/***************************************************************************************************
 *      ADCx_AWD2LOW - ADC Watchdog 2 Low Threshold Rgister
***************************************************************************************************/
struct _ADCx_AWD2LOW_bits
{
    unsigned long LTH2              :12;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_AWD2LOW_bits bit;
    unsigned long all;
} _ADCx_AWD2LOW;
/***************************************************************************************************
 *      ADCx_AWD3HIGH - ADC Watchdog 3 High Threshold Register
***************************************************************************************************/
struct _ADCx_AWD3HIGH_bits
{
    unsigned long HTH3              :12;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_AWD3HIGH_bits bit;
    unsigned long all;
} _ADCx_AWD3HIGH;
/***************************************************************************************************
 *      ADCx_AWD3LOW - ADC Watchdog 3 Low Threshold Rgister
***************************************************************************************************/
struct _ADCx_AWD3LOW_bits
{
    unsigned long LTH3              :12;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_AWD3LOW_bits bit;
    unsigned long all;
} _ADCx_AWD3LOW;
/***************************************************************************************************
 *      ADCx_AWD2EN - ADC Analog Watchdog 2 Configuration Register
***************************************************************************************************/
struct _ADCx_AWD2EN_bits
{
    unsigned long AWD2EN            :19;
    unsigned long Reserved1         :13;
};
typedef union
{
    struct _ADCx_AWD2EN_bits bit;
    unsigned long all;
} _ADCx_AWD2EN;
/***************************************************************************************************
 *      ADCx_AWD3EN - ADC Analog Watchdog 3 Configuration Register
***************************************************************************************************/
struct _ADCx_AWD3EN_bits
{
    unsigned long AWD3EN            :19;
    unsigned long Reserved1         :13;
};
typedef union
{
    struct _ADCx_AWD3EN_bits bit;
    unsigned long all;
} _ADCx_AWD3EN;
/***************************************************************************************************
 *      ADCx_AWD2INTEN - ADC Analog Watchdog 2 Interrupt Enable Register
***************************************************************************************************/
struct _ADCx_AWD2INTEN_bits
{
    unsigned long AWD2INTEN         :19;
    unsigned long Reserved1         :13;
};
typedef union
{
    struct _ADCx_AWD2INTEN_bits bit;
    unsigned long all;
} _ADCx_AWD2INTEN;
/***************************************************************************************************
 *      ADCx_AWD3INTEN - ADC Analog Watchdog 3 Interrupt Enable Register
***************************************************************************************************/
struct _ADCx_AWD3INTEN_bits
{
    unsigned long AWD3INTEN         :19;
    unsigned long Reserved1         :13;
};
typedef union
{
    struct _ADCx_AWD3INTEN_bits bit;
    unsigned long all;
} _ADCx_AWD3INTEN;
/***************************************************************************************************
 *      ADCx_AWD2STS - ADC Analog Watchdog 2 Status Register
***************************************************************************************************/
struct _ADCx_AWD2STS_bits
{
    unsigned long AWD2FLAG          :19;
    unsigned long Reserved1         :13;
};
typedef union
{
    struct _ADCx_AWD2STS_bits bit;
    unsigned long all;
} _ADCx_AWD2STS;
/***************************************************************************************************
 *      ADCx_AWD3STS - ADC Analog Watchdog 3 Status Register
***************************************************************************************************/
struct _ADCx_AWD3STS_bits
{
    unsigned long AWD3FLAG          :19;
    unsigned long Reserved1         :13;
};
typedef union
{
    struct _ADCx_AWD3STS_bits bit;
    unsigned long all;
} _ADCx_AWD3STS;
/***************************************************************************************************
 *      ADCx_RSEQ1 - ADC Regular Sequence Register 1
***************************************************************************************************/
struct _ADCx_RSEQ1_bits
{
    unsigned long SEQ1              :5;
    unsigned long SEQ2              :5;
    unsigned long SEQ3              :5;
    unsigned long SEQ4              :5;
    unsigned long SEQ5              :5;
    unsigned long SEQ6              :5;
    unsigned long Reserved1         :2;
};
typedef union
{
    struct _ADCx_RSEQ1_bits bit;
    unsigned long all;
} _ADCx_RSEQ1;
/***************************************************************************************************
 *      ADCx_RSEQ2 - ADC Regular Sequence Register 2
***************************************************************************************************/
struct _ADCx_RSEQ2_bits
{
    unsigned long SEQ7              :5;
    unsigned long SEQ8              :5;
    unsigned long SEQ9              :5;
    unsigned long SEQ10             :5;
    unsigned long SEQ11             :5;
    unsigned long SEQ12             :5;
    unsigned long Reserved1         :2;
};
typedef union
{
    struct _ADCx_RSEQ2_bits bit;
    unsigned long all;
} _ADCx_RSEQ2;
/***************************************************************************************************
 *      ADCx_RSEQ3 - ADC Regular Sequence Register 3
***************************************************************************************************/
struct _ADCx_RSEQ3_bits
{
    unsigned long SEQ13             :5;
    unsigned long SEQ14             :5;
    unsigned long SEQ15             :5;
    unsigned long SEQ16             :5;
    unsigned long Reserved1         :5;
    unsigned long LEN               :4;
    unsigned long Reserved2         :3;
};
typedef union
{
    struct _ADCx_RSEQ3_bits bit;
    unsigned long all;
} _ADCx_RSEQ3;
/***************************************************************************************************
 *      ADCx_JSEQ - ADC Injected Sequence Register
***************************************************************************************************/
struct _ADCx_JSEQ_bits
{
    unsigned long JSEQ1             :5;
    unsigned long JSEQ2             :5;
    unsigned long JSEQ3             :5;
    unsigned long JSEQ4             :5;
    unsigned long Reserved1         :5;
    unsigned long JLEN              :2;
    unsigned long Reserved2         :5;
};
typedef union
{
    struct _ADCx_JSEQ_bits bit;
    unsigned long all;
} _ADCx_JSEQ;
/***************************************************************************************************
 *      ADCx_JDAT1 - ADC Injection Data Register 1
***************************************************************************************************/
struct _ADCx_JDAT1_bits
{
    unsigned long JDAT1             :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _ADCx_JDAT1_bits bit;
    unsigned long all;
} _ADCx_JDAT1;
/***************************************************************************************************
 *      ADCx_JDAT2 - ADC Injection Data Register 2
***************************************************************************************************/
struct _ADCx_JDAT2_bits
{
    unsigned long JDAT2             :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _ADCx_JDAT2_bits bit;
    unsigned long all;
} _ADCx_JDAT2;
/***************************************************************************************************
 *      ADCx_JDAT3 - ADC Injection Data Register 3
***************************************************************************************************/
struct _ADCx_JDAT3_bits
{
    unsigned long JDAT3             :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _ADCx_JDAT3_bits bit;
    unsigned long all;
} _ADCx_JDAT3;
/***************************************************************************************************
 *      ADCx_JDAT4 - ADC Injection Data Register 4
***************************************************************************************************/
struct _ADCx_JDAT4_bits
{
    unsigned long JDAT4             :16;
    unsigned long Reserved1         :16;
};
typedef union
{
    struct _ADCx_JDAT4_bits bit;
    unsigned long all;
} _ADCx_JDAT4;
/***************************************************************************************************
 *      ADCx_DAT - ADC Regulars Data Register
***************************************************************************************************/
struct _ADCx_DAT_bits
{
    unsigned long DAT               :16;
    unsigned long ADC2DAT           :16;
};
typedef union
{
    struct _ADCx_DAT_bits bit;
    unsigned long all;
} _ADCx_DAT;
/***************************************************************************************************
 *      ADCx_FIFOCFG - ADC FIFO Configuration Register
***************************************************************************************************/
struct _ADCx_FIFOCFG_bits
{
    unsigned long REINTEN           :1;
    unsigned long WEINTEN           :1;
    unsigned long FINTEN            :1;
    unsigned long EINTEN            :1;
    unsigned long HFINTEN           :1;
    unsigned long EN                :1;
    unsigned long WL                :4;
    unsigned long CLR               :1;
    unsigned long NEINTEN           :1;
    unsigned long Reserved1         :20;
};
typedef union
{
    struct _ADCx_FIFOCFG_bits bit;
    unsigned long all;
} _ADCx_FIFOCFG;
/***************************************************************************************************
 *      ADCx_FIFOSTS - ADC FIFO Status Register
***************************************************************************************************/
struct _ADCx_FIFOSTS_bits
{
    unsigned long REFLAG            :1;
    unsigned long WEFLAG            :1;
    unsigned long FFLAG             :1;
    unsigned long EFLAG             :1;
    unsigned long HFFLAG            :1;
    unsigned long DATCNT            :5;
    unsigned long Reserved1         :1;
    unsigned long NEFLAG            :1;
    unsigned long Reserved2         :20;
};
typedef union
{
    struct _ADCx_FIFOSTS_bits bit;
    unsigned long all;
} _ADCx_FIFOSTS;
/***************************************************************************************************
 *      ADCx_PCTRL - ADC Power Control Register
***************************************************************************************************/
struct _ADCx_PCTRL_bits
{
    unsigned long Reserved1         :3;
    unsigned long VREFLDOEN         :1;
    unsigned long RANGESEL          :1;
    unsigned long EXPEN             :1;
    unsigned long Reserved2         :26;
};
typedef union
{
    struct _ADCx_PCTRL_bits bit;
    unsigned long all;
} _ADCx_PCTRL;
/***************************************************************************************************
 *      ADCx_CTRL4 - ADC Control Register 4
***************************************************************************************************/
struct _ADCx_CTRL4_bits
{
    unsigned long Reserved1         :16;
    unsigned long EXTRRSEL          :4;
    unsigned long EXTRISEL          :4;
    unsigned long Reserved2         :8;
};
typedef union
{
    struct _ADCx_CTRL4_bits bit;
    unsigned long all;
} _ADCx_CTRL4;
/***************************************************************************************************
 *      ADCx_INTEN - ADC Interrupt Enable Register
***************************************************************************************************/
struct _ADCx_INTEN_bits
{
    unsigned long ENDCIEN           :1;
    unsigned long ENDCAIEN          :1;
    unsigned long JENDCIEN          :1;
    unsigned long JENDCAIEN         :1;
    unsigned long AWD1IEN           :1;
    unsigned long ENDCERRIEN        :1;
    unsigned long RDYIEN            :1;
    unsigned long PDRDYIEN          :1;
    unsigned long EOSMPIEN          :1;
    unsigned long Reserved1         :23;
};
typedef union
{
    struct _ADCx_INTEN_bits bit;
    unsigned long all;
} _ADCx_INTEN;
/***************************************************************************************************
 *      ADCx_GCOMP - ADC Gain Compensation Register
***************************************************************************************************/
struct _ADCx_GCOMP_bits
{
    unsigned long GCOMPDAT          :14;
    unsigned long Reserved1         :18;
};
typedef union
{
    struct _ADCx_GCOMP_bits bit;
    unsigned long all;
} _ADCx_GCOMP;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _ADCx_STS _ADC1_STS @(ADC1_BASE + 0x0000);
extern volatile _ADCx_CTRL1 _ADC1_CTRL1 @(ADC1_BASE + 0x0004);
extern volatile _ADCx_CTRL2 _ADC1_CTRL2 @(ADC1_BASE + 0x0008);
extern volatile _ADCx_CTRL3 _ADC1_CTRL3 @(ADC1_BASE + 0x000C);
extern volatile _ADCx_SAMPT1 _ADC1_SAMPT1 @(ADC1_BASE + 0x0010);
extern volatile _ADCx_SAMPT2 _ADC1_SAMPT2 @(ADC1_BASE + 0x0014);
extern volatile _ADCx_SAMPT3 _ADC1_SAMPT3 @(ADC1_BASE + 0x0018);
extern volatile _ADCx_DIFSEL _ADC1_DIFSEL @(ADC1_BASE + 0x001C);
extern volatile _ADCx_CALFACT _ADC1_CALFACT @(ADC1_BASE + 0x0020);
extern volatile _ADCx_OFFSET1 _ADC1_OFFSET1 @(ADC1_BASE + 0x0024);
extern volatile _ADCx_OFFSET2 _ADC1_OFFSET2 @(ADC1_BASE + 0x0028);
extern volatile _ADCx_OFFSET3 _ADC1_OFFSET3 @(ADC1_BASE + 0x002C);
extern volatile _ADCx_OFFSET4 _ADC1_OFFSET4 @(ADC1_BASE + 0x0030);
extern volatile _ADCx_AWD1HIGH _ADC1_AWD1HIGH @(ADC1_BASE + 0x0034);
extern volatile _ADCx_AWD1LOW _ADC1_AWD1LOW @(ADC1_BASE + 0x0038);
extern volatile _ADCx_AWD2HIGH _ADC1_AWD2HIGH @(ADC1_BASE + 0x003C);
extern volatile _ADCx_AWD2LOW _ADC1_AWD2LOW @(ADC1_BASE + 0x0040);
extern volatile _ADCx_AWD3HIGH _ADC1_AWD3HIGH @(ADC1_BASE + 0x0044);
extern volatile _ADCx_AWD3LOW _ADC1_AWD3LOW @(ADC1_BASE + 0x0048);
extern volatile _ADCx_AWD2EN _ADC1_AWD2EN @(ADC1_BASE + 0x004C);
extern volatile _ADCx_AWD3EN _ADC1_AWD3EN @(ADC1_BASE + 0x0050);
extern volatile _ADCx_AWD2INTEN _ADC1_AWD2INTEN @(ADC1_BASE + 0x0054);
extern volatile _ADCx_AWD3INTEN _ADC1_AWD3INTEN @(ADC1_BASE + 0x0058);
extern volatile _ADCx_AWD2STS _ADC1_AWD2STS @(ADC1_BASE + 0x005C);
extern volatile _ADCx_AWD3STS _ADC1_AWD3STS @(ADC1_BASE + 0x0060);
extern volatile _ADCx_RSEQ1 _ADC1_RSEQ1 @(ADC1_BASE + 0x0064);
extern volatile _ADCx_RSEQ2 _ADC1_RSEQ2 @(ADC1_BASE + 0x0068);
extern volatile _ADCx_RSEQ3 _ADC1_RSEQ3 @(ADC1_BASE + 0x006C);
extern volatile _ADCx_JSEQ _ADC1_JSEQ @(ADC1_BASE + 0x0070);
extern volatile _ADCx_JDAT1 _ADC1_JDAT1 @(ADC1_BASE + 0x0074);
extern volatile _ADCx_JDAT2 _ADC1_JDAT2 @(ADC1_BASE + 0x0078);
extern volatile _ADCx_JDAT3 _ADC1_JDAT3 @(ADC1_BASE + 0x007C);
extern volatile _ADCx_JDAT4 _ADC1_JDAT4 @(ADC1_BASE + 0x0080);
extern volatile _ADCx_DAT _ADC1_DAT @(ADC1_BASE + 0x0084);
extern volatile _ADCx_FIFOCFG _ADC1_FIFOCFG @(ADC1_BASE + 0x0088);
extern volatile _ADCx_FIFOSTS _ADC1_FIFOSTS @(ADC1_BASE + 0x008C);
extern volatile _ADCx_PCTRL _ADC1_PCTRL @(ADC1_BASE + 0x0090);
extern volatile _ADCx_CTRL4 _ADC1_CTRL4 @(ADC1_BASE + 0x0094);
extern volatile _ADCx_INTEN _ADC1_INTEN @(ADC1_BASE + 0x0098);
extern volatile _ADCx_GCOMP _ADC1_GCOMP @(ADC1_BASE + 0x009C);

extern volatile _ADCx_STS _ADC2_STS @(ADC2_BASE + 0x0000);
extern volatile _ADCx_CTRL1 _ADC2_CTRL1 @(ADC2_BASE + 0x0004);
extern volatile _ADCx_CTRL2 _ADC2_CTRL2 @(ADC2_BASE + 0x0008);
extern volatile _ADCx_CTRL3 _ADC2_CTRL3 @(ADC2_BASE + 0x000C);
extern volatile _ADCx_SAMPT1 _ADC2_SAMPT1 @(ADC2_BASE + 0x0010);
extern volatile _ADCx_SAMPT2 _ADC2_SAMPT2 @(ADC2_BASE + 0x0014);
extern volatile _ADCx_SAMPT3 _ADC2_SAMPT3 @(ADC2_BASE + 0x0018);
extern volatile _ADCx_DIFSEL _ADC2_DIFSEL @(ADC2_BASE + 0x001C);
extern volatile _ADCx_CALFACT _ADC2_CALFACT @(ADC2_BASE + 0x0020);
extern volatile _ADCx_OFFSET1 _ADC2_OFFSET1 @(ADC2_BASE + 0x0024);
extern volatile _ADCx_OFFSET2 _ADC2_OFFSET2 @(ADC2_BASE + 0x0028);
extern volatile _ADCx_OFFSET3 _ADC2_OFFSET3 @(ADC2_BASE + 0x002C);
extern volatile _ADCx_OFFSET4 _ADC2_OFFSET4 @(ADC2_BASE + 0x0030);
extern volatile _ADCx_AWD1HIGH _ADC2_AWD1HIGH @(ADC2_BASE + 0x0034);
extern volatile _ADCx_AWD1LOW _ADC2_AWD1LOW @(ADC2_BASE + 0x0038);
extern volatile _ADCx_AWD2HIGH _ADC2_AWD2HIGH @(ADC2_BASE + 0x003C);
extern volatile _ADCx_AWD2LOW _ADC2_AWD2LOW @(ADC2_BASE + 0x0040);
extern volatile _ADCx_AWD3HIGH _ADC2_AWD3HIGH @(ADC2_BASE + 0x0044);
extern volatile _ADCx_AWD3LOW _ADC2_AWD3LOW @(ADC2_BASE + 0x0048);
extern volatile _ADCx_AWD2EN _ADC2_AWD2EN @(ADC2_BASE + 0x004C);
extern volatile _ADCx_AWD3EN _ADC2_AWD3EN @(ADC2_BASE + 0x0050);
extern volatile _ADCx_AWD2INTEN _ADC2_AWD2INTEN @(ADC2_BASE + 0x0054);
extern volatile _ADCx_AWD3INTEN _ADC2_AWD3INTEN @(ADC2_BASE + 0x0058);
extern volatile _ADCx_AWD2STS _ADC2_AWD2STS @(ADC2_BASE + 0x005C);
extern volatile _ADCx_AWD3STS _ADC2_AWD3STS @(ADC2_BASE + 0x0060);
extern volatile _ADCx_RSEQ1 _ADC2_RSEQ1 @(ADC2_BASE + 0x0064);
extern volatile _ADCx_RSEQ2 _ADC2_RSEQ2 @(ADC2_BASE + 0x0068);
extern volatile _ADCx_RSEQ3 _ADC2_RSEQ3 @(ADC2_BASE + 0x006C);
extern volatile _ADCx_JSEQ _ADC2_JSEQ @(ADC2_BASE + 0x0070);
extern volatile _ADCx_JDAT1 _ADC2_JDAT1 @(ADC2_BASE + 0x0074);
extern volatile _ADCx_JDAT2 _ADC2_JDAT2 @(ADC2_BASE + 0x0078);
extern volatile _ADCx_JDAT3 _ADC2_JDAT3 @(ADC2_BASE + 0x007C);
extern volatile _ADCx_JDAT4 _ADC2_JDAT4 @(ADC2_BASE + 0x0080);
extern volatile _ADCx_DAT _ADC2_DAT @(ADC2_BASE + 0x0084);
extern volatile _ADCx_FIFOCFG _ADC2_FIFOCFG @(ADC2_BASE + 0x0088);
extern volatile _ADCx_FIFOSTS _ADC2_FIFOSTS @(ADC2_BASE + 0x008C);
extern volatile _ADCx_PCTRL _ADC2_PCTRL @(ADC2_BASE + 0x0090);
extern volatile _ADCx_CTRL4 _ADC2_CTRL4 @(ADC2_BASE + 0x0094);
extern volatile _ADCx_INTEN _ADC2_INTEN @(ADC2_BASE + 0x0098);
extern volatile _ADCx_GCOMP _ADC2_GCOMP @(ADC2_BASE + 0x009C);

extern volatile _ADCx_STS _ADC3_STS @(ADC3_BASE + 0x0000);
extern volatile _ADCx_CTRL1 _ADC3_CTRL1 @(ADC3_BASE + 0x0004);
extern volatile _ADCx_CTRL2 _ADC3_CTRL2 @(ADC3_BASE + 0x0008);
extern volatile _ADCx_CTRL3 _ADC3_CTRL3 @(ADC3_BASE + 0x000C);
extern volatile _ADCx_SAMPT1 _ADC3_SAMPT1 @(ADC3_BASE + 0x0010);
extern volatile _ADCx_SAMPT2 _ADC3_SAMPT2 @(ADC3_BASE + 0x0014);
extern volatile _ADCx_SAMPT3 _ADC3_SAMPT3 @(ADC3_BASE + 0x0018);
extern volatile _ADCx_DIFSEL _ADC3_DIFSEL @(ADC3_BASE + 0x001C);
extern volatile _ADCx_CALFACT _ADC3_CALFACT @(ADC3_BASE + 0x0020);
extern volatile _ADCx_OFFSET1 _ADC3_OFFSET1 @(ADC3_BASE + 0x0024);
extern volatile _ADCx_OFFSET2 _ADC3_OFFSET2 @(ADC3_BASE + 0x0028);
extern volatile _ADCx_OFFSET3 _ADC3_OFFSET3 @(ADC3_BASE + 0x002C);
extern volatile _ADCx_OFFSET4 _ADC3_OFFSET4 @(ADC3_BASE + 0x0030);
extern volatile _ADCx_AWD1HIGH _ADC3_AWD1HIGH @(ADC3_BASE + 0x0034);
extern volatile _ADCx_AWD1LOW _ADC3_AWD1LOW @(ADC3_BASE + 0x0038);
extern volatile _ADCx_AWD2HIGH _ADC3_AWD2HIGH @(ADC3_BASE + 0x003C);
extern volatile _ADCx_AWD2LOW _ADC3_AWD2LOW @(ADC3_BASE + 0x0040);
extern volatile _ADCx_AWD3HIGH _ADC3_AWD3HIGH @(ADC3_BASE + 0x0044);
extern volatile _ADCx_AWD3LOW _ADC3_AWD3LOW @(ADC3_BASE + 0x0048);
extern volatile _ADCx_AWD2EN _ADC3_AWD2EN @(ADC3_BASE + 0x004C);
extern volatile _ADCx_AWD3EN _ADC3_AWD3EN @(ADC3_BASE + 0x0050);
extern volatile _ADCx_AWD2INTEN _ADC3_AWD2INTEN @(ADC3_BASE + 0x0054);
extern volatile _ADCx_AWD3INTEN _ADC3_AWD3INTEN @(ADC3_BASE + 0x0058);
extern volatile _ADCx_AWD2STS _ADC3_AWD2STS @(ADC3_BASE + 0x005C);
extern volatile _ADCx_AWD3STS _ADC3_AWD3STS @(ADC3_BASE + 0x0060);
extern volatile _ADCx_RSEQ1 _ADC3_RSEQ1 @(ADC3_BASE + 0x0064);
extern volatile _ADCx_RSEQ2 _ADC3_RSEQ2 @(ADC3_BASE + 0x0068);
extern volatile _ADCx_RSEQ3 _ADC3_RSEQ3 @(ADC3_BASE + 0x006C);
extern volatile _ADCx_JSEQ _ADC3_JSEQ @(ADC3_BASE + 0x0070);
extern volatile _ADCx_JDAT1 _ADC3_JDAT1 @(ADC3_BASE + 0x0074);
extern volatile _ADCx_JDAT2 _ADC3_JDAT2 @(ADC3_BASE + 0x0078);
extern volatile _ADCx_JDAT3 _ADC3_JDAT3 @(ADC3_BASE + 0x007C);
extern volatile _ADCx_JDAT4 _ADC3_JDAT4 @(ADC3_BASE + 0x0080);
extern volatile _ADCx_DAT _ADC3_DAT @(ADC3_BASE + 0x0084);
extern volatile _ADCx_FIFOCFG _ADC3_FIFOCFG @(ADC3_BASE + 0x0088);
extern volatile _ADCx_FIFOSTS _ADC3_FIFOSTS @(ADC3_BASE + 0x008C);
extern volatile _ADCx_PCTRL _ADC3_PCTRL @(ADC3_BASE + 0x0090);
extern volatile _ADCx_CTRL4 _ADC3_CTRL4 @(ADC3_BASE + 0x0094);
extern volatile _ADCx_INTEN _ADC3_INTEN @(ADC3_BASE + 0x0098);
extern volatile _ADCx_GCOMP _ADC3_GCOMP @(ADC3_BASE + 0x009C);

extern volatile _ADCx_STS _ADC4_STS @(ADC4_BASE + 0x0000);
extern volatile _ADCx_CTRL1 _ADC4_CTRL1 @(ADC4_BASE + 0x0004);
extern volatile _ADCx_CTRL2 _ADC4_CTRL2 @(ADC4_BASE + 0x0008);
extern volatile _ADCx_CTRL3 _ADC4_CTRL3 @(ADC4_BASE + 0x000C);
extern volatile _ADCx_SAMPT1 _ADC4_SAMPT1 @(ADC4_BASE + 0x0010);
extern volatile _ADCx_SAMPT2 _ADC4_SAMPT2 @(ADC4_BASE + 0x0014);
extern volatile _ADCx_SAMPT3 _ADC4_SAMPT3 @(ADC4_BASE + 0x0018);
extern volatile _ADCx_DIFSEL _ADC4_DIFSEL @(ADC4_BASE + 0x001C);
extern volatile _ADCx_CALFACT _ADC4_CALFACT @(ADC4_BASE + 0x0020);
extern volatile _ADCx_OFFSET1 _ADC4_OFFSET1 @(ADC4_BASE + 0x0024);
extern volatile _ADCx_OFFSET2 _ADC4_OFFSET2 @(ADC4_BASE + 0x0028);
extern volatile _ADCx_OFFSET3 _ADC4_OFFSET3 @(ADC4_BASE + 0x002C);
extern volatile _ADCx_OFFSET4 _ADC4_OFFSET4 @(ADC4_BASE + 0x0030);
extern volatile _ADCx_AWD1HIGH _ADC4_AWD1HIGH @(ADC4_BASE + 0x0034);
extern volatile _ADCx_AWD1LOW _ADC4_AWD1LOW @(ADC4_BASE + 0x0038);
extern volatile _ADCx_AWD2HIGH _ADC4_AWD2HIGH @(ADC4_BASE + 0x003C);
extern volatile _ADCx_AWD2LOW _ADC4_AWD2LOW @(ADC4_BASE + 0x0040);
extern volatile _ADCx_AWD3HIGH _ADC4_AWD3HIGH @(ADC4_BASE + 0x0044);
extern volatile _ADCx_AWD3LOW _ADC4_AWD3LOW @(ADC4_BASE + 0x0048);
extern volatile _ADCx_AWD2EN _ADC4_AWD2EN @(ADC4_BASE + 0x004C);
extern volatile _ADCx_AWD3EN _ADC4_AWD3EN @(ADC4_BASE + 0x0050);
extern volatile _ADCx_AWD2INTEN _ADC4_AWD2INTEN @(ADC4_BASE + 0x0054);
extern volatile _ADCx_AWD3INTEN _ADC4_AWD3INTEN @(ADC4_BASE + 0x0058);
extern volatile _ADCx_AWD2STS _ADC4_AWD2STS @(ADC4_BASE + 0x005C);
extern volatile _ADCx_AWD3STS _ADC4_AWD3STS @(ADC4_BASE + 0x0060);
extern volatile _ADCx_RSEQ1 _ADC4_RSEQ1 @(ADC4_BASE + 0x0064);
extern volatile _ADCx_RSEQ2 _ADC4_RSEQ2 @(ADC4_BASE + 0x0068);
extern volatile _ADCx_RSEQ3 _ADC4_RSEQ3 @(ADC4_BASE + 0x006C);
extern volatile _ADCx_JSEQ _ADC4_JSEQ @(ADC4_BASE + 0x0070);
extern volatile _ADCx_JDAT1 _ADC4_JDAT1 @(ADC4_BASE + 0x0074);
extern volatile _ADCx_JDAT2 _ADC4_JDAT2 @(ADC4_BASE + 0x0078);
extern volatile _ADCx_JDAT3 _ADC4_JDAT3 @(ADC4_BASE + 0x007C);
extern volatile _ADCx_JDAT4 _ADC4_JDAT4 @(ADC4_BASE + 0x0080);
extern volatile _ADCx_DAT _ADC4_DAT @(ADC4_BASE + 0x0084);
extern volatile _ADCx_FIFOCFG _ADC4_FIFOCFG @(ADC4_BASE + 0x0088);
extern volatile _ADCx_FIFOSTS _ADC4_FIFOSTS @(ADC4_BASE + 0x008C);
extern volatile _ADCx_PCTRL _ADC4_PCTRL @(ADC4_BASE + 0x0090);
extern volatile _ADCx_CTRL4 _ADC4_CTRL4 @(ADC4_BASE + 0x0094);
extern volatile _ADCx_INTEN _ADC4_INTEN @(ADC4_BASE + 0x0098);
extern volatile _ADCx_GCOMP _ADC4_GCOMP @(ADC4_BASE + 0x009C);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_ADC_H__ */
