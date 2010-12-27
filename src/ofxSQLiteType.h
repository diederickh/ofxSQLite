#ifndef OFXSQLITETYPEH
#define OFXSQLITETYPEH
#include <string>

enum {
 	 OFX_SQLITE_TYPE_INT
	,OFX_SQLITE_TYPE_DOUBLE
	,OFX_SQLITE_TYPE_LONG
	,OFX_SQLITE_TYPE_INT64
	,OFX_SQLITE_TYPE_TEXT
	,OFX_SQLITE_TYPE_NULL
};

class ofxSQLiteType {
public:
	virtual std::string getString()		{	return	"";		}
	virtual int getInt()				{ 	return	0;		}	
	virtual long getLong()				{ 	return	0;		}
	virtual uint64_t getUint64()		{	return  0;		}
	virtual int getType() = 0;
};
#endif
