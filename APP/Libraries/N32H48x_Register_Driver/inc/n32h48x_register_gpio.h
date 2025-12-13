/***********************************************************************************************************************
File Name: n32h48x_register_gpio.h
Global Data:
    Name                 Type               Description
    -------------------- ------------------ ------------------------------------------------------------------------

Description:
    Header file of N32H48x MCU GPIO register definitions.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    12/12/2024  Fred Huang      Ver 1.0

***********************************************************************************************************************/
#ifndef __N32H48x_REG_GPIO_H__
#define __N32H48x_REG_GPIO_H__

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
 *      GPIOx_PMODE - GPIO port mode configuration register
***************************************************************************************************/
struct _GPIOx_PMODE_bits
{
    unsigned long PMODE0        :2;
    unsigned long PMODE1        :2;
    unsigned long PMODE2        :2;
    unsigned long PMODE3        :2;
    unsigned long PMODE4        :2;
    unsigned long PMODE5        :2;
    unsigned long PMODE6        :2;
    unsigned long PMODE7        :2;
    unsigned long PMODE8        :2;
    unsigned long PMODE9        :2;
    unsigned long PMODE10       :2;
    unsigned long PMODE11       :2;
    unsigned long PMODE12       :2;
    unsigned long PMODE13       :2;
    unsigned long PMODE14       :2;
    unsigned long PMODE15       :2;
};
typedef union
{
    struct _GPIOx_PMODE_bits bit;
    unsigned long all;
} _GPIOx_PMODE;
/***************************************************************************************************
 *      GPIOx_PUPD - GPIO port pull up/down configuration register
***************************************************************************************************/
struct _GPIOx_PUPD_bits
{
    unsigned long PUPD0         :2;
    unsigned long PUPD1         :2;
    unsigned long PUPD2         :2;
    unsigned long PUPD3         :2;
    unsigned long PUPD4         :2;
    unsigned long PUPD5         :2;
    unsigned long PUPD6         :2;
    unsigned long PUPD7         :2;
    unsigned long PUPD8         :2;
    unsigned long PUPD9         :2;
    unsigned long PUPD10        :2;
    unsigned long PUPD11        :2;
    unsigned long PUPD12        :2;
    unsigned long PUPD13        :2;
    unsigned long PUPD14        :2;
    unsigned long PUPD15        :2;
};
typedef union
{
    struct _GPIOx_PUPD_bits bit;
    unsigned long all;
} _GPIOx_PUPD;
/***************************************************************************************************
 *      GPIOx_POTYPE - GPIO port output type configuration register
***************************************************************************************************/
struct _GPIOx_POTYPE_bits
{
    unsigned long POT0          :1;
    unsigned long POT1          :1;
    unsigned long POT2          :1;
    unsigned long POT3          :1;
    unsigned long POT4          :1;
    unsigned long POT5          :1;
    unsigned long POT6          :1;
    unsigned long POT7          :1;
    unsigned long POT8          :1;
    unsigned long POT9          :1;
    unsigned long POT10         :1;
    unsigned long POT11         :1;
    unsigned long POT12         :1;
    unsigned long POT13         :1;
    unsigned long POT14         :1;
    unsigned long POT15         :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _GPIOx_POTYPE_bits bit;
    unsigned long all;
} _GPIOx_POTYPE;
/***************************************************************************************************
 *      GPIOx_AFSEL0 - GPIO port alternate function selection configuration 0 register
***************************************************************************************************/
struct _GPIOx_AFSEL0_bits
{
    unsigned long AFSEL0        :5;
    unsigned long Reserved1     :3;
    unsigned long AFSEL1        :5;
    unsigned long Reserved2     :3;
    unsigned long AFSEL2        :5;
    unsigned long Reserved3     :3;
    unsigned long AFSEL3        :5;
    unsigned long Reserved4     :3;
};
typedef union
{
    struct _GPIOx_AFSEL0_bits bit;
    unsigned long all;
} _GPIOx_AFSEL0;
/***************************************************************************************************
 *      GPIOx_AFSEL1 - GPIO port alternate function selection configuration 1 register
***************************************************************************************************/
struct _GPIOx_AFSEL1_bits
{
    unsigned long AFSEL4        :5;
    unsigned long Reserved1     :3;
    unsigned long AFSEL5        :5;
    unsigned long Reserved2     :3;
    unsigned long AFSEL6        :5;
    unsigned long Reserved3     :3;
    unsigned long AFSEL7        :5;
    unsigned long Reserved4     :3;
};
typedef union
{
    struct _GPIOx_AFSEL1_bits bit;
    unsigned long all;
} _GPIOx_AFSEL1;
/***************************************************************************************************
 *      GPIOx_AFSEL2 - GPIO port alternate function selection configuration 2 register
***************************************************************************************************/
struct _GPIOx_AFSEL2_bits
{
    unsigned long AFSEL8        :5;
    unsigned long Reserved1     :3;
    unsigned long AFSEL9        :5;
    unsigned long Reserved2     :3;
    unsigned long AFSEL10       :5;
    unsigned long Reserved3     :3;
    unsigned long AFSEL11       :5;
    unsigned long Reserved4     :3;
};
typedef union
{
    struct _GPIOx_AFSEL2_bits bit;
    unsigned long all;
} _GPIOx_AFSEL2;
/***************************************************************************************************
 *      GPIOx_AFSEL3 - GPIO port alternate function selection configuration 3 register
***************************************************************************************************/
struct _GPIOx_AFSEL3_bits
{
    unsigned long AFSEL12       :5;
    unsigned long Reserved1     :3;
    unsigned long AFSEL13       :5;
    unsigned long Reserved2     :3;
    unsigned long AFSEL14       :5;
    unsigned long Reserved3     :3;
    unsigned long AFSEL15       :5;
    unsigned long Reserved4     :3;
};
typedef union
{
    struct _GPIOx_AFSEL3_bits bit;
    unsigned long all;
} _GPIOx_AFSEL3;
/***************************************************************************************************
 *      GPIOx_DS - GPIO driver source configuration register
***************************************************************************************************/
struct _GPIOx_DS_bits
{
    unsigned long DS0           :2;
    unsigned long DS1           :2;
    unsigned long DS2           :2;
    unsigned long DS3           :2;
    unsigned long DS4           :2;
    unsigned long DS5           :2;
    unsigned long DS6           :2;
    unsigned long DS7           :2;
    unsigned long DS8           :2;
    unsigned long DS9           :2;
    unsigned long DS10          :2;
    unsigned long DS11          :2;
    unsigned long DS12          :2;
    unsigned long DS13          :2;
    unsigned long DS14          :2;
    unsigned long DS15          :2;
};
typedef union
{
    struct _GPIOx_DS_bits bit;
    unsigned long all;
} _GPIOx_DS;
/***************************************************************************************************
 *      GPIOx_SR - GPIO slew rate configuration register
***************************************************************************************************/
struct _GPIOx_SR_bits
{
    unsigned long SR0           :1;
    unsigned long SR1           :1;
    unsigned long SR2           :1;
    unsigned long SR3           :1;
    unsigned long SR4           :1;
    unsigned long SR5           :1;
    unsigned long SR6           :1;
    unsigned long SR7           :1;
    unsigned long SR8           :1;
    unsigned long SR9           :1;
    unsigned long SR10          :1;
    unsigned long SR11          :1;
    unsigned long SR12          :1;
    unsigned long SR13          :1;
    unsigned long SR14          :1;
    unsigned long SR15          :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _GPIOx_SR_bits bit;
    unsigned long all;
} _GPIOx_SR;
/***************************************************************************************************
 *      GPIOx_PBSC - GPIO port bit set/reset register
***************************************************************************************************/
struct _GPIOx_PBSC_bits
{
    unsigned long PBS0          :1;
    unsigned long PBS1          :1;
    unsigned long PBS2          :1;
    unsigned long PBS3          :1;
    unsigned long PBS4          :1;
    unsigned long PBS5          :1;
    unsigned long PBS6          :1;
    unsigned long PBS7          :1;
    unsigned long PBS8          :1;
    unsigned long PBS9          :1;
    unsigned long PBS10         :1;
    unsigned long PBS11         :1;
    unsigned long PBS12         :1;
    unsigned long PBS13         :1;
    unsigned long PBS14         :1;
    unsigned long PBS15         :1;
    unsigned long PBC0          :1;
    unsigned long PBC1          :1;
    unsigned long PBC2          :1;
    unsigned long PBC3          :1;
    unsigned long PBC4          :1;
    unsigned long PBC5          :1;
    unsigned long PBC6          :1;
    unsigned long PBC7          :1;
    unsigned long PBC8          :1;
    unsigned long PBC9          :1;
    unsigned long PBC10         :1;
    unsigned long PBC11         :1;
    unsigned long PBC12         :1;
    unsigned long PBC13         :1;
    unsigned long PBC14         :1;
    unsigned long PBC15         :1;
};
typedef union
{
    struct _GPIOx_PBSC_bits bit;
    unsigned long all;
} _GPIOx_PBSC;
/***************************************************************************************************
 *      GPIOx_PBC - GPIO port bit clear register
***************************************************************************************************/
struct _GPIOx_PBC_bits
{
    unsigned long PBC0          :1;
    unsigned long PBC1          :1;
    unsigned long PBC2          :1;
    unsigned long PBC3          :1;
    unsigned long PBC4          :1;
    unsigned long PBC5          :1;
    unsigned long PBC6          :1;
    unsigned long PBC7          :1;
    unsigned long PBC8          :1;
    unsigned long PBC9          :1;
    unsigned long PBC10         :1;
    unsigned long PBC11         :1;
    unsigned long PBC12         :1;
    unsigned long PBC13         :1;
    unsigned long PBC14         :1;
    unsigned long PBC15         :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _GPIOx_PBC_bits bit;
    unsigned long all;
} _GPIOx_PBC;
/***************************************************************************************************
 *      GPIOx_PID - GPIO port input data register
***************************************************************************************************/
struct _GPIOx_PID_bits
{
    unsigned long PID0          :1;
    unsigned long PID1          :1;
    unsigned long PID2          :1;
    unsigned long PID3          :1;
    unsigned long PID4          :1;
    unsigned long PID5          :1;
    unsigned long PID6          :1;
    unsigned long PID7          :1;
    unsigned long PID8          :1;
    unsigned long PID9          :1;
    unsigned long PID10         :1;
    unsigned long PID11         :1;
    unsigned long PID12         :1;
    unsigned long PID13         :1;
    unsigned long PID14         :1;
    unsigned long PID15         :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _GPIOx_PID_bits bit;
    unsigned long all;
} _GPIOx_PID;
/***************************************************************************************************
 *      GPIOx_POD - GPIO port output data register
***************************************************************************************************/
struct _GPIOx_POD_bits
{
    unsigned long POD0          :1;
    unsigned long POD1          :1;
    unsigned long POD2          :1;
    unsigned long POD3          :1;
    unsigned long POD4          :1;
    unsigned long POD5          :1;
    unsigned long POD6          :1;
    unsigned long POD7          :1;
    unsigned long POD8          :1;
    unsigned long POD9          :1;
    unsigned long POD10         :1;
    unsigned long POD11         :1;
    unsigned long POD12         :1;
    unsigned long POD13         :1;
    unsigned long POD14         :1;
    unsigned long POD15         :1;
    unsigned long Reserved1     :16;
};
typedef union
{
    struct _GPIOx_POD_bits bit;
    unsigned long all;
} _GPIOx_POD;
/***************************************************************************************************
 *      GPIOx_PLOCK - GPIO port lock configuration register
***************************************************************************************************/
struct _GPIOx_PLOCK_bits
{
    unsigned long PLOCK0        :1;
    unsigned long PLOCK1        :1;
    unsigned long PLOCK2        :1;
    unsigned long PLOCK3        :1;
    unsigned long PLOCK4        :1;
    unsigned long PLOCK5        :1;
    unsigned long PLOCK6        :1;
    unsigned long PLOCK7        :1;
    unsigned long PLOCK8        :1;
    unsigned long PLOCK9        :1;
    unsigned long PLOCK10       :1;
    unsigned long PLOCK11       :1;
    unsigned long PLOCK12       :1;
    unsigned long PLOCK13       :1;
    unsigned long PLOCK14       :1;
    unsigned long PLOCK15       :1;
    unsigned long PLOCKK        :1;
    unsigned long Reserved1     :15;
};
typedef union
{
    struct _GPIOx_PLOCK_bits bit;
    unsigned long all;
} _GPIOx_PLOCK;

