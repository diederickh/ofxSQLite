#include "ofxSQLite.h"
#include "ofxSQLiteSimpler.h"


ofxSQLite::ofxSQLite():
    db(0),
    db_file("")
{
}


ofxSQLite::~ofxSQLite()
{
    if (db)
    {
        sqlite3_close(db);
    }
}


bool ofxSQLite::setup(const std::string& sDB)
{
	db_file = ofToDataPath(sDB, true);

    if (db)
    {
        sqlite3_close(db);
    }

    int err = sqlite3_open(db_file.c_str(), &db);

	if (SQLITE_OK != err)
    {
        ofLogError("ofxSQLite::setup") << sqlite3_errmsg(db);
        db = 0;
        return false;
	}
    else
    {
        ofLogVerbose("ofxSQLite::setup") << "Version: " << getVersion();
        ofLogVerbose("ofxSQLite::setup") << "Version Number: " << getVersionNumber();
        ofLogVerbose("ofxSQLite::setup") << "SourceID: " << getSourceId();
        ofLogVerbose("ofxSQLite::setup") << "Threadsafe: " << isThreadsafe();
        ofLogVerbose("ofxSQLite::setup") << "Opened:" << db_file << endl;
        return true;
    }

}


bool ofxSQLite::isLoaded() const
{
    return 0 != db;
}


int ofxSQLite::simpleQuery(const std::string& SQL)
{
	sqlite3_stmt* statement;

    int err = sqlite3_prepare_v2(db, SQL.c_str(), SQL.length(), &statement, 0);

	if (SQLITE_OK != err)
    {
        sqlite3_finalize(statement);
		return sqlite3_errcode(db);
	}

    while(SQLITE_ROW == sqlite3_step(statement)); // consuming results

	return sqlite3_finalize(statement);
}


ofxSQLiteInsert ofxSQLite::insert(const std::string& sTable) {
	return ofxSQLiteInsert(db, sTable);
}


ofxSQLiteUpdate ofxSQLite::update(const std::string& sTable) {
	return ofxSQLiteUpdate(db, sTable);
}


ofxSQLiteDelete ofxSQLite::remove(const std::string& sTable) {
	return ofxSQLiteDelete(db, sTable);
}


ofxSQLiteSelect	ofxSQLite::select(const std::string& sFields) {
	return ofxSQLiteSelect(db).select(sFields);;
}


ofxSQLiteTypeNow ofxSQLite::now() const
{
    return ofxSQLiteTypeNow();
}


std::string ofxSQLite::getError() const {
    const char* err = sqlite3_errmsg(db);
    return err ? err : "Unknown Error";
}


sqlite_int64 ofxSQLite::lastInsertID() const {
	return sqlite3_last_insert_rowid(db);
}


ofxSQLiteSimpler ofxSQLite::operator[](const std::string& sKeyValue) {
	return ofxSQLiteSimpler(*this, sKeyValue);
}


void ofxSQLite::printTable(const std::string& sTable) {
    std::cout << select("*").from(sTable).execute().getResultAsAsciiTable();
}


bool ofxSQLite::isThreadsafe() const
{
    return sqlite3_threadsafe() != 0;
}


std::string ofxSQLite::getVersion() const
{
    const char* v = sqlite3_libversion();
    return v ? v : "UNKNOWN";
}


std::string ofxSQLite::getSourceId() const
{
    const char* v = sqlite3_sourceid();
    return v ? v : "UNKNOWN";
}


int ofxSQLite::getVersionNumber() const
{
    return sqlite3_libversion_number();
}

