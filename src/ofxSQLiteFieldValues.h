#pragma once


#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "sqlite/sqlite3.h"
#include "ofxSQLiteType.h"
#include "ofxSQLiteTypeNow.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofUtils.h"


enum
{
    OP_GREATER_THAN,
    OP_GREATER_EQUAL_THAN,
    OP_LESS_THAN,
    OP_LESS_EQUAL_THAN,
    OP_LIKE,
    OP_EQUAL
};

struct FieldValuePair
{
    // Values
    ofBuffer value_blob;
	double value_double;
	int value_int;
    sqlite3_int64 value_int64;
	std::string value_text;

    // Statements
    std::string field;
	int index;
	int type;

	int sql_operator;
	
	int bind(sqlite3_stmt* pStatement)
    {
		switch(type)
        {
			case ofxSQLiteValue::OFX_SQLITE_TYPE_BLOB:
				return sqlite3_bind_blob(pStatement,
                                         index,
                                         value_blob.getBinaryBuffer(),
                                         value_blob.size(),
                                         SQLITE_STATIC);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_DOUBLE:
				return sqlite3_bind_double(pStatement, index, value_double);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_INT:
				return sqlite3_bind_int(pStatement, index, value_int);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_INT64:
				return sqlite3_bind_int64(pStatement, index, value_int64);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_NULL:
				return sqlite3_bind_null(pStatement, index);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_TEXT:
				return sqlite3_bind_text(pStatement,
                                         index,
                                         value_text.c_str(),
                                         value_text.size(),
                                         SQLITE_STATIC);
		}
	}
	
	void setOperatorType(int op) {
		sql_operator = op;
	}
			  
	std::string getFieldAndValueForQuery(bool embedValue = false) const
    {
		std::string result = "";
		std::string value_part = "";
		
		// create value part.
		if(embedValue)
        {
			value_part = "\"" + valueString() + "\"";
		}
		else
        {
			value_part = " ?" + indexString() + " ";
		}
		
		// create field + value part.
		switch(sql_operator)
        {
			case OP_GREATER_THAN:
				result = field + " > " + value_part;
				break;
			case OP_GREATER_EQUAL_THAN:
            	result = field + " >= "  + value_part;
				break;
			case OP_LESS_THAN:
				result = field + " < " + value_part;
				break;
			case OP_LESS_EQUAL_THAN:
				result = field + " <= " + value_part;
				break;
			case OP_EQUAL:{
				result = field + " = " + value_part;
				break;	
			}
			case OP_LIKE: {
				result = field + " LIKE " + value_part;
				break;
			}
			default:{
				printf("Unhandled operator type %d for field: %s.\n", sql_operator, field.c_str());
				break;
			}
		}

		return result;
	}

	std::string indexString() const {
		return ofToString(index);
	}
	
	std::string valueString() const
    {
		switch(type)
        {
			case ofxSQLiteValue::OFX_SQLITE_TYPE_BLOB:
                return value_blob.getText();
			case ofxSQLiteValue::OFX_SQLITE_TYPE_DOUBLE:
                return ofToString(value_double);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_INT:
                return ofToString(value_int);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_INT64:
                return ofToString(value_int64);
			case ofxSQLiteValue::OFX_SQLITE_TYPE_NULL:
                return "";
			case ofxSQLiteValue::OFX_SQLITE_TYPE_TEXT:
                return value_text;
		}
	}
};

class ofxSQLiteFieldValues {
public:
    ofxSQLiteFieldValues();

    std::size_t use(const std::string& sField, const ofxSQLiteValue& oValue);
    std::size_t use(const std::string& sField, const ofBuffer& blob);
    std::size_t use(const std::string& sField, double nValue);
    std::size_t use(const std::string& sField, int nValue);
    std::size_t use(const std::string& sField, sqlite3_int64 nValue );
    std::size_t use(const std::string& sField);
    std::size_t use(const std::string& sField, const std::string& sValue);

    void bind(sqlite3_stmt* pStatement);

    FieldValuePair& at(std::size_t nIndex);
    const FieldValuePair& at(std::size_t nIndex) const;

    void begin();
    FieldValuePair current() const;
    void next();
    bool hasNext() const;
    std::size_t size() const;

private:
    std::size_t _field_count;

    int nextFieldIndex();
    std::vector<FieldValuePair> _field_values;
    std::size_t _index;
};
