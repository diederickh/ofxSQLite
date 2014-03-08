#pragma once


#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "ofxSQLiteType.h"


class ofxSQLiteTypeNow: public ofxSQLiteValue
{
public:
	virtual std::string getString() const
    {
		time_t now = time(0);	
		std::stringstream strm;
		strm << now;
		return strm.str();
	}
	
	virtual DataType getType() const
    {
		return OFX_SQLITE_TYPE_TEXT;
	}
    
};
 