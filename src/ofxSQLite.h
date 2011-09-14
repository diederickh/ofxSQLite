/**
 * When compiling the sqlite.c file I needed to put:
 * SQLITE_ENABLE_UPDATE_DELETE_LIMIT=1
 * in the build options.
 *
 * Or you can use a build define like: SQLITE_DEBUG 0
 *
 */
#ifndef OFXSQLITEH
#define OFXSQLITEH
#define DSQLITE_ENABLE_UPDATE_DELETE_LIMIT 1

#include <vector>
#include <string>
#include "lib/sqlite/sqlite3.h"
//#include "ofMain.h"

#include "ofxSQLiteTypeNow.h"

//
//#include "ofxSQLite.h"
#include "ofxSQLiteAbstract.h"
#include "ofxSQLiteInsert.h"
#include "ofxSQLiteUpdate.h"
#include "ofxSQLiteDelete.h"
#include "ofxSQLiteSelect.h"
#include "ofxSQLiteType.h"
#include "ofxSQLiteTypeNow.h"
//#include "ofxSQLiteSimpler.h"
class ofxSQLiteSimpler;
//
/*
class ofxSQLiteInsert;
class ofxSQLiteSelect;
class ofxSQLiteUpdate;
class ofxSQLiteDelete;
*/
class ofxSQLite {
	public:
		ofxSQLite();
		ofxSQLite(std::string sDB);
		void setup(std::string sDB);
		ofxSQLiteInsert insert(std::string sTable);
		ofxSQLiteUpdate update(std::string sTable);
		ofxSQLiteDelete	remove(std::string sTable);
		ofxSQLiteSelect	select(std::string sFields);
		
		ofxSQLiteSimpler operator[](const std::string sKeyValue);
		
		int lastInsertID();
		const char* getError();

		int simpleQuery(const char* pSQL);
		inline ofxSQLiteTypeNow now() {
			return ofxSQLiteTypeNow();
		}

	private:
		sqlite3* db;
		std::string db_name;
		std::string db_file;
};
#endif

