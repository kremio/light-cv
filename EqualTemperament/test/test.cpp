#include "lest.hpp"
#include "EqualTemperament.cpp"

#include <iostream>
using namespace std;
using lest::approx;

const lest::test specification[] =
{
  CASE( "Provides a constant for 12 bits DAC" )
    {
        EXPECT( pow(2,12) - 1 == EqualTemperament::DAC_12 );
    },

    CASE( "Returns the DAC midpoint for C4" )
    {
	    //12 bits DAC, 3 volts output range
	    EqualTemperament ET_3VOLTS( 30, 3 );
		//12 bits DAC, 5 volts output range
		EqualTemperament ET_5VOLTS( 30, 5 );
		int midpoint = 15;
		
        EXPECT( midpoint == ET_3VOLTS.relativePitchToDAC( 0 ) );
		EXPECT( midpoint == ET_5VOLTS.relativePitchToDAC( 0 ) );
    },

	CASE( "Correctly perfoms conversion (12 bits DAC, 5 Volts output)" )
	{
		//A semitone step for a 12 bits DAC using a 5 V output range
		float semitone_12bits_5volts = (pow(2.0f,12) - 1.0f) / (12.0f * 5.0f);
		//C4 is at 2.5V = 
		int C4 = EqualTemperament::DAC_12 / 2;
		int radius = C4 / semitone_12bits_5volts;

		EqualTemperament ET( EqualTemperament::DAC_12, 5 );
		
		float expected = 0;
		int converted = 0;
		for(int relativePitch = -radius; relativePitch <= radius; relativePitch++){
			expected = C4 + relativePitch * semitone_12bits_5volts;
			converted = ET.relativePitchToDAC( relativePitch );
			cout << relativePitch << "=>" << expected << " : " << converted << "\n";
			EXPECT( approx( expected ).epsilon( 0.25 ) == converted );
		}
	},

  	CASE( "Return 0 for pitches lower than range" )
	{
		EqualTemperament ET( 50, 5 );

		EXPECT( 0 == ET.relativePitchToDAC( -12*2.6 ) );
	},

  	CASE( "Return given input resolution for pitches higher than range" )
	{
		EqualTemperament ET( 50, 5 );

		EXPECT( 50 == ET.relativePitchToDAC( 12*2.7 ) );
	}

};

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}
