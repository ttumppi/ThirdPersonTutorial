#pragma once

#include "MinMaxSpan.h"
#include "vector"
#include "mutex"

/**
* Capsulates xy doubles and has optional checks for min and max for both values.
* if min or max lists are empty, does not include value set checks.
*/
struct Vector2DWithMinMax {

public:
	Vector2DWithMinMax(double x, double y, const std::vector<MinMaxSpan>& spanX, const std::vector<MinMaxSpan>& spanY);

	Vector2DWithMinMax();

	double GetX();
	double GetY();

	/** Set x coordinate and possibly include min max check */
	bool SetX(double value);

	/** Set y coordinate and possibly include min max check */
	bool SetY(double value);
	

private:
	std::vector<MinMaxSpan> _spanX;

	std::vector<MinMaxSpan> _spanY;

	std::mutex _lockForX;
	
	std::mutex _lockForY;

	double _x;

	double _y;

	
};
