/////////////////////////////////////////////
// LIBRARIES
//#include <BLEMIDI_Transport.h>//MIDI over Bluetooth Low Energy
//#include <hardware/BLEMIDI_ArduinoBLE.h>
#include <USB-MIDI.h>
#include <Multiplexer4067.h> // Multiplexer CD4067 library >> https://github.com/sumotoy/Multiplexer4067
#include <SFE_BMP180.h>// Pressure Sensor
#include <Wire.h>//I2C Bus


SFE_BMP180 bmp_180;
USBMIDI_CREATE_DEFAULT_INSTANCE();

//BLEMIDI_CREATE_INSTANCE("Chris's Midi Akkordeon", MIDI)
bool isConnected = false;
bool isExpression = true;

///////////////////////////////////////////
// MULTIPLEXERS
#define N_MUX 5 //* number of multiplexers
//* Define s0, s1, s2, s3, and x pins
#define s0 2
#define s1 3
#define s2 4
#define s3 5
#define xa1 A0
#define xa2 A1
#define xa3 A2
#define xa4 A3
#define xa5 A6// analog pin of the first mux

// Initializes the multiplexer
Multiplexer4067 mux[N_MUX] = {
  Multiplexer4067(s0, s1, s2, s3, xa1),
  Multiplexer4067(s0, s1, s2, s3, xa2),
  Multiplexer4067(s0, s1, s2, s3, xa3),
  Multiplexer4067(s0, s1, s2, s3, xa4),
  Multiplexer4067(s0, s1, s2, s3, xa5)
};
//////////////////Presure sensor part////////////////////////

double Temp, P;
int tempStatus;
int pressureStatus;
double Calib_Pressure;

const int CC_Expression = 11;
int prev_expression = 127;

void init_BMP() {

  // Initialize and calibrate the sensor
  if (bmp_180.begin())
  {
    //get an average reading
    for (int i = 0; i < 32; i++) {
      Calib_Pressure += readPressure();
      delayMicroseconds(500);
    }
    Calib_Pressure = Calib_Pressure / 32;
  }
  else //something went wrong - this is usually a connection problem
  {
    while (1); // Pause forever.
  }
  reset_flags();
}

