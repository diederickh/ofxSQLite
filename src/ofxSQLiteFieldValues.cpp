#include "ofxSQLiteFieldValues.h"
#include <iostream>
#include <sstream>

ofxSQLiteFieldValues::ofxSQLiteFieldValues():
    _index(0),
    _field_count(1)
{
}

std::size_t ofxSQLiteFieldValues::use(const std::string& sField,
                                      const ofxSQLiteValue& oValue)
{
    switch(oValue.getType())
    {
		case ofxSQLiteValue::OFX_SQLITE_TYPE_BLOB:
            return use(sField, oValue.getBlob());
		case ofxSQLiteValue::OFX_SQLITE_TYPE_DOUBLE:
            return use(sField, oValue.getDouble());
		case ofxSQLiteValue::OFX_SQLITE_TYPE_INT:
            return use(sField, oValue.getInt());
		case ofxSQLiteValue::OFX_SQLITE_TYPE_INT64:
            return use(sField, oValue.getInt64());
		case ofxSQLiteValue::OFX_SQLITE_TYPE_NULL:
            return use(sField);
		case ofxSQLiteValue::OFX_SQLITE_TYPE_TEXT:
            return use(sField, oValue.getText());
	}
}


std::size_t ofxSQLiteFieldValues::use(const std::string& sField,
                                      const ofBuffer& blob)
{
	FieldValuePair field;
	field.field			= sField;
	field.type 			= ofxSQLiteValue::OFX_SQLITE_TYPE_INT;
	field.value_blob	= blob;
	field.index 	    = nextFieldIndex();
	_field_values.push_back(field);
	return _field_values.size() - 1;
}


std::size_t ofxSQLiteFieldValues::use(const std::string& sField, double nValue)
{
	FieldValuePair field;
	field.field			= sField;
	field.type 			= ofxSQLiteValue::OFX_SQLITE_TYPE_DOUBLE;
	field.value_double	= nValue;
	field.index 		= nextFieldIndex();
	_field_values.push_back(field);
	return _field_values.size() - 1;
}


std::size_t ofxSQLiteFieldValues::use(const std::string& sField,
                                      int nValue)
{
	FieldValuePair field;
	field.field			= sField;
	field.type 			= ofxSQLiteValue::OFX_SQLITE_TYPE_INT;
	field.value_int		= nValue;
	field.index 	    = nextFieldIndex();
	_field_values.push_back(field);
	return _field_values.size() - 1;
}


std::size_t ofxSQLiteFieldValues::use(const std::string& sField,
                                      sqlite3_int64 nValue)
{
	FieldValuePair field;
	field.field			= sField;
	field.type 			= ofxSQLiteValue::OFX_SQLITE_TYPE_INT64;
	field.value_int64	= nValue;
	field.index 	    = nextFieldIndex();
	_field_values.push_back(field);
	return _field_values.size() - 1;
}


std::size_t ofxSQLiteFieldValues::use(const std::string& sField) {
	FieldValuePair field;
	field.field	= sField;
	field.type 	= ofxSQLiteValue::OFX_SQLITE_TYPE_NULL;
	field.index = nextFieldIndex();
	_field_values.push_back(field);
	return _field_values.size() - 1;
}


std::size_t ofxSQLiteFieldValues::use(const std::string& sField,
                                      const std::string& sValue)
{
	FieldValuePair field;
	field.field			= sField;
	field.type 			= ofxSQLiteValue::OFX_SQLITE_TYPE_TEXT;
	field.value_text	= sValue;
	field.index 		= nextFieldIndex();
	_field_values.push_back(field);
	return _field_values.size() - 1;
}


void ofxSQLiteFieldValues::begin()
{
	_index = 0;
}


bool ofxSQLiteFieldValues::hasNext() const
{
	return _index < _field_values.size();
}

void ofxSQLiteFieldValues::next()
{
	if (hasNext())
    {
        _index++;
    }
}

FieldValuePair ofxSQLiteFieldValues::current() const {
	return _field_values[_index];
}

FieldValuePair& ofxSQLiteFieldValues::at(std::size_t nIndex) {
	return _field_values[nIndex];
}

const FieldValuePair& ofxSQLiteFieldValues::at(std::size_t nIndex) const {
	return _field_values[nIndex];
}

std::size_t ofxSQLiteFieldValues::size() const {
	return _field_values.size();
}

void ofxSQLiteFieldValues::bind(sqlite3_stmt* pStatement) {
	begin();

    while(hasNext())
    {
		current().bind(pStatement);
		next();
	}
}

int ofxSQLiteFieldValues::nextFieldIndex() {
	_field_count++;

    // field_limit = sqlite3_limit(db, SQLITE_LIMIT_VARIABLE_NUMBER, -1);

	if(_field_count >= 999) // you can only insert 999 columns
    {
        _field_count = 1;
    }

	return _field_count;
}
