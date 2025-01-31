#ifndef _BOM_POINT_H
#define _BOM_POINT_H

#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include "../Utils.h"
#include "../bom/Field.h"


class Point
{
	public:
		// TODO: Improve the way fiability is handled (0 / 3 doesn't make sense)
		Point() : _time(0), _millis(0), _lat(FieldUndef), _lon(FieldUndef), _alt(FieldUndef), _speed(FieldUndef), _bpm(FieldUndef), _cadence(FieldUndef), _power(FieldUndef), _fiability(3), _important(false), _distance(FieldUndef)
		{ };

		Point(Field<double> lat, Field<double> lon, Field<int16_t> alt, Field<double> speed, time_t time, uint32_t millis, Field<uint16_t> bpm, uint16_t fiability)
			: _time(time), _millis(millis), _lat(lat), _lon(lon), _alt(alt), _speed(speed), _bpm(bpm), _cadence(FieldUndef), _power(FieldUndef), _fiability(fiability), _important(false), _distance(FieldUndef)
		{ };

		void setLatitude(const Field<double>& lat)        { _lat = lat; };
		void setLongitude(const Field<double>& lon)       { _lon = lon; };
		void setAltitude(const Field<int16_t>& alt)       { _alt = alt; };
		void setSpeed(const Field<double>& speed)         { _speed = speed; };
		void setHeartRate(const Field<uint16_t>& bpm)     { _bpm = bpm; };
		void setCadence(uint16_t c) 			  { _cadence = c; }
		void setPower(uint16_t p) 			  { _power = p; }
		void setFiability(uint16_t f)                     { _fiability = f; };
		void setImportant(bool i)                         { _important = i; };
		void setTime(time_t time)                         { _time = time; };
		void setDistance(const Field<uint32_t>& distance) { _distance = distance;}
		
		const Field<double>& getLatitude() const    { return _lat; };
		const Field<double>& getLongitude() const   { return _lon; };
		const Field<int16_t>& getAltitude() const   { return _alt; };
		const Field<double>& getSpeed() const       { return _speed; };
		const Field<uint16_t>& getHeartRate() const { return _bpm; };
		const Field<uint16_t>& getCadence() const   { return _cadence; };
		const Field<uint16_t>& getPower() const     { return _power; };
		uint16_t getFiability() const               { return _fiability; };
		bool isImportant() const                    { return _important; };
		time_t getTime() const                      { return _time; };
		const Field<uint32_t> getDistance() const   { return _distance;}
		// TODO: Is it really the right place ? We may want to do it with any time ! To move in a "utils" part
		const std::string getTimeAsString(bool human_readable=false, bool local=false) const
		{
			char buffer[256];
			tm time_tm;
			if(local)
			{
				localtime_r(&_time, &time_tm);
			}
			else
			{
				gmtime_r(&_time, &time_tm);
			}
			std::stringstream format_string;
			if(human_readable)
				format_string << "%Y-%m-%d %H:%M:%S." << std::setw(3) << std::setfill('0') << _millis;
			else
				format_string << "%Y-%m-%dT%H:%M:%S." << std::setw(3) << std::setfill('0') << _millis << "Z";
			strftime(buffer, 256, format_string.str().c_str(), &time_tm);
			return std::string(buffer);
		};

		double distanceFromInMeters(Point &p) const
		{
			double lat1 = this->_lat.getValue();
			double lon1 = this->_lon.getValue();
			double lat2 = p._lat.getValue();
			double lon2 = p._lon.getValue();

			if ( lat1 == lat2 && lon1 == lon2 ) 
			{
				return 0;
			}

			double pi = 3.14159265358979323846;

			double dist = sin(lat1 * pi / 180) * sin(lat2 * pi / 180) + cos(lat1 * pi / 180) * cos(lat2 * pi / 180) * cos((lon1 - lon2) * pi / 180);
			dist = acos(dist) * 6371 * 1000;
			return dist;
		};

	private:
		time_t   _time;
		uint32_t _millis;
		Field<double>   _lat;
		Field<double>   _lon;
		Field<int16_t>  _alt;
		Field<double>   _speed;
		Field<uint16_t> _bpm;
		Field<uint16_t> _cadence;
		Field<uint16_t> _power;
		uint16_t _fiability;
		bool _important;
		Field<uint32_t> _distance; //Distance in meter from the begining: if -1, must be computed by the session see Session::getDistanceForPoint(Point point);
};

#endif
