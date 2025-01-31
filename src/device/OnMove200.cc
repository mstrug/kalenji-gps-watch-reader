#include "OnMove200.h"
#include <cstring>
#include <iomanip>
#include <set>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#include <dirent.h>
#include <sys/stat.h>

#ifdef DEBUG
#define DEBUG_CMD(x) x;
#else
#define DEBUG_CMD(x) ;
#endif

namespace device
{
	REGISTER_DEVICE(OnMove200);

	int OnMove200::bytesToInt2(unsigned char b0, unsigned char b1)
	{
		int Int = b0 | ( (int)b1 << 8 );
		return Int;
	}

	int OnMove200::bytesToInt4(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3)
	{
		int Int = b0 | ( (int)b1 << 8 ) | ( (int)b2 << 16 ) | ( (int)b3 << 24 );
		return Int;
	}

	unsigned char* OnMove200::readAllBytes(const std::string& filename, size_t& size)
	{
		std::ifstream fl(filename.c_str());
		fl.seekg( 0, std::ios::end );
		size_t len = fl.tellg();
		auto buffer = new char[len];
		fl.seekg(0, std::ios::beg);
		fl.read(buffer, len);
		fl.close();
		size = len;
		return (unsigned char*)buffer;
	}

	bool OnMove200::fileExists(const std::string& filename)
	{
		struct stat fileInfo;
		return stat(filename.c_str(), &fileInfo) == 0;
	}

	void OnMove200::dump(unsigned char *data, int length)
	{
		DEBUG_CMD(std::cout << std::hex);
		for(int i = 0; i < length; ++i)
		{
			DEBUG_CMD(std::cout << std::setw(2) << std::setfill('0') << (int) data[i] << " ");
		}
		DEBUG_CMD(std::cout << std::endl);
	}

	void OnMove200::init(const DeviceId& deviceId)
	{
		//check if getPath() is a valid path
		DIR* folder = opendir(getPath().c_str());
		if (folder == nullptr)
		{
			std::cout<< "Error: path '" << getPath() << "' does not exist (check option -p <path> on command line or line path=<path> in configuration file)." << std::endl;
			throw std::exception();
		}
		closedir(folder);
	}

	void OnMove200::release()
	{
		DEBUG_CMD(std::cout << "OnMove200: Release (nothing to do)" << std::endl);
	}

	std::string OnMove200::getPath()
	{
		return _configuration["path"];
	}

	void OnMove200::getSessionsList(SessionsMap *oSessions)
	{
		LOG_VERBOSE("OnMove200: Retrieve sessions list from '" << getPath() << "'");

		DIR* folder = nullptr;
		struct dirent* file = nullptr;
		folder = opendir(getPath().c_str());
		if (folder == nullptr)
		{
			std::cerr << "Couldn't open dir " << getPath() << std::endl;
			throw std::exception();
		}

		std::set<std::string> filenamesPrefix;

		while ((file = readdir(folder)) != nullptr)
		{
			std::string fn = std::string(file->d_name);
			if(strstr(file->d_name,".OMD") || strstr(file->d_name,".OMH"))
			{
				std::string fileprefix = fn.substr(0,fn.length() - 4);
				std::string filepathprefix = getPath() + std::string("/") + fileprefix;
				//check if both files (header and data) exists
				if(fileExists(filepathprefix + ".OMD") && fileExists(filepathprefix + ".OMH"))
				{
					filenamesPrefix.insert(fileprefix);
				}
				else
				{
					std::cout << "Discarding " << fileprefix << std::endl;
				}
			}
		}

		closedir(folder);

		int i = 0;
		std::set<std::string>::iterator iter;
		for (iter=filenamesPrefix.begin(); iter!=filenamesPrefix.end(); ++iter)
		{
			std::string fileprefix = *iter;

			DEBUG_CMD(std::cout << "Decode summary of session " << fileprefix << std::endl);
			// Decoding of basic info about the session
			std::vector<char> _sessionId(fileprefix.begin(), fileprefix.end());
			SessionId id = SessionId(_sessionId);
			uint32_t num = i++; //Just increment by one each time

			tm time;
			memset(&time, 0, sizeof(time));

			double duration = 0;
			uint32_t distance = 0;
			uint32_t nbLaps = 999;

			Session mySession(id, num, time, 0, duration, distance, nbLaps);

			// Properly fill necessary session info (duration, distance, nbLaps)
			std::string ghtFilename = getPath() + std::string("/") + fileprefix + std::string(".OMH");
			size_t size = -1;
			unsigned char* buffer = readAllBytes(ghtFilename, size);
			// TODO: if(size != 60) error !
			parseOMHFile(buffer, &mySession);
			delete buffer;

			oSessions->insert(SessionsMapElement(id, mySession));
		}
	}

