#ifndef EqualTemperament_h
#define EqualTemperament_h

class EqualTemperament{
	public:
		static const long DAC_12;
		EqualTemperament(long dacResolution, int octaves){
			max = dacResolution;
			radius = octaves * 6;
			semitoneVolt = ( dacResolution * 100L ) / (octaves * 12L);
			c4Volt = dacResolution / 2L;
		}
		long relativePitchToDAC( long semitonesFromC4 );
	private:
		int radius;
		int max;
		long semitoneVolt;
		long c4Volt;
};

#endif
