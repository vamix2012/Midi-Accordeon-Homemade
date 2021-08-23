# Midi-Accordeon-Homemade

First Version of this Homemade Midi accordeon with MIDI over BLE and Bellows pressure control functions

## Overerview

I am glad to pressent my work witch took some time to be finished but the result is tremendous.

### What was the problem? Why this project?

We all know that accordeons are loud instruments

- Most of the time I met following situations
  - My beloved daughters are sleeping
  - It's 13:00-15:00 or after 22:00 o'clock
  - That means "Be silent don't play that Accordeon"

### Solution

- Roland V-Accordeon a great one but.... 3000-6000€ Sorry it's a no :)

  - I can buy a car with that amount of money, defenetly no.

- Ok, then? An Accordeon with midi builtin was an option but still between 1000-2000€

  - You need special cables and special adapters to plug into a synth
  - It's somehow a mess, ok, that's obviously another NO.

- Ok, there must be a solution...

  - Make your own Midi Accordeon and fit it to your needs, Hurray that's it.

- So I went building my own one under 200€.
  - Even the fact that I didn't had Programming skills, took the Challenge and it was a success.

### Story

After researching I found just parts of what I needed so I did it my way putting the parts together and adding some spice, backed it for some time(Tweaking) adding it in the Controller and it worked. I will add more Info Later stay tuned.

### Parts

| Parts used in this project                                         |
| ------------------------------------------------------------------ |
| Old accordeon, I recommend one with good compression and good keys |
| ESP32 DEVKIT V1                                                    |
| 65 Hall Sensors and small round 4x2mm magnets                      |
| 5 CD74HC4067 multiplexers                                          |
| BMP180 pressure sensor                                             |
| 8000mAh battery                                                    |
| Micro USB Male to USB2.0 B Type Female Adapter w/ monting screws   |
| USB 3.1 extender cable w/ mounting screws                          |

### Arduino Libraries

The following libraries are required to compile the MIDI_Accordion Arduino sketch:

- [Arduino BLE-MIDI Transport](https://github.com/FortySevenEffects/arduino_midi_library/)
  - Used to send MIDI Messages over BLE based on the Arduino MIDI Library.
- [Sparkfun's Arduino BMP180 Library](https://github.com/sparkfun/BMP180_Breakout)
  - Used to interface with the BMP180 barometric pressure sensor.
- [Multiplexer CD4067 library](https://github.com/sumotoy/Multiplexer4067)
  - Used to extend the inputs of the board.
  - With just 5 pins you get 16 inputs
  - With 5 Multiplexers(Total of 65 Inputs) used just 9 pins on the Board

### Third Party

Software:

- FluidSynth with .sf2 soundfonts
  - A great Soundfont is Orpheus GM(https://musical-artifacts.com/artifacts/1213)
  - SGM Soundfont is a Light and Compact(https://musical-artifacts.com/artifacts/855)
  - Feel free to use any Soundfont out there ;)

Hardware:

- Android Phone

As long as I know there is only an Android Version of FluidSynth on (Mobile devices) but you can checkout yourself on their website(https://www.fluidsynth.org/download/)

How this Works:

Connection between the Accordeon and FluidSynth is done with MIDI over BLE function of the ESP32

### Why I choose this setup?

As you can see is just an app on the phone, the controller connects to it over BLE and thats it. Simple as that.

This cool guy inspired me.(https://www.youtube.com/playlist?list=PLuxHP0PdeNpYkzfZUs_iHeYi_2xwNMNbF)

### Credits

- Gustavo Silveira aka Musico Nerd for the DIY-Midi-controller(https://github.com/silveirago)
- Brendan Vavra for the pressure sensor documentation and code(https://github.com/bvavra)