	void OnMove200::getSessionsDetails(SessionsMap *oSessions)
	{
		for(auto& sessionPair : *oSessions)
		{
			Session* session = &(sessionPair.second);
			SessionId sessionId = sessionPair.second.getId();
			std::string filenamePrefix(sessionId.begin(),sessionId.end());
			std::cout << "Retrieve session " << filenamePrefix << std::endl;

			filenamePrefix = getPath() + std::string("/") + filenamePrefix;

			unsigned char* buffer;
			size_t size = -1;

			std::string omdFilename = filenamePrefix + std::string(".OMD");
			buffer = readAllBytes(omdFilename,size);
			parseOMDFile(buffer, size, session);
			delete buffer;
		}
	}

	void OnMove200::dumpInt2(std::ostream &oStream, unsigned int iInt)
	{
		oStream << (char)(iInt & 0xFF) << (char)((iInt & 0xFF00) >> 8);
	}

	void OnMove200::dumpInt4(std::ostream &oStream, unsigned int iInt)
	{
		oStream << (char)(iInt & 0xFF) << (char)((iInt & 0xFF00) >> 8) << (char)((iInt & 0xFF0000) >> 16) << (char)((iInt & 0xFF000000) >> 24);
	}

	void OnMove200::dumpString(std::ostream &oStream, const std::string &iString, size_t iLength)
	{
		size_t toCopy = iString.length();
		if(iLength <= toCopy) toCopy = iLength - 1;
		oStream.write(iString.c_str(), toCopy);
		for(size_t i = toCopy; i < iLength; ++i)
		{
			oStream.put('\0');
		}
	}

	void OnMove200::exportSession(const Session *iSession)
	{
		std::cerr << "Unsupported export session for OnMove200" << std::endl;
	}

	void OnMove200::parseOMHFile(const unsigned char* bytes, Session* session)
	{
		uint32_t distance = bytesToInt4(bytes[0], bytes[1], bytes[2], bytes[3]);
		uint32_t duration = bytesToInt2(bytes[4], bytes[5]);
		uint32_t avgSpeed = bytesToInt2(bytes[6], bytes[7]);
		uint32_t maxSpeed = bytesToInt2(bytes[8], bytes[9]);
		uint32_t energy = bytesToInt2(bytes[10], bytes[11]);
		uint32_t avgHeartRate = static_cast<uint32_t>(bytes[12]);
		uint32_t maxHeartRate = static_cast<uint32_t>(bytes[13]);

		uint32_t year = static_cast<uint32_t>(bytes[14]);
		uint32_t month = static_cast<uint32_t>(bytes[15]);
		uint32_t day = static_cast<uint32_t>(bytes[16]);
		uint32_t hour = static_cast<uint32_t>(bytes[17]);
		uint32_t minute = static_cast<uint32_t>(bytes[18]);
		//uint32_t fileNum = static_cast<uint32_t>(bytes[19]);

		tm time;
		memset(&time, 0, sizeof(time));
		time.tm_year  = 100 + year;// In tm, year is year since 1900. GPS returns year since 2000
		time.tm_mon   = month - 1;// In tm, month is between 0 and 11.
		time.tm_mday  = day;
		time.tm_hour  = hour;
		time.tm_min   = minute;
		time.tm_isdst = -1;
		session->setTime(time);
		session->setDistance(distance);
		session->setDuration(duration);
		session->setAvgSpeed(avgSpeed);
		session->setMaxSpeed(maxSpeed);
		session->setCalories(energy);
		session->setAvgHr(avgHeartRate);
		session->setMaxHr(maxHeartRate);
	}

	void OnMove200::parseOMDFile(const unsigned char* bytes, int length, Session *session)
	{
		const unsigned char* chunk;
		int numPoints = 0;
		time_t startTime = session->getTimeT();
		// We remove 20 bytes from the length because the last line is always a 'metadata' one,
		// even if there's been only one 'data' line since the last one.
		for(int i = 0; i < (length-20); i += 20)
		{
			numPoints++;
			// Every other 3 line doesn't contain coordinates ('metadata')
			if(numPoints % 3 == 0) continue;
			chunk = &bytes[i];
			double latitude = ((double) bytesToInt4(chunk[0], chunk[1], chunk[2], chunk[3])) / 1000000.;
			double longitude = ((double) bytesToInt4(chunk[4], chunk[5], chunk[6], chunk[7])) / 1000000.;
			// Not sure if distance is really on 4 bytes or only on 2, but 2 would seem limited (65 km, can be short for a bike session)
			uint32_t distance = bytesToInt4(chunk[8], chunk[9], chunk[10], chunk[11]);
			uint32_t time = bytesToInt2(chunk[12], chunk[13]);
			// Heart rate for points of lines n and n+1 are on line n+2 (the every other 3 line that doesn't contain coordinates)
			uint32_t hr = chunk[46];
			if(numPoints % 3 == 2) hr = chunk[36];
			auto p = new Point(latitude, longitude, FieldUndef, FieldUndef, startTime + time, 0, hr, 3);
			p->setDistance(distance);
			session->addPoint(p);
		}
	}
}
