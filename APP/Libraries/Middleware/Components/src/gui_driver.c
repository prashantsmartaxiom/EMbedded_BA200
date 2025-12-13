/***********************************************************************************************************************
File Name: gui_driver.c
External Data:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

External Functions:
    Name                                    Source
    --------------------------------------- ------------------------------------------------------------------------

Description:
    BA100 GUI control driver.
    The driver shall create and fill in the required data in the driver's parameters before executing any function.
========================================================================================================================
History:
    Date        Author          Description of Change
    ----------- --------------- ------------------------------------------------------------------------------------
    01/20/2025  Fred Huang      Create file

***********************************************************************************************************************/
//------------------------------------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------------------------------------
#include "gui_driver.h"
#include "boot_process.h"
//------------------------------------------------------------------------------------------------------------
// Local function prototypes
//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
// Data definitions
//------------------------------------------------------------------------------------------------------------
GuiRegisterMap guiRegister;// = {0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5};
u16 mfrKey;
u8 mfrMode, mfrLedResult, mfrRs485Result;

Create_Gui_Slave_Command(guiCommand, 63, 256) =
{
    //            Code,       Command Type,  Data Type,                        DataPointer, DataSize
    {      LED1_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led1Status, 1}, /*list 0*/
    {     LED1_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led1Control, 1}, /*list 1*/
    {      LED2_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led2Status, 1}, /*list 2*/
    {     LED2_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led2Control, 1}, /*list 3*/
    {      LED3_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led3Status, 1}, /*list 4*/
    {     LED3_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led3Control, 1}, /*list 5*/
    {      LED4_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led4Status, 1}, /*list 6*/
    {     LED4_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led4Control, 1}, /*list 7*/
    {      LED5_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led5Status, 1}, /*list 8*/
    {     LED5_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led5Control, 1}, /*list 9*/
    {      LED6_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led6Status, 1}, /*list 10*/
    {     LED6_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led6Control, 1}, /*list 11*/
    {      LED7_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led7Status, 1}, /*list 12*/
    {     LED7_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led7Control, 1}, /*list 13*/
    {      LED8_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led8Status, 1}, /*list 14*/
    {    LED8_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led8Control, 1}, /*list 15*/
    {    LED9_STATUS,     GUI_READ_WRITE,        GUI_LED,       (u8*)&guiRegister.led9Status, 1}, /*list 16*/
    {    LED9_CONTROL,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led9Control, 1}, /*list 17*/
    {    LED10_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led10Status, 1}, /*list 18*/
    {    LED10_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led10Control, 1}, /*list 19*/
    {    LED11_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led11Status, 1}, /*list 20*/
    {    LED11_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led11Control, 1}, /*list 21*/
    {    LED12_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led12Status, 1}, /*list 22*/
    {    LED12_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led12Control, 1}, /*list 23*/
    {    LED13_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led13Status, 1}, /*list 24*/
    {    LED13_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led13Control, 1}, /*list 25*/
    {    LED14_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led14Status,  1}, /*list 26*/
    {    LED14_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led14Control, 1}, /*list 27*/
    {    LED15_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led15Status,  1}, /*list 28*/
    {    LED15_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led15Control, 1}, /*list 29*/
    {    LED16_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led16Status,  1}, /*list 30*/
    {    LED16_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led16Control, 1}, /*list 31*/
    {    LED17_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led17Status,  1}, /*list 32*/
    {    LED17_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led17Control, 1}, /*list 33*/
    {    LED18_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led18Status,  1}, /*list 34*/
    {    LED18_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led18Control, 1}, /*list 35*/
    {    LED19_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led19Status,  1}, /*list 36*/
    {    LED19_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led19Control, 1}, /*list 37*/
    {    LED20_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led20Status,  1}, /*list 38*/
    {    LED20_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led20Control, 1}, /*list 39*/
    {    LED21_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led21Status,  1}, /*list 40*/
    {    LED21_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led21Control, 1}, /*list 41*/
    {    LED22_STATUS,     GUI_READ_WRITE,        GUI_LED,      (u8*)&guiRegister.led22Status,  1}, /*list 42*/
    {    LED22_CONTROL,     GUI_READ_WRITE,        GUI_LED,     (u8*)&guiRegister.led22Control, 1}, /*list 43*/
    {        SHADE1_ID,     GUI_READ_WRITE,      GUI_SHADE,         (u8*)&guiRegister.shade1Id, 1}, /*list 44*/
    {   SHADE1_CONTROL,     GUI_READ_WRITE,      GUI_SHADE,    (u8*)&guiRegister.shade1Control, 1}, /*list 45*/
    {        SHADE2_ID,     GUI_READ_WRITE,      GUI_SHADE,         (u8*)&guiRegister.shade2Id, 1}, /*list 46*/
    {   SHADE2_CONTROL,     GUI_READ_WRITE,      GUI_SHADE,    (u8*)&guiRegister.shade2Control, 1}, /*list 47*/
    {        SHADE3_ID,     GUI_READ_WRITE,      GUI_SHADE,         (u8*)&guiRegister.shade3Id, 1}, /*list 48*/
    {   SHADE3_CONTROL,     GUI_READ_WRITE,      GUI_SHADE,    (u8*)&guiRegister.shade3Control, 1}, /*list 49*/
    {        SHADE4_ID,     GUI_READ_WRITE,      GUI_SHADE,         (u8*)&guiRegister.shade4Id, 1}, /*list 50*/
    {   SHADE4_CONTROL,     GUI_READ_WRITE,      GUI_SHADE,    (u8*)&guiRegister.shade4Control, 1}, /*list 51*/
    {      PIR1_STATUS,     GUI_READ_WRITE,        GUI_PIR,       (u8*)&guiRegister.pir1Status, 1}, /*list 52*/
    {      PIR2_STATUS,     GUI_READ_WRITE,        GUI_PIR,       (u8*)&guiRegister.pir2Status, 1}, /*list 53*/
    {      PIR3_STATUS,     GUI_READ_WRITE,        GUI_PIR,       (u8*)&guiRegister.pir3Status, 1}, /*list 54*/
    {      PIR4_STATUS,     GUI_READ_WRITE,        GUI_PIR,       (u8*)&guiRegister.pir4Status, 1}, /*list 55*/
    // Bootloader
    {  BOOT_PROTECTION,     GUI_READ_WRITE,     BOOTLOADER,     (u16*)&bootInternal.protectKey, 2},
    {        BOOT_INIT,     GUI_READ_WRITE,     BOOTLOADER,        (u16*)&bootInternal.initKey, 2},
    {       BOOT_STATE,     GUI_READ_WRITE,     BOOTLOADER,          (u16*)&bootInternal.state, 2},
    // Manufacture
    {       MFR_ENABLE,     GUI_WRITE_ONLY,    MANUFACTURE,                      (u16*)&mfrKey, 2},
    {         MFR_MODE,     GUI_READ_WRITE,    MANUFACTURE,                      (u8*)&mfrMode, 1},
    {  MFR_LED_ALL_MAX,     GUI_READ_WRITE,    MANUFACTURE,                 (u8*)&mfrLedResult, 1},
    {   MFR_RS485_TEST,     GUI_READ_WRITE,    MANUFACTURE,               (u8*)&mfrRs485Result, 1},
};
//------------------------------------------------------------------------------------------------------------
// Local functions
//------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------
// Global functions
//------------------------------------------------------------------------------------------------------------
/**************************************************************************************************
Function Name:
    void Gui_Command_List_Setup(GuiCommandArray *command)
Input:
    *command - Pointer of GUI command list
Output:
    None.
Comment:
    This function will initialize master GUI command list and command address.
**************************************************************************************************/
void Gui_Command_List_Setup(GuiCommandArray *command)
{
    u16 i;
    // Fill all indexes with 0
    for (i = 0; i < command->indexLength; i++)
    {
        command->index[i] = 0;
    }
    // Point the corresponding command code in the index array to the list array
    for (i = 0; i < command->listLength; i++)
    {
        if (command->list[i].cmdCode < command->indexLength)
        {
            command->index[command->list[i].cmdCode] =
                (command->list[i].cmdType > 0) ? (GuiCommandInfo*)&command->list[i] : 0;
        }
    }
}
/**************************************************************************************************
Function Name:
    slaveCommPack Gui_Communication_Process(I2cSlaveObj *obj)
Input:
    *obj - Pointer of I2C slave object.
    *queueRegPtr - Registers pointer of priority circular buffer.
Output:
    Received data's command index.
Comment:
    Slave I2C communication process.
**************************************************************************************************/
slaveCommPack Gui_Communication_Process(I2cSlaveObj *obj)
{
    slaveCommPack guiControl = {GUI_WRITE, NOT_SUPPORT_CMD};
    u16 timeOutCount = 0;

    // Check if recieved data
    if (obj->func.receive(obj, &guiControl.cmdCode, 1, I2C_NORMAL_RCV))
    {
        if ((*(guiCommand.index + guiControl.cmdCode))->cmdType == GUI_NOT_SUPPORT)
            return guiControl;

        while (obj->member.reg.STS2->bit.BUSY)
        {
            if (obj->member.reg.STS2->bit.TRF)
            {
                obj->func.transmit(obj, (*(guiCommand.index + guiControl.cmdCode))->dataPtr,
                                    (*(guiCommand.index + guiControl.cmdCode))->dataSize);
                guiControl.direction = GUI_READ;
            }
            else if (timeOutCount++ > 1000)
            {
//                obj->member.reg.DAT->all = 0;
//                obj->member.reg.CTRL1->bit.EN = 1;
                break;
            }
        }
        if (guiControl.direction == GUI_WRITE)
        {
            obj->func.receive(obj, (*(guiCommand.index + guiControl.cmdCode))->dataPtr,
                                (*(guiCommand.index + guiControl.cmdCode))->dataSize, I2C_DATA_ONLY);
        }
    }

    return guiControl;
}

//----------------------------------------------------------------------------------------------------------------------
