#ifndef _BOM_LAP_H
#define _BOM_LAP_H

#include "Point.h"

class Lap
{
	public:
		Lap() : _lapNum(0),
		        _firstPointId(0),       _lastPointId(0),
			_start(nullptr),           _end(nullptr),
			_duration(0),           _distance(0),
			_max_speed(FieldUndef), _avg_speed(FieldUndef),
			_max_hr(FieldUndef),    _avg_hr(FieldUndef),
			_calories(FieldUndef),  _grams(FieldUndef),
			_ascent(FieldUndef),    _descent(FieldUndef),
			_min_alt(FieldUndef),   _max_alt(FieldUndef),
			_avg_cad(FieldUndef),   _max_cad(FieldUndef)
		{ };

		Lap(uint32_t firstPointId,    uint32_t lastPointId,
		    double duration,          uint32_t distance,
		    Field<double> max_speed,  Field<double> avg_speed, // NOLINT: incorrectly advise to move Field
		    Field<uint32_t> max_hr,   Field<uint32_t> avg_hr, // NOLINT: incorrectly advise to move Field
		    Field<uint32_t> calories, Field<uint32_t> grams, // NOLINT: incorrectly advise to move Field
		    Field<uint32_t> descent,  Field<uint32_t> ascent) // NOLINT: incorrectly advise to move Field
			: _lapNum(0), _firstPointId(firstPointId), _lastPointId(lastPointId),
				_start(nullptr), _end(nullptr),
				_duration(duration), _distance(distance),
				_max_speed(max_speed), _avg_speed(avg_speed),
				_max_hr(max_hr), _avg_hr(avg_hr),
				_calories(calories), _grams(grams),
				_ascent(ascent), _descent(descent),
				_min_alt(FieldUndef),   _max_alt(FieldUndef),
				_avg_cad(FieldUndef),   _max_cad(FieldUndef)
		{ };

		void setLapNum(uint32_t lapNum)         { _lapNum = lapNum; };
		void setStartPoint(Point *start)        { _start = start; };
		void setEndPoint(Point *end)            { _end = end; };

		void setDuration(double duration)       { _duration = duration; };
		void setDistance(uint32_t distance)     { _distance = distance; };
		void setMaxSpeed(double max_speed)      { _max_speed = max_speed; };
		void setAvgSpeed(double avg_speed)      { _avg_speed = avg_speed; };
		void setMaxHeartrate(uint32_t max_hr)   { _max_hr = max_hr; };
		void setAvgHeartrate(uint32_t avg_hr)   { _avg_hr = avg_hr; };
		void setCalories(uint32_t calories)     { _calories = calories; };
		void setGrams(uint32_t grams)           { _grams = grams; };
		void setAscent(uint32_t ascent)         { _ascent = ascent; };
		void setDescent(uint32_t descent)       { _descent = descent; };
		void setFirstPointId(uint32_t pid)      { _firstPointId = pid; };
		void setLastPointId(uint32_t pid)       { _lastPointId = pid; };
		void setMinAltitude(uint32_t alt)       { _min_alt = alt; };
		void setMaxAltitude(uint32_t alt)     	{ _max_alt = alt; };
		void setAvgCadence(uint32_t cad)   		{ _avg_cad = cad; };
		void setMaxCadence(uint32_t cad)   		{ _max_cad = cad; };

		uint32_t getLapNum() const                     { return _lapNum; };
		uint32_t getFirstPointId() const               { return _firstPointId; };
		uint32_t getLastPointId() const                { return _lastPointId; };
		const Point *getStartPoint() const             { return _start; };
		const Point *getEndPoint() const               { return _end; };

		double getDuration() const                     { return _duration; };
		uint32_t getDistance() const                   { return _distance; };
		const Field<double>& getMaxSpeed() const       { return _max_speed; };
		const Field<double>& getAvgSpeed() const       { return _avg_speed; };
		const Field<uint32_t>& getMaxHeartrate() const { return _max_hr; };
		const Field<uint32_t>& getAvgHeartrate() const { return _avg_hr; };
		const Field<uint32_t>& getCalories() const     { return _calories; };
		const Field<uint32_t>& getGrams() const        { return _grams; };
		const Field<uint32_t>& getAscent() const       { return _ascent; };
		const Field<uint32_t>& getDescent() const      { return _descent; };
		const Field<uint32_t>& getMinAltitude() const  { return _min_alt; };
		const Field<uint32_t>& getMaxAltitude() const  { return _max_alt; };
		const Field<uint32_t>& getAvgCadence() const   { return _avg_cad; };
		const Field<uint32_t>& getMaxCadence() const   { return _max_cad; };

		void getSummary(std::ostream& os) const;

	private:
		uint32_t _lapNum;
		uint32_t _firstPointId;
		uint32_t _lastPointId;
		Point *_start;
		Point *_end;

		// TODO: Add begin time, as using the time of the first point can be very unprecise !
		double _duration;
		uint32_t _distance;
		Field<double> _max_speed;
		Field<double> _avg_speed;
		Field<uint32_t> _max_hr;
		Field<uint32_t> _avg_hr;
		Field<uint32_t> _calories;
		Field<uint32_t> _grams;
		Field<uint32_t> _ascent;
		Field<uint32_t> _descent;
		Field<uint32_t> _min_alt;
		Field<uint32_t> _max_alt;
		Field<uint32_t> _avg_cad;
		Field<uint32_t> _max_cad;
};

std::ostream& operator<<(std::ostream& os, const Lap& lap);

#endif
