# 12 tones equal temperament tuning system to DAC input conversion

A tiny utility library to convert a pitch from the 12 tone equal temperament system to an integer that can be sent to a DAC to produce the appropriate voltage.  
This utility uses the 1 volt equals 1 octave system, and the midpoint of the DAC voltage output range is chosen as encoding C4. In other words, if your output voltage is 5 volts, your DAC will output 2.5 volts for C4, 1.5 volts for C3 and 3.5 volts for C5.  
An input pitch is given as a difference to C4 in semitones.  

## Installation
Copy ./src/EqualTemperament.cpp and ./src/EqualTemperament.h to you Arduino sketch folder.

## Usage
Include the library header:
```
#include "EqualTemperament.h"
```

Create an instance of the class by passing the target DAC maximum input value as a first parameter, and the DAC max output voltage as a second parameter.  
For instance for a 32 bits DAC with a 5 volts max outputs, you would use:
```
EqualTemperament ET( /*2^32 - 1*/4294967295, 5 );
```

For convenience, a constant for 12 bits resolution DAC is provided:
```EqualTemperament::DAC_12```

Call `relativePitchToDAC` to then convert a pitch relative to C4 (in semitones) into a value you can send to your DAC.  
Some examples:
```
ET.relativePitchToDAC(0); //C4
ET.relativePitchToDAC(-12); //C3
ET.relativePitchToDAC(12); //C5
ET.relativePitchToDAC(15); //D#5
```
