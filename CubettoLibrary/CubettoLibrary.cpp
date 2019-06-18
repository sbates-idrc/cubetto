/*********************************************************************************************************************************************************
  * @file    CubettoLibray.cpp
	*	@project Primo 
  * @author  Dominic Ewan Moffat
  * @version V1.0.0
  * @date    28/06/2016
  * @brief   Cubetto Libray Source for Ardunio 
/*********************************************************************************************************************************************************/
/*********************************************************************************************************************************************************
 * @include   
/*********************************************************************************************************************************************************/
#include "Arduino.h"
#include "CubettoLibrary.h"
#include "AccelStepper.h"
#include <SPI.h>
#include "lib_aci.h"
#include "aci_setup.h"
#include "services.h"

/*********************************************************************************************************************************************************
 * @flavars
/*********************************************************************************************************************************************************/
const uint16_t toneTable[89] PROGMEM = {NOTE_B0,NOTE_C1,NOTE_CS1,NOTE_D1,NOTE_DS1,NOTE_E1,NOTE_F1,NOTE_FS1,NOTE_G1,NOTE_GS1,NOTE_A1,NOTE_AS1,NOTE_B1,NOTE_C2,NOTE_CS2,NOTE_D2,NOTE_DS2, NOTE_E2,NOTE_F2, NOTE_FS2,NOTE_G2,NOTE_GS2,NOTE_A2, NOTE_AS2, NOTE_B2,NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3, NOTE_F3, NOTE_FS3,NOTE_G3, NOTE_GS3,NOTE_A3, NOTE_AS3, NOTE_B3,  
NOTE_C4,  NOTE_CS4, NOTE_D4,  NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4,  NOTE_GS4,NOTE_A4, NOTE_AS4,NOTE_B4, NOTE_C5,NOTE_CS5,NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5,NOTE_GS5,NOTE_A5, NOTE_AS5,NOTE_B5, NOTE_C6,NOTE_CS6,NOTE_D6,NOTE_DS6, NOTE_E6,  NOTE_F6, NOTE_FS6, NOTE_G6,  NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6, NOTE_C7,  NOTE_CS7, 
NOTE_D7,  NOTE_DS7, NOTE_E7,NOTE_F7, NOTE_FS7,NOTE_G7,NOTE_GS7,NOTE_A7,NOTE_AS7,NOTE_B7,NOTE_C8,NOTE_CS8,NOTE_D8,NOTE_DS8
};

const uint8_t randomLookup [80] = {1,1,2,1,3,4,3,1,2,2,1,1,4,4,2,2,1,1,2,2,4,3,4,4,3,1,4,4,4,1,2,3,2,4,3,4,2,4,2,3,3,4,1,3,1,2,4,3,1,2,3,4,3,4,2,3,1,2,1,4,4,3,1,2,2,3,3,1,2,4,2,3,1,4,1,3,1,2,3,4};

/*********************************************************************************************************************************************************
 * @glovars
/*********************************************************************************************************************************************************/
#ifdef SERVICES_PIPE_TYPE_MAPPING_CONTENT
  static services_pipe_type_mapping_t
      services_pipe_type_mapping[NUMBER_OF_PIPES] = SERVICES_PIPE_TYPE_MAPPING_CONTENT;
#else
  #define NUMBER_OF_PIPES 0
  static services_pipe_type_mapping_t * services_pipe_type_mapping = NULL;
#endif


