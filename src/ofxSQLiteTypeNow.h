#ifndef SQLITETYPENOWH
#define SQLITETYPENOWH
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "ofxSQLiteType.h"
class ofxSQLiteTypeNow : public ofxSQLiteType {
public:
	virtual std::string getString() {
		time_t now = time(0);	
		std::stringstream strm;
		strm << now;
		return strm.str();
	}
	
	virtual int getType() {
		return OFX_SQLITE_TYPE_TEXT;
	}
};
#endif

 