#include "ofxSQLiteUpdate.h"
#include "lib/sqlite/sqlite3.h"
#include <iostream>

ofxSQLiteUpdate::ofxSQLiteUpdate(sqlite3* pSQLite, std::string sTable):sqlite(pSQLite), table(sTable) {
}

std::string ofxSQLiteUpdate::getLiteralQuery() {
	std::string sql = "UPDATE " +table +" SET ";

	field_values.begin();
	while(field_values.hasNext()) {
		FieldValuePair field = field_values.current();
		sql += field.field +" = ?" +field.indexString();
		field_values.next();
		sql += (field_values.hasNext() ? "," : "");
	}

	if(wheres.size() > 0) {
		sql += wheres.getLiteralQuery();
	}
	return sql;
}

int ofxSQLiteUpdate::execute() {
	std::string sql = getLiteralQuery();
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(),-1, &statement, 0)) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	field_values.bind(statement);
	wheres.bind(statement);

	if (sqlite3_step(statement) != SQLITE_DONE) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	sqlite3_finalize(statement);
	return SQLITE_OK;
}
