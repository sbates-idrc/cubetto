#include <SPI.h>
#include <CubettoLibrary.h>
#include <AccelStepper.h>
#include <aci.h>
#include <KickstarterBackers.h>

Cubetto           CubettoRobot; 
/** ************************************************************************************************************************************
 * @function   
 * @brief     
 ***************************************************************************************************************************************/
void setup() {

  SPI.begin(); 
  SPI.setBitOrder(LSBFIRST); 
  SPI.setDataMode(SPI_MODE0); 
  SPI.setClockDivider(SPI_CLOCK_DIV16); 

  CubettoRobot.begin();                                // Setup Robot
  CubettoRobot.startAdvertisingBluetooth(0, true);     // Start Advertising and Auto
}


/** ************************************************************************************************************************************
 * @function   
 * @brief     
 ***************************************************************************************************************************************/
void loop() {
    CubettoRobot.bluetoothTasks(); 
    if (CubettoRobot.checkForBluetoothMessage()==true){
        CubettoRobot.decodeInterfaceInstructions (CubettoRobot.bluetoothMessageCommand); 
    }

    // Tasks to call every 1 second
    oneSecondLoop (); 

}

/** ************************************************************************************************************************************
 * @function   
 * @brief     
 ***************************************************************************************************************************************/
void oneSecondLoop (void){
  static uint32_t counter = 0;
  counter++; 
  if (counter>0xFFFF){
   counter=0;
    CubettoRobot.updateBatteryLevel(); 
    CubettoRobot.sendBatteryLevelToBluetooth(); 
  }
}







