#include "Lap.h"
#include "../Utils.h"
#include <iostream>


void Lap::getSummary(std::ostream& os) const
{
	os << "Lap num: " << this->_lapNum;
	os << "  point first: " << this->_firstPointId;
	os << "  point last: " << this->_lastPointId;
	os << "  duration: " << durationAsString(this->_duration);
	os << "  duration_sec: " << this->_duration;
	os << "  distance: " << (double)this->_distance / 1000 << " km";
	os << "  calories: " << this->_calories << " kcal";
	os << "  AvgHr: " << this->_avg_hr;
	os << "  MaxHr: " << this->_max_hr;
	os << "  AvgV: " << this->_avg_speed << " km/h";
	os << "  MaxV: " << this->_max_speed << " km/h";
	os << "  descent: " << this->_descent;
	os << "  ascent: " << this->_ascent;
}

std::ostream& operator<<(std::ostream& os, const Lap& lap) {
	lap.getSummary(os);
	return os;
}

