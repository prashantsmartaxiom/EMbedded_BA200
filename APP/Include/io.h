/***********************************************************************************************************************
File Name: io.h
Global Data:
    NameTypeDescription
    ------------------- ------------------- ------------------------------------------------------------------------

Description:

========================================================================================================================
History:
    DateAuthor  Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/06/2025  Fred Huang      Create file

***********************************************************************************************************************/
#ifndef _IO_H_
#define _IO_H_

//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Macro definitions
//------------------------------------------------------------------------------------------------------------
#define PIN(p, n)                   p, n

#define _IN_PIN(p, n)               _GPIO##p##_PID.bit.PID##n
#define IN_PIN(pin)                 _IN_PIN(pin)

#define _OUT_PIN(p, n)              _GPIO##p##_POD.bit.POD##n
#define OUT_PIN(pin)                _OUT_PIN(pin)

#define _SET_PIN(p, n)              (_GPIO##p##_PBSC.bit.PBS##n = 1)
#define SET_PIN(pin)                _SET_PIN(pin)

#define _CLEAR_PIN(p,n)             (_GPIO##p##_PBC.bit.PBC##n = 1)
#define CLEAR_PIN(pin)              _CLEAR_PIN(pin)

#define _TOGGLE_PIN(p,n)            (_GPIO##p##_POD.bit.POD##n ^= 1)
#define TOGGLE_PIN(pin)             _TOGGLE_PIN(pin)

// Pin defination
// Input

// Output
#define LED_GREEN                   PIN(D, 15)
#define DEVICE_ENABLE               PIN(G, 0)
#define LED_RED                     PIN(G, 2)

#define USART1_DE                   PIN(H, 5)
#define USART2_DE                   PIN(G, 15)
#define USART3_DE                   PIN(B, 7)
#define USART4_DE                   PIN(E, 4)
#define UART5_DE                    PIN(D, 6)
#define UART6_DE                    PIN(D, 2)
#define UART7_DE                    PIN(G, 6)
#define UART8_DE                    PIN(G, 3)

#define DEVICE_PORT_1               PIN(A, 12)
#define DEVICE_PORT_2               PIN(H, 2)
#define DEVICE_PORT_3               PIN(A, 15)
#define DEVICE_PORT_4               PIN(C, 10)
#define DEVICE_PORT_5               PIN(C, 11)
#define DEVICE_PORT_6               PIN(C, 12)
#define DEVICE_PORT_7               PIN(D, 7)
#define DEVICE_PORT_8               PIN(G, 10)
#define DEVICE_PORT_9               PIN(G, 11)
#define DEVICE_PORT_10              PIN(G, 12)
#define DEVICE_PORT_11              PIN(G, 13)
#define DEVICE_PORT_12              PIN(F, 1)
#define DEVICE_PORT_13              PIN(F, 0)
#define DEVICE_PORT_14              PIN(C, 14)
#define DEVICE_PORT_15              PIN(C, 13)
#define DEVICE_PORT_16              PIN(E, 6)
#define DEVICE_PORT_17              PIN(E, 5)
#define DEVICE_PORT_18              PIN(B, 10)
#define DEVICE_PORT_19              PIN(G, 1)
#define DEVICE_PORT_20              PIN(F, 13)
#define DEVICE_PORT_21              PIN(F, 12)
#define DEVICE_PORT_22              PIN(F, 11)

// Macros for App layer
#define LED_GREEN_ON                CLEAR_PIN(LED_GREEN)
#define LED_GREEN_OFF               SET_PIN(LED_GREEN)
#define LED_RED_ON                  CLEAR_PIN(LED_RED)
#define LED_RED_OFF                 SET_PIN(LED_RED)

#define ALL_DEVICE_ENABLE           SET_PIN(DEVICE_ENABLE)
#define ALL_DEVICE_DISABLE          CLEAR_PIN(DEVICE_ENABLE)

#define USART1_DE_ENABLE            SET_PIN(USART1_DE)
#define USART1_DE_DISABLE           CLEAR_PIN(USART1_DE)
#define USART2_DE_ENABLE            SET_PIN(USART2_DE)
#define USART2_DE_DISABLE           CLEAR_PIN(USART2_DE)
#define USART3_DE_ENABLE            SET_PIN(USART3_DE)
#define USART3_DE_DISABLE           CLEAR_PIN(USART3_DE)
#define USART4_DE_ENABLE            SET_PIN(USART4_DE)
#define USART4_DE_DISABLE           CLEAR_PIN(USART4_DE)
#define UART5_DE_ENABLE             SET_PIN(UART5_DE)
#define UART5_DE_DISABLE            CLEAR_PIN(UART5_DE)
#define UART6_DE_ENABLE             SET_PIN(UART6_DE)
#define UART6_DE_DISABLE            CLEAR_PIN(UART6_DE)
#define UART7_DE_ENABLE             SET_PIN(UART7_DE)
#define UART7_DE_DISABLE            CLEAR_PIN(UART7_DE)
#define UART8_DE_ENABLE             SET_PIN(UART8_DE)
#define UART8_DE_DISABLE            CLEAR_PIN(UART8_DE)

#define Chip_Select(n)              CLEAR_PIN(DEVICE_PORT_##n)
#define Disable_Chip_Select(n)      SET_PIN(DEVICE_PORT_##n)
#define Toggle_Chip_Select(n)       TOGGLE_PIN(DEVICE_PORT_##n)

//------------------------------------------------------------------------------------------------------------
// Type definitions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global function prototypes
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global data declarations
//------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
#endif  /* _IO_H_ */
