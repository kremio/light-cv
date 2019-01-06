#include "EqualTemperament.h"

const long EqualTemperament::DAC_12 = 4095L;

long EqualTemperament::relativePitchToDAC( long semitonesFromC4 ){
	if( semitonesFromC4 <= -radius ){
		return 0;
	}
	if( semitonesFromC4 >= radius ){
		return max;
	}
	return c4Volt + ( ( semitonesFromC4 * semitoneVolt) / 100L );
}
