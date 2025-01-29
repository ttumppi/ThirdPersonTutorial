#include "Vector2DWithMinMax.h"
#include "MinMaxSpan.h"
#include "vector"
#include "GlobalMutexFunctions.h"


Vector2DWithMinMax::Vector2DWithMinMax(double x, double y, const std::vector<MinMaxSpan>& spanX, const std::vector<MinMaxSpan>& spanY) {

	_x = x;
	_y = y;
	_spanX = spanX;
	_spanY = spanY;
	
}

Vector2DWithMinMax::Vector2DWithMinMax() {
	_x = 0.0f;
	_y = 0.0f;
	_spanX = std::vector<MinMaxSpan>();
	_spanY = std::vector<MinMaxSpan>();
}

double Vector2DWithMinMax::GetX() const {
	return _x;
}

double Vector2DWithMinMax::GetY() const {
	return _y;
}

bool Vector2DWithMinMax::SetX(double value) {

	if (_spanX.size() == 0) {
		GlobalMutexFunctions::SetValueThreadSafely(_lockForX, _x, value);
		return true;
	}

	for (const MinMaxSpan& span : _spanX) {
		if (!span.Valid(value)) {
			return false;
		}
	}
	

	GlobalMutexFunctions::SetValueThreadSafely(_lockForX, _x, value);
	
	return true;

}

bool Vector2DWithMinMax::SetY(double value) {

	if (_spanY.size() == 0) {
		GlobalMutexFunctions::SetValueThreadSafely(_lockForY, _y, value);
		return true;
	}

	

	for (const MinMaxSpan& span : _spanY) {
		if (!span.Valid(value)) {
			return false;
		}
	}

	GlobalMutexFunctions::SetValueThreadSafely(_lockForY, _y, value);

	return true;
}
