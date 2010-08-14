#ifndef OFXSQLITEABSTRACTH
#define OFXSQLITEABSTRACTH


#include <string>

class ofxSQLite;
class ofxSQLiteFieldValues;
class sqlite3;

class ofxSQLiteAbstract {
	public:
		ofxSQLiteAbstract(sqlite3* pSQLite);
		sqlite3* sqlite;

	private:

};

#endif
