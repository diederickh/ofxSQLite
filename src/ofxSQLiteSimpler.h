#ifndef OFXSQLITESIMPLERH
#define OFXSQLITESIMPLERH

#include <string>
#include <iostream>

#include "ofxSQLite.h"
#include "ofxSQLiteSelect.h"

using std::string;
using std::cout;
using std::endl;

class ofxSQLite;
class ofxSQLiteSimpler {
public:
	ofxSQLiteSimpler(ofxSQLite& rDB, string sTable)
	:db(rDB)
	{
	}
	void setDB(ofxSQLite& rDB) {
	}
	
	template<typename T>
	ofxSQLiteSelect find(string sWhereField, T mWhereValue) {
		ofxSQLiteSelect sel = db.select("*").from(table).where(sWhereField, mWhereValue);
		return sel;
	}
	
	
private:
	ofxSQLite& db;
	string table;

};
#endif
