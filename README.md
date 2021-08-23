# Midi-Accordeon-Homemade

First Version of this Homemade Midi accordeon with MIDI over BLE and Bellows pressure control functions

## Overerview

I am glad to pressent my work witch took some time to be finished but the result is tremendous.

### Problem?

We all know that accordeons are loud instruments
Most of the time I met following situations
-My beloved daughters are sleeping
-It's 13:00-15:00 or after 22:00 o'clock where is silence time

### Solution

I was thinking about Roland V-Accordeon a great one but.... 3000-6000€
I can buy a car with that amount of money, defenetly no.

Ok, then? An Accordeon with midi builtin was an option but still between 1000-2000€
and you need special cables and special adapters to plug into a synth
It's somehow a mess, ok, that's obviously another NO.

Ok, there must be a solution...
Make your own Midi Accordeon and fit it to your needs, Hurray that's it.

So I went making my own one under 200€.

### Story

I started resesrching about DIY MIDI Accordeons and found some useful information like:

Brendan Vavra, the ideea of using BMP180 for bellow pressure control.

Electronic Bytes(Youtube), the ideea of using ESP32 instead of good old Arduino pro Micro.

Musico Nerd(DIY Midi Controller) using multiplexers to extend inputs and map them to MIDI messages.

But it was not that easy to put them all together and start right away...
Some used Hall sensors in their build but no multiplexers,
Others used different kind of switches and Matrix methods and so on
I went doing it my way with a bit of code from Musico Nerd optimized to my needs, because he had no application for using his code with an accordeon but seems to be a very good fit.

### Parts

| Parts used in this project                                         |
| ------------------------------------------------------------------ |
| Old accordeon, I recommend one with good compression and good keys |
| ESP32 DEVKIT V1                                                    |
| 65 Hall Sensors and small round 4x2mm magnets                      |
| 5 CD74HC4067 multiplexers                                          |
| BMP180 pressure sensor                                             |
| 8000mAh battery                                                    |
| Micro USB Male to USB2.0 B Type Female Adapter with monting screws |
| USB 3.1 extender cable w/ mounting screws                          |

Others:
FluidSynth with .sf2 soundfonts
Connection between the Accordeon and the software is done with MIDI over BLE function of the ESP32

Credits to Gustavo Silveira aka Musico Nerd for the DIY-Midi-controller and Brendan Vavra for the pressure sensor part part
