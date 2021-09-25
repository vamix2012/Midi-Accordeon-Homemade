#include <MIDI.h>  // Add Midi Library
#include <USB-MIDI.h>

USBMIDI_CREATE_INSTANCE(0, MIDIUSB);//Create an instance of the library with default name, serial port and settings
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);



void setup() {
  // Set Arduino board pin 13 to output
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
 
  // OMNI sets it to listen to all channels.. MIDI.begin(2) would set it
  // to respond to notes on channel 2 only.
  MIDI.setHandleNoteOn(MyHandleNoteOn); // This is important!! This command
  // tells the Midi Library which function you want to call when a NOTE ON command
  // is received. In this case it's "MyHandleNoteOn".
  MIDI.setHandleNoteOff(MyHandleNoteOff); // This command tells the Midi Library
  // to call "MyHandleNoteOff" when a NOTE OFF command is received.
  MIDI.setHandleControlChange (MyControlChange);
  MIDI.setHandleProgramChange (MyProgrammChange);
}

void loop() { // Main loop
  MIDI.read(); // Continuously check if Midi data has been received.
}

// MyHandleNoteON is the function that will be called by the Midi Library
// when a MIDI NOTE ON message is received.
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {
  MIDIUSB.sendNoteOn(pitch, velocity, channel);// note, velocity, channel
}

// MyHandleNoteOFF is the function that will be called by the Midi Library
// when a MIDI NOTE OFF message is received.
// * A NOTE ON message with Velocity = 0 will be treated as a NOTE OFF message *
// It will be passed bytes for Channel, Pitch, and Velocity
void MyHandleNoteOff(byte channel, byte pitch, byte velocity) {
  MIDIUSB.sendNoteOff(pitch, velocity, channel);// note, velocity, channel
}

void MyControlChange(byte channel, byte number, byte value) {
  MIDIUSB.sendControlChange(number, value, channel);
}

void MyProgrammChange(byte channel, byte number) {
  MIDIUSB.sendProgramChange(number, channel);
}
