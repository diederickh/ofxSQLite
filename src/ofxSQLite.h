/**
 * When compiling the sqlite.c file I needed to put:
 * SQLITE_ENABLE_UPDATE_DELETE_LIMIT=1
 * in the build options.
 *
 * Or you can use a build define like: SQLITE_DEBUG 0
 *
 */


#pragma once

#define DSQLITE_ENABLE_UPDATE_DELETE_LIMIT 1

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

class ofxSQLite {
public:
    ofxSQLite();
    void setup(const std::string& sDB);
    ofxSQLiteInsert insert(const std::string& sTable);
    ofxSQLiteUpdate update(const std::string& sTable);
    ofxSQLiteDelete remove(const std::string& sTable);
    ofxSQLiteSelect select(const std::string& sFields);
    void printTable(const std::string& sTable);
    
    ofxSQLiteSimpler operator[](const std::string& sKeyValue);
    
    int lastInsertID();
    std::string getError();

    int simpleQuery(const std::string& SQL);

    inline ofxSQLiteTypeNow now() {
        return ofxSQLiteTypeNow();
    }
    
private:
    sqlite3* db;
    std::string db_file;

};
