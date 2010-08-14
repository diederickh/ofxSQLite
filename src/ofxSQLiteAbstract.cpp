#include "ofxSQLiteAbstract.h"
#include "ofxSQLiteFieldValues.h"
#include "lib/sqlite/sqlite3.h"

ofxSQLiteAbstract::ofxSQLiteAbstract(sqlite3* pSQLite):sqlite(pSQLite) {
}
