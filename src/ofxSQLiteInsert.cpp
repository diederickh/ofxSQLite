#include "ofxSQLite.h"
#include "ofxSQLiteAbstract.h"
#include "ofxSQLiteInsert.h"
#include "ofxSQLiteFieldValues.h"
#include "lib/sqlite/sqlite3.h"


ofxSQLiteInsert::ofxSQLiteInsert(sqlite3* pSQLite, std::string sTable):sqlite(pSQLite),table(sTable),pair_count(0) {
}

ofxSQLiteFieldValues ofxSQLiteInsert::getFields() {
	return field_values;
}

std::string ofxSQLiteInsert::getLiteralQuery() {
	std::string sql = "INSERT INTO " +table +" (";
	std::string values = ") VALUES (";
	field_values.begin();
	while(field_values.hasNext()) {
		FieldValuePair field = field_values.current();
		field_values.next();

		sql += field.field +(field_values.hasNext() ? "," : "");
		values += "?" +(std::string)field.indexString()+(field_values.hasNext() ? "," : "");
	}
	return sql + values +")";
}

int ofxSQLiteInsert::execute() {
	std::string sql = getLiteralQuery();
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(),-1, &statement, 0)) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	// bind values.
	field_values.bind(statement);

	// execute the query.
	if (sqlite3_step(statement) != SQLITE_DONE) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	// cleanup.
	sqlite3_finalize(statement);
	return SQLITE_OK;
}
