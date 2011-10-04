#include "ofxSQLiteFieldValues.h"
#include <iostream>
#include <sstream>

ofxSQLiteFieldValues::ofxSQLiteFieldValues():_index(0) {
}

int ofxSQLiteFieldValues::use(std::string sField, int nValue) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_INT;
	field.value_int		= nValue;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}

int ofxSQLiteFieldValues::use(std::string sField, uint64_t nValue) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_INT64;
	field.value_uint64	= nValue;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}

int ofxSQLiteFieldValues::use(std::string sField, long nValue) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_LONG;
	field.value_long		= nValue;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}

int ofxSQLiteFieldValues::use(std::string sField, unsigned long nValue) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_INT64;
	field.value_long		= nValue;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}

int ofxSQLiteFieldValues::use(std::string sField, std::string sValue) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_TEXT;
	field.value_string	= sValue;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}

int ofxSQLiteFieldValues::use(std::string sField, ofxSQLiteType& oValue) {
	switch(oValue.getType()) {
		case OFX_SQLITE_TYPE_INT64: return use(sField, oValue.getUint64()); break;
		case OFX_SQLITE_TYPE_LONG: return use(sField, oValue.getLong());break;
		case OFX_SQLITE_TYPE_INT: return use(sField, oValue.getInt()); break;
		case OFX_SQLITE_TYPE_TEXT: return use(sField,oValue.getString()); break;
		default: break;
	};
	return 0; 
}


int ofxSQLiteFieldValues::use(std::string sField, double nValue) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_DOUBLE;
	field.value_double	= nValue;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}

int ofxSQLiteFieldValues::use(std::string sField) {
	FieldValuePair field;
	field.field			= sField;
	field.type 			= OFX_SQLITE_TYPE_NULL;
	field.index 			= nextFieldIndex();
	field_values.push_back(field);
	return field_values.size()-1;
}


int ofxSQLiteFieldValues::use(std::string sField, float nValue) {
	return use(sField, (double)nValue);
	
}

void ofxSQLiteFieldValues::begin() {
	_index = 0;
}

bool ofxSQLiteFieldValues::hasNext() {
	return (_index < field_values.size());
}

void ofxSQLiteFieldValues::next() {
	if (hasNext()) _index++;
}

FieldValuePair ofxSQLiteFieldValues::current() {
	return field_values[_index];
}

FieldValuePair& ofxSQLiteFieldValues::at(int nIndex) {
	return field_values[nIndex];
}

int ofxSQLiteFieldValues::size() {
	return field_values.size();
}

void ofxSQLiteFieldValues::bind(sqlite3_stmt* pStatement) {
	begin();
	while(hasNext()) {
		current().bind(pStatement);
		next();
	}
}

int ofxSQLiteFieldValues::nextFieldIndex() {
	static int field_count = 1;
	field_count++;
	if(field_count >= 999) // you can only insert 999 columns
		field_count = 1;
	return field_count;
}
