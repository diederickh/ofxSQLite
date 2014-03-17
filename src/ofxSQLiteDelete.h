#pragma once

#include <vector>
#include <string>
#include "ofxSQLiteWhere.h"
#include "ofxSQLiteFieldValues.h"

class ofxSQLiteFieldValues;
class sqlite3;

class ofxSQLiteDelete {
public:
    ofxSQLiteDelete(sqlite3*  pSQLite, const std::string& sTable);

    template<typename T>
    ofxSQLiteDelete& use(const std::string& sField, T sValue) {
        field_values.use(sField, sValue);
        return *this;
    }

    std::string getLiteralQuery(bool bFillValues = false);

    int execute();

    // where clause..
    template<typename T>
    ofxSQLiteDelete& where(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE);
    }
    template<typename T>
    ofxSQLiteDelete& orWhere(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_OR);
    }
    template<typename T>
    ofxSQLiteDelete& andWhere(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_AND);
    }
    template<typename T>
    ofxSQLiteDelete& where(const std::string& sField,
                           const T& mValue,
                           Where::Type nType)
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