//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------
extern volatile _GPIOx_PMODE _GPIOA_PMODE @(GPIOA_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOA_PUPD @(GPIOA_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOA_POTYPE @(GPIOA_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOA_AFSEL0 @(GPIOA_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOA_AFSEL1 @(GPIOA_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOA_AFSEL2 @(GPIOA_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOA_AFSEL3 @(GPIOA_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOA_DS @(GPIOA_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOA_SR @(GPIOA_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOA_PBSC @(GPIOA_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOA_PBC @(GPIOA_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOA_PID @(GPIOA_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOA_POD @(GPIOA_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOA_PLOCK @(GPIOA_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOB_PMODE @(GPIOB_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOB_PUPD @(GPIOB_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOB_POTYPE @(GPIOB_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOB_AFSEL0 @(GPIOB_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOB_AFSEL1 @(GPIOB_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOB_AFSEL2 @(GPIOB_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOB_AFSEL3 @(GPIOB_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOB_DS @(GPIOB_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOB_SR @(GPIOB_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOB_PBSC @(GPIOB_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOB_PBC @(GPIOB_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOB_PID @(GPIOB_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOB_POD @(GPIOB_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOB_PLOCK @(GPIOB_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOC_PMODE @(GPIOC_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOC_PUPD @(GPIOC_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOC_POTYPE @(GPIOC_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOC_AFSEL0 @(GPIOC_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOC_AFSEL1 @(GPIOC_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOC_AFSEL2 @(GPIOC_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOC_AFSEL3 @(GPIOC_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOC_DS @(GPIOC_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOC_SR @(GPIOC_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOC_PBSC @(GPIOC_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOC_PBC @(GPIOC_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOC_PID @(GPIOC_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOC_POD @(GPIOC_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOC_PLOCK @(GPIOC_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOD_PMODE @(GPIOD_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOD_PUPD @(GPIOD_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOD_POTYPE @(GPIOD_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOD_AFSEL0 @(GPIOD_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOD_AFSEL1 @(GPIOD_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOD_AFSEL2 @(GPIOD_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOD_AFSEL3 @(GPIOD_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOD_DS @(GPIOD_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOD_SR @(GPIOD_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOD_PBSC @(GPIOD_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOD_PBC @(GPIOD_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOD_PID @(GPIOD_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOD_POD @(GPIOD_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOD_PLOCK @(GPIOD_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOE_PMODE @(GPIOE_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOE_PUPD @(GPIOE_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOE_POTYPE @(GPIOE_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOE_AFSEL0 @(GPIOE_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOE_AFSEL1 @(GPIOE_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOE_AFSEL2 @(GPIOE_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOE_AFSEL3 @(GPIOE_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOE_DS @(GPIOE_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOE_SR @(GPIOE_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOE_PBSC @(GPIOE_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOE_PBC @(GPIOE_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOE_PID @(GPIOE_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOE_POD @(GPIOE_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOE_PLOCK @(GPIOE_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOF_PMODE @(GPIOF_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOF_PUPD @(GPIOF_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOF_POTYPE @(GPIOF_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOF_AFSEL0 @(GPIOF_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOF_AFSEL1 @(GPIOF_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOF_AFSEL2 @(GPIOF_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOF_AFSEL3 @(GPIOF_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOF_DS @(GPIOF_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOF_SR @(GPIOF_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOF_PBSC @(GPIOF_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOF_PBC @(GPIOF_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOF_PID @(GPIOF_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOF_POD @(GPIOF_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOF_PLOCK @(GPIOF_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOG_PMODE @(GPIOG_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOG_PUPD @(GPIOG_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOG_POTYPE @(GPIOG_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOG_AFSEL0 @(GPIOG_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOG_AFSEL1 @(GPIOG_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOG_AFSEL2 @(GPIOG_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOG_AFSEL3 @(GPIOG_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOG_DS @(GPIOG_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOG_SR @(GPIOG_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOG_PBSC @(GPIOG_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOG_PBC @(GPIOG_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOG_PID @(GPIOG_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOG_POD @(GPIOG_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOG_PLOCK @(GPIOG_BASE + 0x0034);

extern volatile _GPIOx_PMODE _GPIOH_PMODE @(GPIOH_BASE + 0x0000);
extern volatile _GPIOx_PUPD _GPIOH_PUPD @(GPIOH_BASE + 0x0004);
extern volatile _GPIOx_POTYPE _GPIOH_POTYPE @(GPIOH_BASE + 0x0008);
extern volatile _GPIOx_AFSEL0 _GPIOH_AFSEL0 @(GPIOH_BASE + 0x000C);
extern volatile _GPIOx_AFSEL1 _GPIOH_AFSEL1 @(GPIOH_BASE + 0x0010);
extern volatile _GPIOx_AFSEL2 _GPIOH_AFSEL2 @(GPIOH_BASE + 0x0014);
extern volatile _GPIOx_AFSEL3 _GPIOH_AFSEL3 @(GPIOH_BASE + 0x0018);
extern volatile _GPIOx_DS _GPIOH_DS @(GPIOH_BASE + 0x001C);
extern volatile _GPIOx_SR _GPIOH_SR @(GPIOH_BASE + 0x0020);
extern volatile _GPIOx_PBSC _GPIOH_PBSC @(GPIOH_BASE + 0x0024);
extern volatile _GPIOx_PBC _GPIOH_PBC @(GPIOH_BASE + 0x0028);
extern volatile _GPIOx_PID _GPIOH_PID @(GPIOH_BASE + 0x002C);
extern volatile _GPIOx_POD _GPIOH_POD @(GPIOH_BASE + 0x0030);
extern volatile _GPIOx_PLOCK _GPIOH_PLOCK @(GPIOH_BASE + 0x0034);

//----------------------------------------------------------------------------------------------------------------------
#endif  /* __N32H48x_REG_GPIO_H__ */
