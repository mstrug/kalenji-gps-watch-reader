#include "Session.h"
#include "../Utils.h"
#include <iostream>

void Session::getSummary(std::ostream& os) const
{
	os << std::setw(5) << this->getNum() << " - " << this->getBeginTime(true);
	os << " " << std::setw(5) << this->getNbLaps() << " laps ";
	os << std::setw(10) << (double)this->getDistance() / 1000 << " km ";
	os << std::setw(15) << durationAsString(this->getDuration());
}

void Session::getSummaryLong(std::ostream& os) const
{
	os << std::setw(4) << this->getNum() << " - " << this->getBeginTime(true);
	os << " " << std::setw(2) << this->getNbLaps() << " laps ";
	os << std::setw(6) << (double)this->getDistance() / 1000 << " km ";
	os << std::setw(8) << durationAsString(this->getDuration());
	os << std::setw(8) << " Points: " << this->_nb_points;

	os << std::setw(5) << "  AvgV: " << this->_avg_speed << " km/h";
	os << std::setw(5) << "  MaxV: " << this->_max_speed << " km/h";
	os << std::setw(5) << "  AvgHR: " << this->_avg_hr << " bpm";
	os << std::setw(5) << "  MaxHR: " << this->_max_hr << " bpm";
	os << std::setw(5) << "  Cal: " << this->_calories << " kcal";
	os << std::setw(5) << "  Asc: " << this->_ascent << " m";
	os << std::setw(5) << "  Desc: " << this->_descent << " m";
	os << std::setw(5) << "  MinAlt: " << this->_min_alt << " m";
	os << std::setw(5) << "  MaxAlt: " << this->_max_alt << " m";
	os << std::setw(5) << "  AvgCad: " << this->_avg_cad;
	os << std::setw(5) << "  MaxCad: " << this->_max_cad;
}

std::ostream& operator<<(std::ostream& os, const Session& session) {
	session.getSummaryLong(os);
	return os;
}
