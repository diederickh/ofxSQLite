#ifndef OFXSQLITEFIELDVALUESH
#define OFXSQLITEFIELDVALUESH

#include <vector>
#include <string>
#include <sstream>
#include "lib/sqlite/sqlite3.h"
#include "ofxSQLiteType.h"
#include "ofxSQLiteTypeNow.h"
#include <iostream>
#include <stdio.h>


struct FieldValuePair {
	std::string field;
	std::string value_string;
	int value_int;
	double value_double;
	long value_long;
	uint64_t value_uint64;
	int index;
	int type;

	void bind(sqlite3_stmt* pStatement) {
		int result = SQLITE_OK;
		switch(type) {
			case OFX_SQLITE_TYPE_INT:		result = sqlite3_bind_int(pStatement, index, value_int);break;
			case OFX_SQLITE_TYPE_LONG:		result = sqlite3_bind_int64(pStatement,index, value_long); break;
			case OFX_SQLITE_TYPE_INT64:		result = sqlite3_bind_int64(pStatement,index, value_uint64); break;
			case OFX_SQLITE_TYPE_TEXT: {
				result = sqlite3_bind_text(pStatement, index, value_string.c_str(), value_string.size(), SQLITE_STATIC); break;
			}
			case OFX_SQLITE_TYPE_DOUBLE:	result = sqlite3_bind_double(pStatement, index, value_double);break;
			default:break;
		}

		if(result != SQLITE_OK) {
			printf("SQLITE: Error while binding parameter: %d\n", index);
		}
	}

	std::string indexString() {
		std::stringstream ss;
		ss << index;
		return ss.str();
	}
};

class ofxSQLiteFieldValues {
	public:
		ofxSQLiteFieldValues();
		void use(std::string sField, int nValue);
		void use(std::string sField, float nValue);
		void use(std::string sField, long nValue);
		void use(std::string sField, uint64_t nValue);
		void use(std::string sField, double nValue);
		void use(std::string sField, std::string sValue);
		void use(std::string sField, ofxSQLiteType& oValue);
		void use(std::string sField);
		void bind(sqlite3_stmt* pStatement);

		FieldValuePair at(int nIndex);
		void begin();
		FieldValuePair current();
		void next();
		bool hasNext();
		int size();

	private:
		std::vector<FieldValuePair> field_values;
		int nextFieldIndex();
		int _index;
};

#endif
