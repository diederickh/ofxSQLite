#pragma once

#include <vector>
#include "ofxSQLiteFieldValues.h"
#include <string>


class sqlite3;


class ofxSQLiteInsert {
public:
    ofxSQLiteInsert(sqlite3* pSQLite, const std::string& sTable);
    std::string getLiteralQuery();
    ofxSQLiteFieldValues getFields();

    template<typename T>
    ofxSQLiteInsert& use(const std::string& sField, const T& sValue) {
        field_values.use(sField, sValue);
        return *this;
    }
    int execute();

private:
    std::string table;
    sqlite3* sqlite;
    int pair_count;
    ofxSQLiteFieldValues field_values;
};
