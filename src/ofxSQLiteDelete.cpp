#include "ofxSQLiteDelete.h"
#include "lib/sqlite/sqlite3.h"
#include <iostream>

ofxSQLiteDelete::ofxSQLiteDelete(sqlite3* pSQLite, std::string sTable):sqlite(pSQLite), table(sTable) {
}

std::string ofxSQLiteDelete::getLiteralQuery(bool bFillValues) {
	std::string sql = "DELETE FROM " +table +" ";
	if(wheres.size() > 0) {
		sql += wheres.getLiteralQuery(bFillValues);
	}
	return sql;
}

int ofxSQLiteDelete::execute() {
	std::string sql = getLiteralQuery();
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(),-1, &statement, 0)) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	wheres.bind(statement);

	if (sqlite3_step(statement) != SQLITE_DONE) {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	sqlite3_finalize(statement);
	return SQLITE_OK;
}
