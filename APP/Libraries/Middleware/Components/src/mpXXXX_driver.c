/***********************************************************************************************************************
File Name: mpXXXX_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    08/06/2025  Wilson Chen      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "mpXXXX_driver.h"

//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
Mp5920RegisterMap Mp5920Register;
Mp8880aRegisterMap Mp8880aRegister;
Tpt29555aRegisterMap Tpt29555aRegister;

Create_I2C_Master_Command(I2cMasterCommand, 34, 256) =
{
    //                 Code,     Command Type,                                 DataPointer,  DataSize
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,     (u16*)&Mp5920Register.Shade_01_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,     (u16*)&Mp5920Register.Shade_02_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,     (u16*)&Mp5920Register.Shade_03_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,     (u16*)&Mp5920Register.Shade_04_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_01_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_02_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_03_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_04_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_05_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_06_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_07_outputPower,         2},
    {  MP5920_CMD_READ_POUT,   I2C_WRITE_READ,   (u16*)&Mp5920Register.EXP_LED_08_outputPower,         2},
    
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_01_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_01_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_02_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_02_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_03_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_03_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_04_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,              (u16*)&Mp8880aRegister.IO_04_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,            (u16*)&Mp8880aRegister.FAN_CH1_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,            (u16*)&Mp8880aRegister.FAN_CH1_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,            (u16*)&Mp8880aRegister.FAN_CH2_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,            (u16*)&Mp8880aRegister.FAN_CH2_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_01_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_01_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_02_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_02_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_03_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_03_IOUT,         2},
    { MP8880A_CMD_READ_VOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_04_VOUT,         2},
    { MP8880A_CMD_READ_IOUT,   I2C_WRITE_READ,           (u16*)&Mp8880aRegister.RS485_04_IOUT,         2},
    
    { TPT29555A_CMD_INPUT_0,   I2C_WRITE_READ,             (u8*)&Tpt29555aRegister.InputPort0,         1},
    { TPT29555A_CMD_INPUT_1,   I2C_WRITE_READ,             (u8*)&Tpt29555aRegister.InputPort1,         1}
};
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Mp5920_Command_List_Setup(MP5920CommandArray *command)
Input:
    *command - Pointer of MP5920 command list
Output:
    None.
Comment:
    This function will initialize master MP5920 command list and command address.
**************************************************************************************************/
//void MpXXXX_Command_List_Setup(MpXXXXCommandArray *command)
//{
//    u16 i;
//    // Fill all indexes with 0
//    for (i = 0; i < command->indexLength; i++)
//    {
//        command->index[i] = 0;
//    }
//    // Point the corresponding command code in the index array to the list array
//    for (i = 0; i < command->listLength; i++)
//    {
//        if (command->list[i].cmdCode < command->indexLength)
//        {
//            command->index[command->list[i].cmdCode] =
//                (command->list[i].cmdType > 0) ? (MpXXXXCommandInfo*)&command->list[i] : 0;
//        }
//    }
//}
/**************************************************************************************************
Function Name:
    void Mp5920_Communication_Process(I2cMasterObj *obj)
Input:
    *obj - Pointer of I2C master object.
    *queueRegPtr - Registers pointer of priority circular buffer.
Output:
    Received data's command index.
Comment:
    Master I2C communication process.
**************************************************************************************************/
void MpXXXX_Communication_Process(I2cMasterObj *obj2, I2cMasterObj *obj3)
{
    static uint8_t list_cnt = 0, addr_cnt = 0;
    
    if (list_cnt < 4)
    {
        if (obj2->func.write(obj2, MP5920_ADDR_BASE + addr_cnt, I2cMasterCommand.list[list_cnt].cmdCode, 0, 0, I2C_WRITE_READ))
        {
            obj2->func.read(obj2, I2cMasterCommand.list[list_cnt].dataPtr, MP5920_ADDR_BASE + addr_cnt, I2cMasterCommand.list[list_cnt].dataSize);
        }
        
        addr_cnt = (addr_cnt + 1) % 4;
    }
    else if (list_cnt >= 4 && list_cnt < 12)
    {
        if (obj3->func.write(obj3, MP5920_ADDR_BASE + addr_cnt, I2cMasterCommand.list[list_cnt].cmdCode, 0, 0, I2C_WRITE_READ))
        {
            obj3->func.read(obj3, I2cMasterCommand.list[list_cnt].dataPtr, MP5920_ADDR_BASE + addr_cnt, I2cMasterCommand.list[list_cnt].dataSize);
        }
        
        addr_cnt = (addr_cnt + 1) % 8;
    }
    else if (list_cnt >= 12 && list_cnt < 24)
    {
        if (obj2->func.write(obj2, MP8880A_MFR_ADDRESS + addr_cnt, I2cMasterCommand.list[list_cnt].cmdCode, 0, 0, I2C_WRITE_READ))
        {
            obj2->func.read(obj2, I2cMasterCommand.list[list_cnt].dataPtr, MP8880A_MFR_ADDRESS + addr_cnt, I2cMasterCommand.list[list_cnt].dataSize);
        }
        
        addr_cnt = ((list_cnt - 11) / 2) % 6;
    }
    else
    {
        if (obj3->func.write(obj3, MP8880A_MFR_ADDRESS + addr_cnt, I2cMasterCommand.list[list_cnt].cmdCode, 0, 0, I2C_WRITE_READ))
        {
            obj3->func.read(obj3, I2cMasterCommand.list[list_cnt].dataPtr, MP8880A_MFR_ADDRESS + addr_cnt, I2cMasterCommand.list[list_cnt].dataSize);
        }
        
        addr_cnt = ((list_cnt - 23) / 2) % 4;
    }
    
    list_cnt = (list_cnt + 1) % 32;
        
//    static int i2ctest = 0, list_cnt = 12, addr_cnt = 0;
//    
//    if (i2ctest == 1)
//    {
//        if (obj2->func.write(obj2, MP8880A_MFR_ADDRESS, 0xD2, 0, 0, I2C_WRITE_READ))
//        {
//            obj2->func.read(obj2, I2cMasterCommand.list[0].dataPtr, MP8880A_MFR_ADDRESS, 1);
//        }
//        i2ctest = 0;
//    }
//    else if (i2ctest == 2)
//    {
//        obj2->func.write(obj2, MP8880A_MFR_ADDRESS, 0xD2, 1, 0x0B, I2C_WRITE_ONLY);
//        i2ctest = 0;
//    }
//    else if (i2ctest == 3)
//    {
//        obj2->func.write(obj2, MP8880A_MFR_ADDRESS, 0xD2, 1, 0x0C, I2C_WRITE_ONLY);
//        i2ctest = 0;
//    }
//    else if (i2ctest == 4)
//    {
//        if (list_cnt >= 12 && list_cnt < 18)
//        {
//            if (obj2->func.write(obj2, MP8880A_MFR_ADDRESS + addr_cnt, I2cMasterCommand.list[list_cnt].cmdCode, 0, 0, I2C_WRITE_READ))
//            {
//                obj2->func.read(obj2, I2cMasterCommand.list[list_cnt].dataPtr, MP8880A_MFR_ADDRESS + addr_cnt, I2cMasterCommand.list[list_cnt].dataSize);
//            }
//            
//            addr_cnt = (list_cnt == 17) ? 0 : ((list_cnt - 12) / 2) % 3;
//            if (list_cnt == 17)
//            {
//                i2ctest = 0;
//            }
//            list_cnt = (list_cnt == 17) ? 12 : (list_cnt + 1) % 18;
//        }
//    }
//    else if (i2ctest == 5)
//    {
//        if (obj2->func.write(obj2, MP8880A_MFR_ADDRESS, 0x21, 0, 0, I2C_WRITE_READ))
//        {
//            obj2->func.read(obj2, I2cMasterCommand.list[0].dataPtr, MP8880A_MFR_ADDRESS, 1);
//        }
//        i2ctest = 0;
//    }
//    else if (i2ctest == 6)
//    {
//        if (obj2->func.write(obj2, MP8880A_MFR_ADDRESS, 0x01, 0, 0, I2C_WRITE_READ))
//        {
//            obj2->func.read(obj2, I2cMasterCommand.list[0].dataPtr, MP8880A_MFR_ADDRESS, 1);
//        }
//        i2ctest = 0;
//    }
}
/**************************************************************************************************
Function Name:
    void IO_Expander_Communication_Process(I2cMasterObj *obj)
Input:
    *obj - Pointer of I2C master object.
    *queueRegPtr - Registers pointer of priority circular buffer.
Output:
    Received data's command index.
Comment:
    Master I2C communication process.
**************************************************************************************************/
void IO_Expander_Communication_Process(I2cMasterObj *obj)
{    
    if (obj->func.write(obj, TPT29555A_ADDRESS, I2cMasterCommand.list[32].cmdCode, 0, 0, I2C_WRITE_READ))
    {
        obj->func.read(obj, I2cMasterCommand.list[32].dataPtr, TPT29555A_ADDRESS, I2cMasterCommand.list[32].dataSize);
    }
    
    if (obj->func.write(obj, TPT29555A_ADDRESS, I2cMasterCommand.list[33].cmdCode, 0, 0, I2C_WRITE_READ))
    {
        obj->func.read(obj, I2cMasterCommand.list[33].dataPtr, TPT29555A_ADDRESS, I2cMasterCommand.list[33].dataSize);
    }
}
//----------------------------------------------------------------------------------------------------------------------
