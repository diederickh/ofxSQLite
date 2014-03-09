#pragma once


#include <vector>
#include <string>
#include "sqlite/sqlite3.h"
#include "ofLog.h"
#include "ofUtils.h"
#include "ofxSQLiteTypeNow.h"
#include "ofxSQLiteInsert.h"
#include "ofxSQLiteUpdate.h"
#include "ofxSQLiteDelete.h"
#include "ofxSQLiteSelect.h"
#include "ofxSQLiteType.h"
#include "ofxSQLiteTypeNow.h"


class ofxSQLiteSimpler;


class ofxSQLite
{
public:
    ofxSQLite();
    bool setup(const std::string& sDB);

    bool isLoaded() const;

    ofxSQLiteInsert insert(const std::string& sTable);
    ofxSQLiteUpdate update(const std::string& sTable);
    ofxSQLiteDelete remove(const std::string& sTable);
    ofxSQLiteSelect select(const std::string& sFields);

    void printTable(const std::string& sTable);

    ofxSQLiteSimpler operator[](const std::string& sKeyValue);
    
    sqlite_int64 lastInsertID() const;
    std::string getError() const;

    int simpleQuery(const std::string& SQL);

    bool isThreadsafe() const;
    std::string getVersion() const;
    std::string getSourceId() const;
    int getVersionNumber() const;

    ofxSQLiteTypeNow now() const;

private:
    sqlite3* db;
    std::string db_file;

};