hal_aci_data_t setup_msgs[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;
aci_state_t aci_state;
hal_aci_evt_t aci_data;
hal_aci_data_t aci_cmd;
uint8_t 	uid[4]={0xDE, 0xAD, 0xBE, 0xEF}; 
int8_t 		HAL_IO_RADIO_RESET = IO_NRF_RESET; 
int8_t 		HAL_IO_RADIO_REQN = IO_NRF_REQN; 
int8_t 		HAL_IO_RADIO_RDY =  IO_NRF_RDY;	

// Internal Bluetooth Flags
bool nRFIsReady = false; 	
bool setLocalDataPending = false;
uint8_t bleLEDStatus = false; 
bool ioStreaming = false; 
bool bleMessage = false; 

// Public Bluetooth Flags
uint8_t bluetoothConnected=false; 
uint8_t	bluetoothAdvertising =false;
bool	autoReadvertiseAfterDisconnect = false; 
uint32_t	advertistingTimeout = 0;

uint8_t bluetoothMessageCommand = 0; 
uint8_t bluetoothMessageData[3]; 

// Public Cubetto Flags
uint8_t batteryVoltage = 0; 
uint8_t batteryLevel = 0; 

/*********************************************************************************************************************************************************
  * @instances
/*********************************************************************************************************************************************************/
AccelStepper left(AccelStepper::HALF4WIRE, IO_STEP_A4, IO_STEP_A2, IO_STEP_A3, IO_STEP_A1);
AccelStepper right(AccelStepper::HALF4WIRE, IO_STEP_B4, IO_STEP_B2, IO_STEP_B3, IO_STEP_B1);
/*********************************************************************************************************************************************************
 * @methods
/*********************************************************************************************************************************************************/
/*********************************************************************************************************************************************************
 * @public
/*********************************************************************************************************************************************************/
/*********************************************************************************************************************************************************
 * @method		Cubetto::Cubetto() 
 * @brief  		Constructor for class
 * @param			None
 * @return		Instance of Cubetto Class
/*********************************************************************************************************************************************************/
Cubetto::Cubetto() 
{
	// Initialise Bluetooth Variables
	// Reset internal flags			
	nRFIsReady = false; 	

	setLocalDataPending = false;
	bleLEDStatus = BLE_STATUS_OFF; 
	
	// Reset public flags
	bluetoothConnected=false; 
	bluetoothAdvertising=false;
	bluetoothMessageCommand = 0; 
	memset (bluetoothMessageData, 3, sizeof(bluetoothMessageData));

	

}
/*********************************************************************************************************************************************************
 * @method   	void Cubetto::begin()
 * @brief  		Initialises Cubetto 
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::begin()
{
	
	// Pin Directions
  pinMode (IO_LED_YEL, OUTPUT); 
  pinMode (IO_LED_RED0, OUTPUT); 
  pinMode (IO_LED_RED1, OUTPUT); 
  pinMode (IO_BATT_GND, INPUT); 
  pinMode (IO_DIG_1, OUTPUT);
  pinMode (IO_DIG_2, OUTPUT); 
  pinMode (IO_DIG_3, OUTPUT); 
  pinMode (IO_BUT_1, INPUT); 
  pinMode (IO_BUT_2, INPUT); 
	pinMode (IO_SOUNDER, OUTPUT); 
	pinMode (IO_LED_BLU, OUTPUT);
	pinMode (HAL_IO_RADIO_RESET, OUTPUT); 
	pinMode (HAL_IO_RADIO_REQN, OUTPUT); 
	pinMode (HAL_IO_RADIO_RDY, INPUT_PULLUP); 
	pinMode(IO_STEP_A1, OUTPUT);
  pinMode(IO_STEP_A2, OUTPUT);
  pinMode(IO_STEP_A3, OUTPUT);
  pinMode(IO_STEP_A4, OUTPUT);
  pinMode(IO_STEP_B1, OUTPUT);
  pinMode(IO_STEP_B2, OUTPUT);
  pinMode(IO_STEP_B3, OUTPUT);
  pinMode(IO_STEP_B4, OUTPUT);

  // Initial States
  digitalWrite (IO_LED_YEL, LOW); 
  digitalWrite (IO_LED_RED0, HIGH); 
  digitalWrite (IO_LED_RED1, HIGH); 
  digitalWrite (IO_DIG_1, LOW); 
  digitalWrite (IO_DIG_2, LOW); 
  digitalWrite (IO_DIG_3, LOW); 
	digitalWrite(HAL_IO_RADIO_REQN,  1);
	digitalWrite(HAL_IO_RADIO_RESET, 1);
	digitalWrite (IO_LED_BLU, LOW); 
	

	// Toggle Test
  delay(200);
  digitalWrite (IO_DIG_1, HIGH);
  delay(200);
  digitalWrite (IO_DIG_1, LOW);
	
	// Setup Left Stepper
	left.disableOutputs();
	left.setMaxSpeed(STEPPER_MAX_SPEED);
	left.setAcceleration(STEPPER_ACCELERATION);

	// Setup Right Stepper
  right.disableOutputs();
  right.setMaxSpeed(STEPPER_MAX_SPEED);
  right.setAcceleration(STEPPER_ACCELERATION);
	
	// Play Power Tone
	playPowerTone(); 

	// Start SPI forNRF8001
	SPI.begin(); 
  SPI.setBitOrder(LSBFIRST); 
  SPI.setDataMode(SPI_MODE0); 
  SPI.setClockDivider(SPI_CLOCK_DIV16); 

	// Setup NRF
	nrfSetup(); 


}	

/*********************************************************************************************************************************************************
 * @method   	uint16_t Cubetto::getAnalog(uint8_t analog)
 * @brief  		Reads the voltage from Cubetto's analog ports
 * @param			uint8_t analog. Analog pin of cubetto to sample
 * @return		Voltage in millivolts
/*********************************************************************************************************************************************************/
uint16_t Cubetto::getAnalog(uint8_t analog)
{
  switch (analog)
  {
    case 1: return analogRead(IO_ANA_1)*(3.2*2);  
    case 2: return analogRead(IO_ANA_2)*(3.2*2);    
    case 3: return analogRead(IO_ANA_3)*(3.2*2);
		default: return 0; 
  }
} 

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::setLED(uint8_t led, uint8_t state)
 * @brief  		Write to LED on Cubetto's board
 * @param			uint8_t led 				LED number to set
 * @param			uint8_t state 			State to set LED
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::setLED(uint8_t led, uint8_t state)
{
  switch (led)
  {
		case 1: 
			digitalWrite (IO_LED_BLU, state); 
			break; 
		case 2: 
			digitalWrite (IO_LED_YEL, state); 
			break; 
		case 3: 
			digitalWrite (IO_LED_RED0, state); 
			break; 
		case 4: 
			digitalWrite (IO_LED_RED1, state); 
			break; 
  }
} 

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::setDigital(uint8_t digital, uint8_t state)
 * @brief  		Write digital pin on Cubetto's board
 * @param			uint8_t digital: 		Digital pin to set
 * @param			uint8_t state  			State to write to digital pin
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::setDigital(uint8_t digital, uint8_t state)
{
  switch (digital)
  {
		case 1: 
			pinMode (IO_DIG_1, OUTPUT);
			digitalWrite (IO_DIG_1, state); 
			break; 
		case 2: 
			pinMode (IO_DIG_2, OUTPUT);
			digitalWrite (IO_DIG_2, state); 
			break; 
		case 3: 
			pinMode (IO_DIG_3, OUTPUT);
			digitalWrite (IO_DIG_3, state);  
			break; 
  }
} 

/*********************************************************************************************************************************************************
 * @method   	Cubetto::getDigital(uint8_t digital)
 * @brief  		Read digital pin on Cubetto's board
 * @param			uint8_t digital: 		Digital pin to read	
 * @return		State of digital pin
/*********************************************************************************************************************************************************/
bool Cubetto::getDigital(uint8_t digital)
{
  switch (digital)
  {
		case 1: 
			pinMode (IO_DIG_1, INPUT);
			return digitalRead (IO_DIG_1);  
		case 2: 
			pinMode (IO_DIG_2, INPUT);
			return digitalRead (IO_DIG_2);   
		case 3: 
			pinMode (IO_DIG_3, INPUT);
			return digitalRead (IO_DIG_3);    
  }
} 
/*********************************************************************************************************************************************************
 * @method   	bool Cubetto::getButton (uint8_t button)
 * @brief  		Read the state of user buttons on Cubetto
 * @param			uint8_t button: 		Button to read	
 * @return
/*********************************************************************************************************************************************************/
bool Cubetto::getButton (uint8_t button)
{
  switch (button)
  {
		case 1: 
			pinMode (IO_BUT_1, INPUT);
			return digitalRead (IO_BUT_1);  
		case 2: 
			pinMode (IO_BUT_2, INPUT);
			return digitalRead (IO_BUT_2);     
  }
} 

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::updateBatteryLevel ()
 * @brief  		Updates the global variables batteryVoltage and batteryLevel with new battery information
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::updateBatteryLevel ()
{

  uint16_t val; 
  pinMode (IO_BATT_GND, OUTPUT);  						// LoZ Input 
  digitalWrite(IO_BATT_GND, LOW);    					// Connect Output
  batteryVoltage = analogRead(IO_ANA_BATT);   // Read
  pinMode (IO_BATT_GND, INPUT);      					// HiZ Input 
	
  if (val >= 4000){								batteryLevel = 100;}
  else if (val > 3700){         	batteryLevel = 100 -((4000 - val) * 40) / 300;}
  else if (val > 3600){          	batteryLevel = 42 - ((3700 - val) * 40) / 100;}
  else if (val > 3300){          	batteryLevel = 18 - ((3600 - val) * 14) / 300;}
  else if (val > 3100){         	batteryLevel = 6 - ((3300 - val) * 6) / 200;}
  else { 												 	batteryLevel = 0;}

}
/*********************************************************************************************************************************************************
 * @method   	void Cubetto::decodeInterfaceInstructions (uint8_t command)
 * @brief  		Decodes the bluetooth message instructions from Primo Interface and performs the actions requested. Call if message is received on Bluetooth
 * @param			uint8_t command				Command message recieved on bluetooth
 * @return
/*********************************************************************************************************************************************************/
void Cubetto::decodeInterfaceInstructions (uint8_t command)
{
	switch (command) {
		// CUBETTO MOVEMENT COMMAND *************************************************************************************************
		case CUBETTO_MESSAGE_MOVEMENT:	
		{
			uint8_t Direction = bluetoothMessageData[0]; 
			if (Direction & 0x80){playForwardTone(); Direction &= 0x7F;} // Check if we are the instruction of a sequence
			// Move direction
			switch (Direction){
				case CUBETTO_FORWARD:
					moveForward(); 
					break; 
				case CUBETTO_BACKWARD:
					moveBackward();
					break; 	
				case CUBETTO_RIGHT:
					moveRight();
					break; 
				case CUBETTO_LEFT:
					moveLeft(); 
					break; 
				case CUBETTO_RANDOM:
					moveRandom(); 
					break; 		
			}
			replyToMovementMessage(true); 	
			break;
		}			
		// CUBETTO MOTOR COMMAND ****************************************************************************************************			
		case CUBETTO_MESSAGE_MOTOR:
		{
			uint8_t motor = bluetoothMessageData[0];
			int16_t steps = bluetoothMessageData[1]; 
			steps |= ((int16_t) bluetoothMessageData[2])<<8; 
			constrain(steps, -1000, 1000); 
			moveSingle(motor, steps);
			replyToMotorMessage (true);
			break; 
		}			
		// CUBETTO DIGITAL WRITE COMMAND ********************************************************************************************		
		case CUBETTO_MESSAGE_IO_WRITE:
		{
			uint8_t pin = bluetoothMessageData[0];
			uint8_t state = bluetoothMessageData[1]; 
			setDigital(pin, state);
			replyToDigitalWriteMessage (true);
			playHappyTone(); 
			break; 
		}
		// CUBETTO DIGITAL READ COMMAND *********************************************************************************************
		case CUBETTO_MESSAGE_IO_READ:
		{
			uint8_t pin = bluetoothMessageData[0];
			uint8_t state = getDigital(pin);
			replyToDigitalReadMessage (true, state);
			playHappyTone(); 
			break;
		}
		// CUBETTO LED WRITE COMMAND ************************************************************************************************
		case CUBETTO_MESSAGE_LED_WRITE:
		{
			uint8_t led = bluetoothMessageData[0];
			uint8_t state = bluetoothMessageData[1];
			setLED(led, state); 
			replyToLedWriteMessage (true);
			playHappyTone(); 
			break;
		}
		// CUBETTO IO STREAMING COMMAND *********************************************************************************************
		case CUBETTO_MESSAGE_ANA_READ:
		{
			uint8_t state = bluetoothMessageData[0]; 
			if (state==0){
				disableIOStreaming(); 
			}
			else {
				enableIOStreaming();
			}
			replyToAnalogReadMessage (true); 
			playHappyTone();
			break; 
		}
		// CUBETTO PLAY NOTE COMMAND ************************************************************************************************
		case CUBETTO_MESSAGE_PLAY_NOTE:
		{
			uint8_t note = bluetoothMessageData[0];
			uint8_t duration = bluetoothMessageData[1];
			duration |= ((uint16_t) bluetoothMessageData[2])<<8; 
			if (note == 0){noTone(IO_SOUNDER);}
			else {
					tone(IO_SOUNDER, lookUpTone(note-1), duration); 
			}
			replyToPlayNoteMessage (true); 
			break; 
		}
		// CUBETTO PLAY TUNE COMMAND ************************************************************************************************
		case CUBETTO_MESSAGE_PLAY_TUNE:
		{
			uint8_t tune = bluetoothMessageData[0];
			switch (tune){
				case CUBETTO_TUNE_HAPPY:
					playHappyTone(); 
					break; 
				case CUBETTO_TUNE_SAD:
					 playSadTone(); 
					break; 		
				case CUBETTO_TUNE_FORWARD:
					playForwardTone();
					break; 		
				case CUBETTO_TUNE_BACKWARD:
					playBackwardTone();
					break; 		
				case CUBETTO_TUNE_LEFT:
					playLeftTone(); 	
					break; 		
				case CUBETTO_TUNE_RIGHT:
					playRightTone();
					break; 		
				case CUBETTO_TUNE_PAIR:
					playPairedTone();
					break; 		
				case CUBETTO_TUNE_UNPAIR:
					playUnpairedTone(); 
					break; 		 		
				case CUBETTO_TUNE_POWER:
					playPowerTone(); 
					break; 		
				case CUBETTO_TUNE_BEEP:
					playBeepTone(); 
					break; 		
				case CUBETTO_TUNE_TEST:
					playTestTone(); 
					break; 						
			}
			replyToPlayTuneMessage (true); 
			break; 
		}
		default:
			break; 
	}
}



/*********************************************************************************************************************************************************
 * @method  	void Cubetto::moveRandom()
 * @brief  		Moves Cubetto randomly	
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::moveRandom()
{

		int seed = analogRead(42)*3;
		randomSeed(seed);
		uint8_t randomNumber = random(0, 80); 
		uint8_t randomMovement = randomLookup[randomNumber]; 

		switch (randomMovement){
			case 1:
				moveRight(); 
				break;
			case 2:
				moveLeft(); 
				break;
			case 3:
				moveForward(); 
				break;
			case 4:
				moveBackward(); 
				break;
		}
} 

/*********************************************************************************************************************************************************
 * @method   	Cubetto::moveSingle(uint8_t stepper, int16_t steps)
 * @brief  		Moves one of Cubetto's motors
 * @param			uint8_t stepper			Stepper to move, either 0 (Left) or 1 (Right)	
 * @param			uint16_t steps				Number of steps to move. 	
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::moveSingle(uint8_t stepper, int16_t steps)
{
	if (stepper > 1){
		playSadTone(); 
		return; 
	}
	
	playHappyTone(); 
	
  bool leftStepperIsRunning, rightStepperIsRunning;
  digitalWrite (IO_LED_YEL, HIGH);
	if (stepper == 0){
	  left.move(steps);
    left.enableOutputs();
		do {
      leftStepperIsRunning = left.run();
		} while (leftStepperIsRunning);
	}
	else {
		right.move(steps);
    right.enableOutputs();
		do {
      rightStepperIsRunning = right.run();
		} while (rightStepperIsRunning);
	}
	
	left.disableOutputs();
  right.disableOutputs(); 
	digitalWrite (IO_LED_YEL, LOW);
}


/*********************************************************************************************************************************************************
 * @method  	void Cubetto::moveRight()
 * @brief  		Move Cubetto right
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::moveRight() 
{
	left.move(STEPPER_TURN_STEPS);
	right.move(STEPPER_TURN_STEPS);
	move(); 
	playRightTone();
}

/*********************************************************************************************************************************************************
 * @method  	void Cubetto::moveLeft()
 * @brief  		Move Cubetto left
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::moveLeft() 
{
	left.move(-STEPPER_TURN_STEPS);
	right.move(-STEPPER_TURN_STEPS);
	move(); 
	playLeftTone(); 	
}

/*********************************************************************************************************************************************************
 * @method  	void Cubetto::moveForward()
 * @brief  		Move Cubetto forward
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::moveForward() 
{
	left.move(STEPPER_FORWARD_STEPS);
	right.move(-STEPPER_FORWARD_STEPS);
	move();
	playForwardTone(); 	
}

/*********************************************************************************************************************************************************
 * @method  	void Cubetto::moveBackward()
 * @brief  		Move Cubetto backwards
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::moveBackward() 
{
	left.move(-STEPPER_FORWARD_STEPS);
	right.move(STEPPER_FORWARD_STEPS);
	move();
	playBackwardTone(); 	
}


/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playPowerTone()
 * @brief  		Plays Power Tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playPowerTone()
{
  tone(IO_SOUNDER, NOTE_C6);
  delay(375);
  tone(IO_SOUNDER, NOTE_E6);
  delay(125);
  tone(IO_SOUNDER, NOTE_G6);
  delay(250);
  tone(IO_SOUNDER, NOTE_E6);
  delay(125);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playHappyTone ()
 * @brief  		Plays Happy tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playHappyTone ()
{
  tone(IO_SOUNDER, NOTE_C6);
  delay(150);
  tone(IO_SOUNDER, NOTE_E6);
  delay(150);
  tone(IO_SOUNDER, NOTE_G6);
  delay(100);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playSadTone()
 * @brief  		Plays Sad tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playSadTone()
{
  tone(IO_SOUNDER, NOTE_G5);
  delay(150);
  tone(IO_SOUNDER, NOTE_DS5);
  delay(150);
  tone(IO_SOUNDER, NOTE_C5);
  delay(300);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playPairedTone()
 * @brief  		Plays bluetooth paired Tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playPairedTone()
{
  tone(IO_SOUNDER, NOTE_D6);
  delay(150);
  tone(IO_SOUNDER, NOTE_F6);
  delay(150);
  tone(IO_SOUNDER, NOTE_DS5);
  delay(100);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playUnpairedTone()
 * @brief  		Plays bluetooth unpaired Tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playUnpairedTone()
{
  tone(IO_SOUNDER, NOTE_G3);
  delay(250);
  tone(IO_SOUNDER, NOTE_F3);
  delay(250);
  tone(IO_SOUNDER, NOTE_B2);
  delay(300);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playBeepTone()
 * @brief  		Plays beep tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playBeepTone ()
{
  tone(IO_SOUNDER, NOTE_G3);
  delay(250);
  tone(IO_SOUNDER, NOTE_F3);
  delay(250);
  tone(IO_SOUNDER, NOTE_B2);
  delay(300);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playForwardTone()
 * @brief  		Plays forward tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playForwardTone()
{
  tone(IO_SOUNDER, 3000);
  delay(150);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playBackwardTone()
 * @brief  		Plays backwards tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playBackwardTone()
{
  tone(IO_SOUNDER, 2000);
  delay(150);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playLeftTone()
 * @brief  		Plays left tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playLeftTone()
{
  tone(IO_SOUNDER, 5000);
  delay(50);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playRightTone()
 * @brief  		Plays left tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playRightTone()
{
  tone(IO_SOUNDER, 4000);
  delay(50);
  noTone(IO_SOUNDER);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::playTestTone()
 * @brief  		Plays test tone
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::playTestTone()
{
  tone(IO_SOUNDER, NOTE_C1);
  delay(300);
  tone(IO_SOUNDER, NOTE_C2);
  delay(300);
  tone(IO_SOUNDER, NOTE_C3);
  delay(300);
  tone(IO_SOUNDER, NOTE_C4);
  delay(300);
  tone(IO_SOUNDER, NOTE_C5);
  delay(300);
  tone(IO_SOUNDER, NOTE_C6);
  delay(300);
  tone(IO_SOUNDER, NOTE_C7);
  delay(300);
  tone(IO_SOUNDER, NOTE_C8);
  delay(300);
  noTone(IO_SOUNDER);
}


/*********************************************************************************************************************************************************
 * @method   	void Cubetto::bluetoothTasks()
 * @brief  		Handles bluetooth housekeeping tasks, and checks for incoming messages. Call this at least once in the loop (). 
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::bluetoothTasks()
{
	nrfLoop(); 					// Check for messages
	nrfLEDUpdate(); 		// Update LEDs
	ioUpdateStream(); 	// Process IO Stream (if enabled); 
}

/*********************************************************************************************************************************************************
 * @method   	bool Cubetto::startAdvertisingBluetooth(uint8_t timeout, bool autoReconnect)
 * @brief  		Starts advertising Cubetto on Bluetooth so a Central device can connect. 
 * @param			uint8_t timeout 				Time that Cubetto should advertise in ms. 0 causes cubetto to constantly advertise. 
 * @param			bool autoReconnect			Setting this causes Cubetto to automatically readvertise on Bluetooth after it is disconnected from a Central device
 * @return		TRUE: Started  FALSE: Error 
/*********************************************************************************************************************************************************/
bool Cubetto::startAdvertisingBluetooth(uint8_t timeout, bool autoReconnect)
{
		if (nRFIsReady==true){
      uint8_t status = lib_aci_connect(timeout, 0x0100);
			if (status==true){
				bleLEDStatus=BLE_STATUS_ADVERTISING;
				bluetoothAdvertising=true;  
				advertistingTimeout = timeout; 
				autoReadvertiseAfterDisconnect = autoReconnect; 
				return status; 
			}
		}
		return 0; 
}
/*********************************************************************************************************************************************************
 * @method   	bool Cubetto::stopAdvertisingBluetooth()
 * @brief  		Stops Cubetto advertising. 
 * @param			None
 * @return		TRUE: Stopped  FALSE: Error 
/*********************************************************************************************************************************************************/
bool Cubetto::stopAdvertisingBluetooth()
{
	
	// Reseting the nrf is the only way to stop advertising
	lib_aci_radio_reset();		

	// Reset internal flags			
	nRFIsReady = false; 	
	setLocalDataPending = false;
	bleLEDStatus = BLE_STATUS_OFF; 
	
	// Reset public flags
	bluetoothConnected=false; 
	bluetoothAdvertising=false;
	bluetoothMessageCommand = 0; 
	memset (bluetoothMessageData, 3, sizeof(bluetoothMessageData));

	// Setup NRF
	nrfSetup();
	
}

/*********************************************************************************************************************************************************
 * @method   	bool Cubetto::disconnectBluetooth()
 * @brief  		Causes Cubetto to disconnect from a Central device. 
 * @param			None
 * @return		TRUE: Disconnected  FALSE: Error 
/*********************************************************************************************************************************************************/
bool Cubetto::disconnectBluetooth()
{
	uint8_t status = 0; 
	if (bluetoothConnected==true){
		status = lib_aci_disconnect(&aci_state, ACI_REASON_TERMINATE);
		// Wait for disconnection event
		if (status == true){
			while (bluetoothConnected==true){
				nrfLoop(); 
			}	
		}
	}
	return status; 
}
/*********************************************************************************************************************************************************
 * @method   	bool Cubetto::checkForBluetoothMessage()
 * @brief  		Check for messages received through Cubetto.
 * @param			None
 * @return		TRUE: Message Waiting  FALSE: Empty
/*********************************************************************************************************************************************************/
bool Cubetto::checkForBluetoothMessage()
{
	nrfLoop(); 
	bool status = bleMessage; 
	bleMessage = false; 
	return status;		
}
/*********************************************************************************************************************************************************
 * @method   	void Cubetto::enableIOStreaming()
 * @brief  		Enable Cubetto to start streaming all IOs (Analog, Digitals, Buttons) over Bluetooth if connected.
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::enableIOStreaming()
{
	ioStreaming = true;
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::disableIOStreaming ()
 * @brief  		Disable Cubetto streaming all IOs (Analog, Digitals, Buttons) over Bluetooth.
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::disableIOStreaming ()
{
	ioStreaming = false;
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::sendBatteryLevelToBluetooth()
 * @brief  		Updates current battery levels over Bluetooth.
 * @param			None
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::sendBatteryLevelToBluetooth()
{
	static uint8_t prevBattery = 0; 	
	if (prevBattery!=batteryLevel){
			nrfWriteNotify(PIPE_BATTERY_BATTERY_LEVEL_TX, PIPE_BATTERY_BATTERY_LEVEL_SET, &batteryLevel, 1);
			prevBattery=batteryLevel; 
	}
}


/*********************************************************************************************************************************************************
 * @method   	void Cubetto::replyToMovementMessage(bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::replyToMovementMessage(bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_MOVEMENT; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}
/*********************************************************************************************************************************************************
 * @method    void Cubetto::replyToMotorMessage (bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::replyToMotorMessage (bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_MOTOR; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}
/*********************************************************************************************************************************************************
 * @method   	void Cubetto::replyToDigitalWriteMessage (bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::replyToDigitalWriteMessage (bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_IO_WRITE; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto::replyToLedWriteMessage (bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto::replyToLedWriteMessage (bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_LED_WRITE; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto:: replyToDigitalReadMessage (bool status, uint8_t state)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto:: replyToDigitalReadMessage (bool status, uint8_t state)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_IO_READ; 
	reply[1] = status; 
	reply[2] = state; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto:: replyToAnalogReadMessage (bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto:: replyToAnalogReadMessage (bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_ANA_READ; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto:: replyToPlayNoteMessage (bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto:: replyToPlayNoteMessage (bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_PLAY_NOTE; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}

/*********************************************************************************************************************************************************
 * @method   	void Cubetto:: replyToPlayTuneMessage (bool status)
 * @brief  		Reply to Central device after receiving a message. Reply with TRUE (Received OK) or FALSE (Received ERROR) 
 * @param			bool status 				TRUE: OK			FALSE:  ERROR
 * @return		None
/*********************************************************************************************************************************************************/
void Cubetto:: replyToPlayTuneMessage (bool status)
{
	uint8_t reply[4]; 	
	reply[0] = CUBETTO_MESSAGE_PLAY_TUNE; 
	reply[1] = status; 
	reply[2] = 0; 
	reply[3] = 0; 
	nrfWriteNotify(PIPE_PRIMO_RESPONSE_TX, 0, reply, 4);
}

/*********************************************************************************************************************************************************
 * @private
/*********************************************************************************************************************************************************/
/*********************************************************************************************************************************************************
 * @method  	uint16_t Cubetto::lookUpTone (uint8_t note)
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
uint16_t Cubetto::lookUpTone (uint8_t note)
{
  if (note>sizeof(toneTable-1)){return toneTable[sizeof(toneTable-1)];} 
  return toneTable[note]; 
}


/*********************************************************************************************************************************************************
 * @method  	void Cubetto::move() 
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
void Cubetto::move() 
{
  bool leftStepperIsRunning, rightStepperIsRunning;
	digitalWrite (IO_LED_YEL, HIGH);
  left.enableOutputs();
  right.enableOutputs();
  do {
    // Run both steppers
    leftStepperIsRunning = left.run();
    rightStepperIsRunning = right.run();
  }
  while (leftStepperIsRunning || rightStepperIsRunning);

  left.disableOutputs();
  right.disableOutputs();  
  digitalWrite (IO_LED_YEL, LOW);
}
/*********************************************************************************************************************************************************
 * @method  	void Cubetto::nrfSetup()
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
void Cubetto::nrfSetup()
{

	delay(10);  
  if (NULL != services_pipe_type_mapping)
    {
      aci_state.aci_setup_info.services_pipe_type_mapping = &services_pipe_type_mapping[0];
    }
    else
    {
      aci_state.aci_setup_info.services_pipe_type_mapping = NULL;
    }
    aci_state.aci_setup_info.number_of_pipes    = NUMBER_OF_PIPES;
    aci_state.aci_setup_info.setup_msgs         = setup_msgs;
    aci_state.aci_setup_info.num_setup_msgs     = NB_SETUP_MESSAGES;
    aci_state.aci_pins.interface_is_interrupt = false;
		nRFIsReady = false; 
    lib_aci_init(&aci_state,false);
		
		
	// Wait for Device to Start
	while (nRFIsReady == false){
		nrfLoop(); 
	}
	
	// Set Local Data for Battery
	uint8_t battery = 100; 
	nrfWriteLocal (PIPE_BATTERY_BATTERY_LEVEL_SET, &battery, 1); 
} 

/*********************************************************************************************************************************************************
 * @method  	void Cubetto::nrfLoop()
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
void Cubetto::nrfLoop()
{
	static bool setup_required = false;
  if (lib_aci_event_get(&aci_state, &aci_data)){
       aci_evt_t * aci_evt;
       aci_evt = &aci_data.evt;
       switch(aci_evt->evt_opcode) 
       {
         case ACI_EVT_DEVICE_STARTED:
           aci_state.data_credit_total = aci_evt->params.device_started.credit_available;
           switch(aci_evt->params.device_started.device_mode)
           {
             case ACI_DEVICE_SETUP:
               aci_state.device_state = ACI_DEVICE_SETUP;
               setup_required = true;
               break;
             case ACI_DEVICE_STANDBY:
               aci_state.device_state = ACI_DEVICE_STANDBY;
               if (aci_evt->params.device_started.hw_error){	delay(20);}
               else {nRFIsReady = true;}
               break;
						}
						break; 
         case ACI_EVT_CMD_RSP:
						if (ACI_STATUS_SUCCESS != aci_evt->params.cmd_rsp.cmd_status ){while (1);}
						if (aci_evt->params.cmd_rsp.cmd_opcode == ACI_CMD_SET_LOCAL_DATA){setLocalDataPending = false;}
						break;
         case ACI_EVT_DATA_RECEIVED:
						nrfOnWrite (aci_evt->params.data_received.rx_data.pipe_number, aci_evt->params.data_received.rx_data.aci_data); 
						break; 
         case ACI_EVT_PIPE_STATUS: 
						break;
         case ACI_EVT_TIMING:
						break;
         case ACI_EVT_DATA_ACK:
						break;
         case ACI_EVT_CONNECTED:
						aci_state.data_credit_available = aci_state.data_credit_total;
						bluetoothConnected=true; 
						bluetoothAdvertising=false;					
						bleLEDStatus=BLE_STATUS_CONNECTED; 
						playPairedTone(); 
						break;
         case ACI_EVT_DATA_CREDIT:
						aci_state.data_credit_available = aci_state.data_credit_available + aci_evt->params.data_credit.credit;
						break;
         case ACI_EVT_PIPE_ERROR:
						if (ACI_STATUS_ERROR_PEER_ATT_ERROR != aci_evt->params.pipe_error.error_code){aci_state.data_credit_available++;}
						break;
          case ACI_EVT_DISCONNECTED:
					  bluetoothConnected=false; 
						ioStreaming = false;
						bleMessage = false; 
						bleLEDStatus=BLE_STATUS_OFF; 
						if (ACI_STATUS_ERROR_ADVT_TIMEOUT == aci_evt->params.disconnected.aci_status)
            {
                bluetoothAdvertising=false;
            }
						else {
							playUnpairedTone(); 
							if (autoReadvertiseAfterDisconnect==true){
									startAdvertisingBluetooth(advertistingTimeout, true); 
							}
						}
						break;
         case ACI_EVT_HW_ERROR:
					 	bluetoothConnected=false; 
						bluetoothAdvertising=false;
						bleLEDStatus=BLE_STATUS_OFF;
						break;
       }
     }
		
		 // Need to set up NRF 
     if(setup_required){if (SETUP_SUCCESS == do_aci_setup(&aci_state)){setup_required = false;}}
}

/*********************************************************************************************************************************************************
 * @method  	void Cubetto::nrfOnWrite (uint8_t pipeNumber, uint8_t *data)
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
void Cubetto::nrfOnWrite (uint8_t pipeNumber, uint8_t *data)
{
	if (pipeNumber == PIPE_PRIMO_COMMAND_RX_ACK_AUTO){
		bluetoothMessageCommand = data[0]; 
		memcpy(bluetoothMessageData, (data+1), 3); 
		bleMessage=true; 
	}
} 

/*********************************************************************************************************************************************************
 * @method  	bool Cubetto::nrfWriteNotify (uint8_t txpipe, uint8_t setpipe, uint8_t* data, uint32_t size)
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
bool Cubetto::nrfWriteNotify (uint8_t txpipe, uint8_t setpipe, uint8_t* data, uint32_t size)
{
	
	if (nRFIsReady == false){
		 return false; 
	}


	if (lib_aci_is_pipe_available(&aci_state, txpipe)  && aci_state.data_credit_available > 0){
			uint8_t status = lib_aci_send_data(txpipe, data, size);
			if (status) {
				uint8_t currentDataCredits = aci_state.data_credit_available; 
				aci_state.data_credit_available--;
				// Wait for datacredit to be returned
				while (aci_state.data_credit_available!=currentDataCredits){
						nrfLoop(); // Process until clear.  
				}
				return true; 
			}
			return false; 
	}
	
	// If no pipe available, write to local data instead. 
	else {
		if (setpipe==0){
			nrfWriteLocal (setpipe, data, size); 
			return true; 
		}
	}
}

/*********************************************************************************************************************************************************
 * @method  	bool Cubetto::nrfWriteLocal (uint8_t pipe, uint8_t* data, uint32_t size)
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
bool Cubetto::nrfWriteLocal (uint8_t pipe, uint8_t* data, uint32_t size)
{
	if (nRFIsReady == false){
		 return false; 
	}
	
	uint8_t status = lib_aci_set_local_data(&aci_state, pipe, data, size);
	if (status){
		setLocalDataPending = true;
		while (setLocalDataPending==true){
				nrfLoop (); // Process until clear. 
		}
		return true; 
	}
	return false; 
}
/*********************************************************************************************************************************************************
 * @method  	void Cubetto::nrfLEDUpdate()
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
void Cubetto::nrfLEDUpdate()
{
    static uint32_t counter=0;
    static bool led_stat=false; 
    counter++; 
    switch (bleLEDStatus){
      case BLE_STATUS_OFF:
        digitalWrite(IO_LED_BLU, LOW); 
        break; 
      case BLE_STATUS_ADVERTISING:
        if (counter>0x7FFF){
            counter=0; 
            led_stat^=1; 
            digitalWrite(IO_LED_BLU, led_stat); 
        }
        break; 
      case BLE_STATUS_CONNECTED:
        digitalWrite(IO_LED_BLU, 1); 
        break; 
    }
}
/*********************************************************************************************************************************************************
 * @method  	void Cubetto:: ioUpdateStream()
 * @brief  		Private Method
/*********************************************************************************************************************************************************/
void Cubetto:: ioUpdateStream()
{
  static uint32_t counter = 0;
  int16_t ana1=0;
  int16_t ana2=0;
  int16_t ana3=0;
  uint8_t digital=0; 
  uint8_t buf[7]; 
  
	counter++; 
  if (counter>0xFFFF){
   counter=0; 
   if (ioStreaming==true){
     // Read ANA
     ana1 = analogRead(IO_ANA_1);     // Read ANA1
     ana2 = analogRead(IO_ANA_2);     // Read ANA2
     ana3 = analogRead(IO_ANA_3);     // Read ANA3
  
     // Read DIG
     digital 	= digitalRead(IO_DIG_1);
     digital |=  (digitalRead(IO_DIG_2)<<1);
     digital |=  (digitalRead(IO_DIG_3)<<2);  

    // Read Button 
     digital |=  (digitalRead(IO_BUT_1)<<3);
     digital |=  (digitalRead(IO_BUT_2)<<4);  
     
     buf[0] = digital; 
     buf[1] = ana1 & 0xFF;
     buf[2] = (ana1>>8) & 0xFF; 
     buf[3] = ana2 & 0xFF;
     buf[4] = (ana2>>8) & 0xFF; 
     buf[5] = ana3 & 0xFF;
     buf[6] = (ana3>>8) & 0xFF; 
  
     // Send to BLE
		 nrfWriteNotify (PIPE_PRIMO_IO_TX, 0, buf, 7); 
   }
  }
}





