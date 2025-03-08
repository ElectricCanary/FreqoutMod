# FreqoutMod

## Intro

This is a mod for the [Digitech FreqOut](https://digitech.com/dp/freqout/) guitar pedal. It adds peripheral momentary switches to control the "type" pot, making possible to create musical feedback easily.  
The mod is based on the Attiny84. The only mod toA filtered PWM output generates a voltage that replaces the one of the type pot. DIP switches are added to choose which momentary footswitche are linked to which Type.

## Pedal Overview

![Circuit](/img/circuit.jpg)
![Circuit Back](/img/circuitback.jpg)
![Face Circuit](/img/face.jpg)

| **Type** | **Voltage** |
|----------|-------------|
| Sub      | 0V          |
| 1st      | 0.24V       |
| 2nd      | 0.89V       |
| 3rd      | 1.57V       |
| 5th      | 2.27V       |
| Nat Low  | 2.95V       |
| Nat Hi   | 3.3V        |

## Pedal Mod

![Pedal Mod](/img/facemod.jpg)