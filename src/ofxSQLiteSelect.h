#pragma once


#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "sqlite/sqlite3.h"
#include "ofLog.h"
#include "ofxSQLiteFieldValues.h"
#include "ofxSQLiteWhere.h"


struct Join
{
	std::string table;
	std::string condition;
	std::string fields;
};

struct Order
{
	std::string field;
	std::string order;
};

class ofxSQLiteSelect
{
public:
    ofxSQLiteSelect(sqlite3* pSQLite);

    // create select query
    ofxSQLiteSelect& select(const std::string& sFields);
    ofxSQLiteSelect& from(const std::string& sFrom);
    ofxSQLiteSelect& join(const std::string& sTable,
                          const std::string& sOnField,
                          const std::string& sFields);

    // where clause..
    template<typename T>
    ofxSQLiteSelect& where(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE);
    }
    
    template<typename T>
    ofxSQLiteSelect& orWhere(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_OR);
    }
    
    template<typename T>
    ofxSQLiteSelect& orLike(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_OR_LIKE);
    }
            
    template<typename T>
    ofxSQLiteSelect& andWhere(const std::string& sField, const T& mValue) {
        return where(sField, mValue, Where::WHERE_AND);
    }
    
    template<typename T>
    ofxSQLiteSelect& where(const std::string& sField, const T& mValue, Where::Type nType)
{
        wheres.where(sField, mValue, nType);
        return *this;
    }
    
    ofxSQLiteSelect& whereNull(const std::string& sField) {
        wheres.whereNull(sField);
        return *this;
    }

    ofxSQLiteSelect& limit(int nCount, int nOffset);
    ofxSQLiteSelect& limit(int nCount);

    ofxSQLiteSelect& order(const std::string& sField);
    ofxSQLiteSelect& order(const std::string& sField, const std::string& sOrder);

    // execution
    ofxSQLiteSelect& execute();
    std::string getLiteralQuery(bool bFillValues = false);
    bool hasRow();

    // iterating
    ofxSQLiteSelect& begin();
    int next();
    bool hasNext();

    // fetching values.
    int getNumColumns();
    string getColumnName(int nColumNum);
    std::string getString(int nIndex = -1);
    int getInt(int nIndex = -1);
    float getFloat(int nIndex = -1);
    double getDouble(int nIndex = -1);

    std::string getResultAsAsciiTable();
private:
    std::vector<Join> joins;
    ofxSQLiteWhere wheres;
    std::vector<Order> orders;

    ofxSQLiteFieldValues where_values;
    std::string fields;
    std::string from_table;
    sqlite3* sqlite;
    sqlite3_stmt* statement;
    int limit_offset;
    int limit_count;
    int col_index;
    int last_result;
};
