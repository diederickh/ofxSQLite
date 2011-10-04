#include "ofxSQLiteSimpler.h"

ofxSQLiteSimpler::ofxSQLiteSimpler(ofxSQLite& rDB, string sTable) 
:db(rDB)
,table(sTable)
{
}

