# Midi-Accordeon-Homemade

First MIDI Accordion to use Internal Sounds and amplified Speakers with additional Bellows pressure control functions

## Overview

I am glad to present my work, which took some time to be finished, but the result is tremendous.

### What was the problem? Why this project?

We all know that accordions are loud instruments

- Most of the time I met following situations
  - My beloved daughters are sleeping
  - It's 13:00-15:00 or after 22:00 o'clock
  - That means "Be silent, don't play that Accordion"

### Solution

- Roland V-Accordeon a great one but.... €3000-€6000, Sorry it's a no :)

  - I can buy a car with that amount of money, absolutely no.

- Ok, then? An Accordion with midi built-in was an option but still between €1000-€2000

  - You need special cables and special adapters to plug into a synth
  - It's somehow a mess, ok, that's obviously another NO.

- Ok, but there must be a solution...

  - Make your own Midi Accordion and fit it to your needs, Hurray, that's it.

- So I went to building my own one under €200.
  - Even the fact that at the time I started the project had no programming skills.
  - But took the Challenge, and it was a success.
  - That was my turning point to programming.
  - I learned the Arduino code.
  - Then decided to go with Frontend development and learn it professionally.
  - Now, after a year of intense work, I am job ready in Frontend development.

### Story

After researching I found just parts of what I needed, so I did it my way putting the parts together and adding some spice, backed it for some time(Tweaking) adding it in the Controller, and it worked. I will add more Info Later, stay tuned.

### Parts

| Parts used in this project                                                         |
| ---------------------------------------------------------------------------------- |
| Old accordion, I recommend one with good compression and good mechanical condition |
| ESP32 DEVKIT V1                                                                    |
| Arduino NANO 33 IoT                                                                |
| Raspberry Pi 3b+ With DAC+ Audio                                                   |
| Old USB Computer Speakers                                                          |
| 65pcs Hall Sensors and small round 4x2mm magnets                                   |
| 5pcs CD74HC4067 multiplexers                                                       |
| BMP180 pressure sensor                                                             |
| 8000mAh battery                                                                    |
| Micro USB Male to USB2.0 B Type Female Adapter w/ mounting screws                  |
| USB 3.1 extender cable w/ mounting screws                                          |

### Arduino Libraries

The following libraries are required to compile the MIDI_Accordion Arduino sketch:

- [Arduino USB-MIDI Transport](https://github.com/lathoub/Arduino-USBMIDI)
  - Used to send MIDI Messages over USB based on the Arduino MIDI Library.
- [Sparkfun's Arduino BMP180 Library](https://github.com/sparkfun/BMP180_Breakout)
  - Used to interface with the BMP180 barometric pressure sensor.
- [Multiplexer CD4067 library](https://github.com/sumotoy/Multiplexer4067)
  - Used to extend the inputs of the board.
  - With just 5 pins you get 16 inputs
  - With 5 Multiplexers(Total of 65 Inputs) used just 9 pins on the Board

### Software on Headless Raspberry Pi:

- FluidSynth with .sf2 sound fonts
  - A great Sound font is [Orpheus GM](https://musical-artifacts.com/artifacts/1213)
  - A Light and Compact Sound font is[SGM Soundfont](https://musical-artifacts.com/artifacts/855)
  - Feel free to use any Sound font out [there](https://www.google.com/search?q=soundfonts+sf2&rlz=1C1CHBD_deDE932DE932&sxsrf=ALeKk02X2pNNehXxbOWeKxTRRQ1Wxy9J7w%3A1629754976019&ei=YBYkYcJEnf3v9Q-E4rzwCg&oq=soundfonts+sf2&gs_lcp=Cgdnd3Mtd2l6EAMyBQgAEMsBMgUIABDLATIFCAAQywEyBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjIGCAAQFhAeMgYIABAWEB4yBggAEBYQHjoHCAAQRxCwAzoHCAAQsAMQQzoFCAAQgAQ6BAgAEB5KBAhBGABQqTdYgFlgjV1oA3ACeACAAX6IAacGkgEDNi4ymAEAoAEByAEKwAEB&sclient=gws-wiz&ved=0ahUKEwjC9KmzjsjyAhWd_rsIHQQxD64Q4dUDCA4&uact=5) ;)

### How this Works:

- Esp32 is reading the sensor keystroke values and pressure, converts them into MIDI signals
- Arduino NANO receives the MIDI signals from the Esp32 and redirects them to the Raspberry Pi
- Raspberry Pi runs FluidSynth to turn MIDI signals into sounds
- The sound is played trough internal speakers or Headphones

### Why I choose this setup?

Turn the device on and wait a couple of seconds to boot up, and you can play right away, Because it's standalone and doesn't need any third party software or hardware to run.

### Credits

- [Gustavo Silveira](https://github.com/silveirago) Musico Nerd, DIY-Midi-controller Workshop.
- [Brendan Vavra](https://github.com/bvavra) for the pressure sensor documentation and code.
- [This guy inspired me to use the ESP32 instead of Arduino.](https://www.youtube.com/playlist?list=PLuxHP0PdeNpYkzfZUs_iHeYi_2xwNMNbF)
