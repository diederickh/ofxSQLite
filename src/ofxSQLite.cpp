#include "ofxSQLite.h"
#include "ofxSQLiteSimpler.h"


ofxSQLite::ofxSQLite():
    db(0),
    db_file("")
{
}


void ofxSQLite::setup(const std::string& sDB)
{
	db_file = ofToDataPath(sDB, true);

	if (SQLITE_OK != sqlite3_open(db_file.c_str(), &db))
    {
        ofLogError("ofxSQLite::setup") << sqlite3_errmsg(db);
        db = 0;
	}

    ofLogVerbose("ofxSQLite::setup") << "opened:" << db_file << endl;
}

int ofxSQLite::simpleQuery(const std::string& SQL) {
	sqlite3_stmt* statement;

	if (SQLITE_OK != sqlite3_prepare_v2(db, SQL.c_str(), -1, &statement, 0)) {
		return sqlite3_errcode(db);
	}

    while(SQLITE_ROW == sqlite3_step(statement)); // ?

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

std::string ofxSQLite::getError() {
    const char* err = sqlite3_errmsg(db);
    return err ? err : "Unknown Error";
}

int ofxSQLite::lastInsertID() {
	return sqlite3_last_insert_rowid(db);
}

ofxSQLiteSimpler ofxSQLite::operator[](const std::string& sKeyValue) {
	return ofxSQLiteSimpler(*this, sKeyValue);
}

void ofxSQLite::printTable(const std::string& sTable) {
    std::cout << select("*").from(sTable).execute().getResultAsAsciiTable();
}
