
#include "MinMaxSpan.h"


MinMaxSpan::MinMaxSpan(float min, float max) {

	Min = min;
	Max = max;
}

bool MinMaxSpan::Valid(float value) const {

	
	return value > Min && value < Max;
	
}