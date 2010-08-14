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
#include "ofMain.h"


class ofxSQLiteInsert;
class ofxSQLiteSelect;
class ofxSQLiteUpdate;
class ofxSQLiteDelete;

class ofxSQLite {
	public:
		ofxSQLite(std::string sDB);
		ofxSQLiteInsert insert(std::string sTable);
		ofxSQLiteUpdate update(std::string sTable);
		ofxSQLiteDelete	remove(std::string sTable);
		ofxSQLiteSelect	select(std::string sFields);
		int lastInsertID();
		const char* getError();

		int simpleQuery(const char* pSQL);

	private:
		sqlite3* db;
		std::string db_name;
		std::string db_file;
};
#endif

