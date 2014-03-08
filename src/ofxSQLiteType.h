#pragma once


#include <string>
#include "sqlite/sqlite3.h"
#include "ofFileUtils.h"


class ofxSQLiteValue
{
public:
    enum DataType
    {
        OFX_SQLITE_TYPE_BLOB,
        OFX_SQLITE_TYPE_DOUBLE,
        OFX_SQLITE_TYPE_INT,
        OFX_SQLITE_TYPE_INT64,
        OFX_SQLITE_TYPE_NULL,
        OFX_SQLITE_TYPE_TEXT,
    };

    virtual ofBuffer getBlob() const { return ofBuffer(); }
    virtual double getDouble() const { return 0; }
    virtual int getInt() const { return	0; }
    virtual sqlite3_int64 getInt64() const { return 0; }
    virtual std::string getText() const { return ""; }

    virtual DataType getType() const = 0;
};
