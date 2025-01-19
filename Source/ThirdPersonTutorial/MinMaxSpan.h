#pragma once

struct MinMaxSpan {

private:
	

public:

	float Min;
	float Max;


	MinMaxSpan(float min, float max);

	bool Valid(float value) const;
};