//This is only used to calibrate the BMP on startup because it takes about 12ms
double readPressure() {
  //We need to read the temperature every time because
  //wind affects it, which in turn affects the pressure
  tempStatus = bmp_180.startTemperature();
  if (tempStatus != 0)
  {
    delay(tempStatus);// Wait for the measurement to complete:

    // Retrieve the completed temperature measurement:
    tempStatus = bmp_180.getTemperature(Temp);//sets Temp
    if (tempStatus != 0)
    {
      // Start a pressure measurement:
      pressureStatus = bmp_180.startPressure(0);//Passing in 0 for speed
      if (pressureStatus != 0)
      {
        delay(pressureStatus);// Wait for the measurement to complete:

        // Retrieve the completed pressure measurement using the given temp:
        pressureStatus = bmp_180.getPressure(P, Temp); //sets P to pressure (in millibars)
        if (pressureStatus == 0) {
#ifdef DEBUG
Serial.println("error retrieving pressure measurement\n");
#endif
}
} 
else {
#ifdef DEBUG
Serial.println("error starting pressure measurement\n");
#endif
}
} 
else {
#ifdef DEBUG
Serial.println("error retrieving temperature measurement\n");
#endif
}
} 
else {
#ifdef DEBUG
Serial.println("error starting temperature measurement\n");
#endif
}

          return P * 100; //converting from millibars to pascals
        }

        int temp_wait_ms = 0;
        int pressure_wait_ms = 0;
        unsigned long temp_init_ms = 0;
        unsigned long temp_final_ms = 0;
        unsigned long pressure_init_ms = 0;
        unsigned long pressure_final_ms = 0;

        bool need_temp_reading = true;
        bool temp_started = false;
        bool temp_ready = false;
        bool pressure_started = false;
        bool pressure_ready = false;

        void reset_flags() {
          need_temp_reading = true;
          temp_started = false;
          temp_ready = false;
          pressure_started = false;
          pressure_ready = false;
          temp_wait_ms = 0;
          pressure_wait_ms = 0;
          temp_init_ms = 0;
          temp_final_ms = 0;
          pressure_init_ms = 0;
          pressure_final_ms = 0;
        }

        /* If we're entering this function, we're in one of the following states:
           1 - We just entered and need a temp reading
           2 - We're waiting for temp reading but it's not ready yet
           3 - Temp reading is ready so we can start pressure reading
           4 - We're waiting for pressure reading but it's not ready yet
           5 - pressure reading is ready and we can get our new expression
        */
        int get_expression(int prev_expression) {
          int expression = prev_expression;

          if (need_temp_reading) { //States 1-3
            if (temp_started && !temp_ready) { //2 - We're waiting for temp reading but it's not ready yet
              temp_final_ms = millis();
              temp_wait_ms = temp_wait_ms + (temp_final_ms - temp_init_ms);
              temp_init_ms = millis();
              if (temp_wait_ms > tempStatus) {
                temp_ready = true;
              }
            }
            if (!temp_started) { //1 - We just entered and need a temp reading
              temp_init_ms = millis();
              tempStatus = bmp_180.startTemperature();
              temp_started = true;
            }
            if (temp_ready) { //3 - Temp reading is ready so we can start pressure reading
              tempStatus = bmp_180.getTemperature(Temp);
              need_temp_reading = false;
              pressure_init_ms = millis();
              pressureStatus = bmp_180.startPressure(0);
              pressure_started = true;
            }
          }
          else if (pressure_started) { //States 4-5
            if (!pressure_ready) { //4 - We're waiting for pressure reading but it's not ready yet
              pressure_final_ms = millis();
              pressure_wait_ms = pressure_wait_ms + (pressure_final_ms - pressure_init_ms);
              pressure_init_ms = millis();
              if (pressure_wait_ms > tempStatus) {
                pressure_ready = true;
              }
            }
            if (pressure_ready) { //5 - pressure reading is ready and we can get our new expression
              pressureStatus = bmp_180.getPressure(P, Temp);
              expression = get_expression_from_pressure(P * 100); //converting millibars to pascals
              reset_flags();
            }
          }

          return expression;
        }

        //Minimum amount of pressure change required to cause a change in volume.
        int pressure_low_limit = 0;
        int min_velocity = 126;
        int get_expression_from_pressure(double Pressure) {
          int expression = 0;
          float Pressure_Delta = abs(Pressure - Calib_Pressure);
          expression = int(0.00000005 * pow(Pressure_Delta - 450, 3) + 0.112 * Pressure_Delta) + 50;
          if (Pressure_Delta < pressure_low_limit) {
            expression = min_velocity;
          }
          if (expression > 127) {
            expression = 127;
          }
          return expression;
        }
        //////////////////////Presure sensor end////////////////////////////

        /////////////////////////////////////////////
        // BUTTONS
        const int N_BUTTONS = 0 + 12 + 12 + 16 + 16 + 9; //*  total numbers of buttons. Number of buttons in the Arduino + number of buttons on multiplexer 1 + number of buttons on multiplexer 2...
        const int N_BUTTONS_ARDUINO = 0; //* number of buttons connected straight to the Arduino (in order)
        const int BUTTON_ARDUINO_PIN[N_BUTTONS] = {}; //* pins of each button connected straight to the Arduino

        const int N_BUTTONS_PER_MUX[N_MUX] = {12, 12, 16, 16 , 9}; //* number of buttons in each mux (in order)
        const int BUTTON_MUX_PIN[N_MUX][16] = {
          {1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14},//Pins of first mux
          {1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13, 14},//Pins of second mux
          {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},//Pins of third mux
          {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},//Pins of fourth mux
          {0, 1, 2, 3, 4, 5, 6, 7, 8}//Pins of fifth mux

        };

        int buttonMuxThreshold = 600;
        int buttonCState[N_BUTTONS] = {0};        // stores the button current value
        int buttonPState[N_BUTTONS] = {0};        // stores the button previous value

        // debounce
        unsigned long lastDebounceTime[N_BUTTONS] = {0};  // the last time the output pin was toggled
        unsigned long debounceDelay = 5;    //* the debounce time; increase if the output flickers

        unsigned long myTime = millis();

        // velocity
        byte velocity[N_BUTTONS] = {100};

        /////////////////////////////////////////////
        // MIDI Channels to be used
        byte MIDI_CH2 = 2;
        byte MIDI_CH3 = 3;
        byte MIDI_CH = 1;
        // Lowest NOTE to be used
        byte NOTE = 40;
        byte PATCH_NUM = 1;
        ///////////////////////////////////////////////

        void setup() {
        
          Serial.begin(115200);
          
          MIDI.begin(MIDI_CHANNEL_OMNI);

          // Buttons
          // Initialize buttons with pull up resistors
          for (int i = 0; i < N_BUTTONS_ARDUINO; i++) {
            pinMode(BUTTON_ARDUINO_PIN[i], INPUT_PULLUP);
          }

          init_BMP();

          /////////////////////////////////////////////
          // Multiplexers
          // Initialize the multiplexers
          for (int i = 0; i < N_MUX; i++) {
            mux[i].begin();
          }
          //* Set each X pin as input_pullup (avoid floating behavior)
          pinMode(xa1, INPUT_PULLUP);
          pinMode(xa2, INPUT_PULLUP);
          pinMode(xa3, INPUT_PULLUP);
          pinMode(xa4, INPUT_PULLUP);
          pinMode(xa5, INPUT_PULLUP);
        }


        void loop() {
          buttons();
          readPressureFromBMP();
        }

        //////////Constantly read pressure
        void readPressureFromBMP() {
          int expression = get_expression(prev_expression);

          //Ignore it if it didn't change
          if (expression != prev_expression) {
            
            MIDI.sendControlChange(CC_Expression, expression, 3);
            //Don't let bass overpower melody
            MIDI.sendControlChange(CC_Expression, expression, 1);
            //Don't let chords overpower melody
            MIDI.sendControlChange(CC_Expression, expression, 2);
            prev_expression = expression;
          }
        }
        /////////////////////////////////////////////
        // BUTTONS
        void buttons() {

          // It will happen if you are using MUX
          int nButtonsPerMuxSum = N_BUTTONS_ARDUINO; // offsets the buttonCState at every mux reading

          // read the pins from every mux
          for (int j = 0; j < N_MUX; j++) {
            for (int i = 0; i < N_BUTTONS_PER_MUX[j]; i++) {
              buttonCState[i + nButtonsPerMuxSum] = mux[j].readChannel(BUTTON_MUX_PIN[j][i]);
              
              // Scale values to 0-1
              if (buttonCState[i + nButtonsPerMuxSum] > buttonMuxThreshold) {
                buttonCState[i + nButtonsPerMuxSum] = HIGH;
              }
              else {
                buttonCState[i + nButtonsPerMuxSum] = LOW;
              }
            }
            nButtonsPerMuxSum += N_BUTTONS_PER_MUX[j];
          }


          for (int i = 0; i < N_BUTTONS; i++) { // Read the buttons connected to the Arduino
            if ((millis() - lastDebounceTime[i]) > debounceDelay) {
              if (buttonPState[i] != buttonCState[i]) {
                lastDebounceTime[i] = millis();
                //// Map for Chords
                if (i >= 12 && i <= 23) {
                  if (buttonCState[i] == LOW) {
                    velocity[i] = 103; // if button is pressed velocity is 88
                    MIDI.sendNoteOn(NOTE + i, velocity[i], MIDI_CH2);// note, velocity, channel
                    MIDI.sendNoteOn((NOTE + i) + 12, velocity[i], MIDI_CH2);
                  }
                  else {
                    velocity[i] = 0; // if button is released velocity is 0
                    MIDI.sendNoteOn(NOTE + i, velocity[i], MIDI_CH2);// note, velocity, channel
                    MIDI.sendNoteOn((NOTE + i) + 12, velocity[i], MIDI_CH2);
                  }
                }
                //// Map for basses
                if (i <= 11) {
                  if (buttonCState[i] == LOW) {
                    velocity[i] = 115; // if button is pressed velocity is 94
                    MIDI.sendNoteOn(NOTE + i, velocity[i] , MIDI_CH); // note, velocity, channel
                    MIDI.sendNoteOn((NOTE + i) - 12, velocity[i] , MIDI_CH);
                  }
                  else {
                    velocity[i] = 0; // if button is released velocity is 0
                    MIDI.sendNoteOn(NOTE + i, velocity[i] , MIDI_CH); // note, velocity, channel
                    MIDI.sendNoteOn((NOTE + i) - 12, velocity[i] , MIDI_CH);
                  }
                }
                //// Map for Right hand notes
                if (i > 23) {
                  if (buttonCState[i] == LOW) {
                    velocity[i] = 127; // if button is pressed velocity is 110
                    MIDI.sendNoteOn(NOTE + i - 23, velocity[i], MIDI_CH3); // note, velocity, channel
                  }
                  else {
                    velocity[i] = 0; // if button is released velocity is 0
                    MIDI.sendNoteOn(NOTE + i - 23, velocity[i], MIDI_CH3); // note, velocity, channel
                  }
                  // Sends the MIDI NOTE ON accordingly to the chosen board
                }

                if (buttonCState[64] == LOW && buttonCState[48] == LOW && millis() - myTime > 500) {
                  if (PATCH_NUM > 127) {
                    PATCH_NUM = 127;
                  } else {
                    PATCH_NUM++;
                  }
                  MIDI.sendProgramChange(PATCH_NUM, 3);
                  myTime = millis();
                }
                if (buttonCState[63] == LOW && buttonCState[48] == LOW && millis() - myTime > 500) {
                  if (PATCH_NUM < 2) {
                    PATCH_NUM = 1;
                  } else {
                    PATCH_NUM--;
                  }
                  MIDI.sendProgramChange(PATCH_NUM, 3);
                  myTime = millis();
                }

                buttonPState[i] = buttonCState[i];
              }

            }
          }
        }
