#include "ofxSQLiteSimpler.h"

ofxSQLiteSimpler::ofxSQLiteSimpler(ofxSQLite& rDB, string sTable) 
:db(rDB)
,table(sTable)
{
}

void ofxSQLiteSimpler::setDB(ofxSQLite& rDB) {
	db = rDB;
}
