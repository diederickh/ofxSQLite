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

enum OperatorType {
	 OP_GREATER_THAN
	,OP_GREATER_EQUAL_THAN
	,OP_LESS_THAN
	,OP_LESS_EQUAL_THAN
	,OP_LIKE
	,OP_EQUAL
};

struct FieldValuePair {
	std::string field;
	std::string value_string;
	int value_int;
	double value_double;
	long value_long;
	uint64_t value_uint64;
	int index;
	int type;
	int sql_operator;
	
	void bind(sqlite3_stmt* pStatement) {
		int result = SQLITE_OK;
		switch(type) {
			case OFX_SQLITE_TYPE_INT:	{
				result = sqlite3_bind_int(pStatement, index, value_int);
				break;
			}
			case OFX_SQLITE_TYPE_LONG: {
				result = sqlite3_bind_int64(pStatement,index, value_long); 
				break;
			}
			case OFX_SQLITE_TYPE_INT64: {	
				result = sqlite3_bind_int64(pStatement,index, value_uint64); 
				break;
			}
			case OFX_SQLITE_TYPE_TEXT: {
				result = sqlite3_bind_text(
						 	pStatement
						 	,index
						 	,value_string.c_str()
						 	,value_string.size()
						 	,SQLITE_STATIC
				); 
				break;
			}
			case OFX_SQLITE_TYPE_DOUBLE: {	
				result = sqlite3_bind_double(pStatement, index, value_double);
				break;
			}
			default:break;
		}

		if(result != SQLITE_OK) {
			printf("SQLITE: Error while binding parameter: %d\n", index);
		}
	}
	
	void setOperatorType(int op) {
		sql_operator = op;
	}
			  
	std::string getFieldAndValueForQuery(bool embedValue = false) {
		std::string result = "";
		std::string value_part = "";
		
		// create value part.
		if(embedValue) {
			value_part = "\"" +valueString() +"\"";
		}
		else {
			value_part = " ?" +indexString() +" ";
		}
		
		// create field + value part.
		switch(sql_operator) {
			case OP_GREATER_THAN: {
				result = field +" > " +value_part;
				break;
			}
			case OP_GREATER_EQUAL_THAN: {
				result = field +" >= "  +value_part;
				break;
			}
			case OP_LESS_THAN: {
				result = field +" < " +value_part;
				break;
			}
			case OP_LESS_EQUAL_THAN: {
				result = field +" <= " +value_part;
				break;
			}
			case OP_EQUAL:{
				result = field +" = " +value_part;	
				break;	
			}
			case OP_LIKE: {
				result = field + " LIKE " +value_part;
				break;
			}
			default:{
				printf("Unhandled operator type %d for field: %s.\n", sql_operator, field.c_str());
				break;
			}
		}
		return result;
	}

	std::string indexString() {
		std::stringstream ss;
		ss << index;
		return ss.str();
	}
	
	std::string valueString() {
		std::string result = "";
		switch(type) {
			case OFX_SQLITE_TYPE_INT: {
				std::stringstream ss;					
				ss << value_int;
				result = ss.str();
				break;
			}
			case OFX_SQLITE_TYPE_LONG: {
				std::stringstream ss;					
				ss << value_long;
				result = ss.str();
				break;
			}
			case OFX_SQLITE_TYPE_INT64: {
				std::stringstream ss;					
				ss << value_uint64;
				result = ss.str();
				break;
			}
			case OFX_SQLITE_TYPE_TEXT: {
				result = value_string;
				break;
			}
		}
		return result;
	}
};

class ofxSQLiteFieldValues {
	public:
		ofxSQLiteFieldValues();
		int use(std::string sField, int nValue);
		int use(std::string sField, unsigned long nValue);
		int use(std::string sField, uint64_t nValue );
		int use(std::string sField, float nValue);
		int use(std::string sField, long nValue);
		int use(std::string sField, double nValue);
		int use(std::string sField, std::string sValue);
		int use(std::string sField, ofxSQLiteType& oValue);
		int use(std::string sField);
		void bind(sqlite3_stmt* pStatement);

		FieldValuePair& at(int nIndex);
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
