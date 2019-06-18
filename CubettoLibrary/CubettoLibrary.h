/*********************************************************************************************************************************************************
  * @file    CubettoLibray.cpp
	*	@project Primo 
  * @author  Dominic Ewan Moffat
  * @version V1.0.0
  * @date    28/06/2016
  * @brief   Cubetto Libray Source for Ardunio 
/*********************************************************************************************************************************************************/
#ifndef CUBETTO_H
#define CUBETTO_H

#include "Arduino.h"
/*********************************************************************************************************************************************************
  * Cubetto IO Defines
/*********************************************************************************************************************************************************/
#define IO_STEP_A1  	6           						// PA20 6
#define IO_STEP_A2  	7          							// PA21 7
#define IO_STEP_A3  	20          						// PA22 20
#define IO_STEP_A4  	21          						// PA23 21
#define IO_STEP_B1  	27          						// PA28 27
#define IO_STEP_B2  	26          						// PA27 26
#define IO_STEP_B3  	31          						// PB23 31
#define IO_STEP_B4  	30          						// PB22 30
#define IO_SOUNDER		17											// PA04 17
#define IO_LED_YEL  	38        							// PA13 38
#define IO_LED_RED0 	35        							// PA16 35
#define	IO_LED_RED1   2        								// PA14 2
#define IO_DIG_1      4         							// PA08 4
#define IO_DIG_2      3         							// PA09 3
#define IO_DIG_3      1         							// PA10 1
#define IO_ANA_1      14        							// PA02 14
#define IO_ANA_2     	15        							// PB08 15
#define IO_ANA_3      16        							// PB09 16
#define IO_ANA_BATT   19        							// PB02 19
#define IO_BATT_GND   25        							// PB03 25
#define IO_BUT_1      12        							// PA19 12
#define IO_BUT_2      5         							// PA15 5
#define IO_NRF_RESET 	18        							// PA05 18
#define IO_NRF_REQN   41        							// PA07 41
#define IO_NRF_RDY    40         							// PA06 40
#define IO_LED_BLU  	0         							// PA11 0

/*********************************************************************************************************************************************************
  * Stepper Settings 
/*********************************************************************************************************************************************************/
#define STEPPER_MAX_SPEED     	850
#define STEPPER_ACCELERATION		500
#define STEPPER_FORWARD_STEPS 	2551
#define STEPPER_TURN_STEPS   	 	1120

#define CUBETTO_FORWARD     0x01
#define CUBETTO_BACKWARD    0x02
#define CUBETTO_LEFT        0x03
#define CUBETTO_RIGHT       0x04
#define CUBETTO_RANDOM      0x05

/*********************************************************************************************************************************************************
  * Bluetooth Status
/*********************************************************************************************************************************************************/
#define BLE_STATUS_OFF 0 
#define BLE_STATUS_ADVERTISING 1 
#define BLE_STATUS_CONNECTED 2
 
/*********************************************************************************************************************************************************
  * Bluetooth Messages
/*********************************************************************************************************************************************************/
#define CUBETTO_MESSAGE_MOVEMENT        0x01					
#define CUBETTO_MESSAGE_MOTOR           0x02					
#define CUBETTO_MESSAGE_IO_DIRECTION    0x03					
#define CUBETTO_MESSAGE_IO_WRITE        0x04
#define CUBETTO_MESSAGE_IO_READ         0x05
#define CUBETTO_MESSAGE_LED_WRITE       0x06
#define CUBETTO_MESSAGE_ANA_READ        0x07 
#define CUBETTO_MESSAGE_PLAY_NOTE       0x08
#define CUBETTO_MESSAGE_PLAY_TUNE       0x09

#define CUBETTO_TUNE_HAPPY 							0x01
#define CUBETTO_TUNE_SAD 								0x02
#define CUBETTO_TUNE_FORWARD 						0x03
#define CUBETTO_TUNE_BACKWARD 					0x04
#define CUBETTO_TUNE_LEFT								0x05
#define CUBETTO_TUNE_RIGHT 							0x06
#define CUBETTO_TUNE_PAIR 							0x07
#define CUBETTO_TUNE_UNPAIR 						0x08
#define CUBETTO_TUNE_POWER 							0x09
#define CUBETTO_TUNE_BEEP 							0x0A
#define CUBETTO_TUNE_TEST 							0xAA


/*********************************************************************************************************************************************************
  * Tone PWM Values
/*********************************************************************************************************************************************************/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


class Cubetto
{
  public:
	Cubetto(); 
	void begin();
	
	// Movement Methods
	void moveRight();
	void moveLeft(); 
	void moveForward(); 
	void moveBackward(); 
	void moveRandom();
	void moveSingle(uint8_t stepper, int16_t steps);
	void playPowerTone(); 
	void playHappyTone(); 
	void playSadTone(); 
	void playPairedTone();
	void playUnpairedTone(); 
	void playBeepTone(); 
	void playForwardTone();
	void playBackwardTone(); 
	void playLeftTone(); 	
	void playRightTone();
	void playTestTone(); 
	uint16_t getAnalog(uint8_t analog);
	bool getButton (uint8_t button);
	bool getDigital(uint8_t digital);
	void setDigital(uint8_t digital, uint8_t state);
	void setLED(uint8_t led, uint8_t state); 
	void updateBatteryLevel(); 
	void decodeInterfaceInstructions (uint8_t command); 
	uint8_t bluetoothConnected; 
	uint8_t	bluetoothAdvertising; 
	uint8_t bluetoothMessageCommand; 
	uint8_t bluetoothMessage[3]; 
	void bluetoothTasks(); 
	bool startAdvertisingBluetooth(uint8_t timeout, bool autoReadvertise); 
	bool stopAdvertisingBluetooth(); 
	bool disconnectBluetooth(); 
	bool checkForBluetoothMessage(); 
	void sendBatteryLevelToBluetooth(); 
	void enableIOStreaming(); 
	void disableIOStreaming(); 

	void replyToMovementMessage(bool status); 											
	void replyToMotorMessage (bool status);
	void replyToDigitalWriteMessage (bool status);
	void replyToLedWriteMessage (bool status);
	void replyToDigitalReadMessage (bool status, uint8_t state);
	void replyToAnalogReadMessage (bool status); 
	void replyToPlayNoteMessage (bool status); 
	void replyToPlayTuneMessage (bool status); 
	
	private:
	void move();
	uint16_t lookUpTone (uint8_t note);	
	void nrfSetup(); 
	void nrfLoop(); 
	void nrfOnWrite (uint8_t pipeNumber, uint8_t *data); 
	bool nrfWriteNotify (uint8_t txpipe, uint8_t setpipe, uint8_t* data, uint32_t size); 
	bool nrfWriteLocal (uint8_t pipe, uint8_t* data, uint32_t size);
	void nrfLEDUpdate(); 
	void ioUpdateStream(); 
};






#endif