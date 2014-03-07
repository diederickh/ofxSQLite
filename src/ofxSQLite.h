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


class ofxSQLiteSimpler {
public:
	ofxSQLiteSimpler(ofxSQLite& rDB, const std::string& sTable):
        db(rDB),
        table(sTable)
	{
	}
	
	template<typename T>
	ofxSQLiteSelect find(const std::string& sWhereField,
                         T mWhereValue,
                         const std::string& sSelectFields = "*")
    {
		ofxSQLiteSelect sel = db.select(sSelectFields).from(table).where(sWhereField, mWhereValue).execute();
		return sel;
	}
	
	// example: db["pakbox_users"].findOne("pu_id", 68, "pu_name").getString(0);
	template<typename T>
	ofxSQLiteSelect findOne(const std::string& sWhereField,
                            T mWhereValue,
                            const std::string& sSelectFields = "*")
    {
		ofxSQLiteSelect sel = db
			.select(sSelectFields)
			.from(table)
			.where(sWhereField, mWhereValue)
			.limit(1)
			.execute()
			.begin();
		
		return sel;
	}
	
	void print(){
		cout << db.select("*").from(table).execute().getResultAsAsciiTable();
	}
	
private:
	ofxSQLite& db;
    std::string table;

};
