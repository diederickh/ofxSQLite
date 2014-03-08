#include "ofxSQLiteSimpler.h"


ofxSQLiteSimpler::ofxSQLiteSimpler(ofxSQLite& rDB, const std::string& sTable):
    db(rDB),
    table(sTable)
{
}
