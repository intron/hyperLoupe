# hyperLoupe
Toggles pin high and low at a specified rate. Frequency can be changed from the console.
The idea is to use this program to flash lights in order to determine [Flicker Fusion Threshold] (http://en.wikipedia.org/wiki/Flicker_fusion_threshold).

## Instructions
Connect to the Arduino and open up the Serial Monitor (or an equivalent tool). The program should prompt you to set the strobeFreuency.
It accepts floating point values but does not sanitize the input. There are upper and lower bounds on the achievable frequencies, but these limits should be irrelevant, unless you feel like submitting an issue ;)
