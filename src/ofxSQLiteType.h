#pragma once

#include <string>
#include <inttypes.h>

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
	virtual std::string getString()	const { return	""; }
	virtual int getInt() const { return	0; }
	virtual long getLong() const { return 0; }
	virtual uint64_t getUint64() const { return 0; }

	virtual int getType() const = 0;
};
