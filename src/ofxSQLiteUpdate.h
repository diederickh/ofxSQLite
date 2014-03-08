#pragma once


#include <vector>
#include <string>
#include "ofxSQLiteWhere.h"
#include "ofxSQLiteFieldValues.h"


class ofxSQLiteFieldValues;
class sqlite3;

class ofxSQLiteUpdate {
public:
    ofxSQLiteUpdate(sqlite3* pSQLite, const std::string& sTable);

    template<typename T>
    ofxSQLiteUpdate& use(const std::string& sField, const T& sValue) {
        field_values.use(sField, sValue);
        return *this;
    }

    std::string getLiteralQuery();
    int execute();

    // where clause..
    template<typename T>
    ofxSQLiteUpdate& where(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE);
    }
    template<typename T>
    ofxSQLiteUpdate& orWhere(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_OR);
    }
    template<typename T>
    ofxSQLiteUpdate& andWhere(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_AND);
    }
    template<typename T>
    ofxSQLiteUpdate& where(const std::string& sField, const T& mValue, Where::Type nType)
    {
        wheres.where(sField, mValue, nType);
        return *this;
    }

private:
    std::string table;
    ofxSQLiteWhere wheres;
    ofxSQLiteFieldValues field_values;
    sqlite3* sqlite;
};
