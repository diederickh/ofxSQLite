#include "ofxSQLiteDelete.h"
#include "sqlite/sqlite3.h"
#include <iostream>

ofxSQLiteDelete::ofxSQLiteDelete(sqlite3* pSQLite,
                                 const std::string& sTable):
    sqlite(pSQLite),
    table(sTable)
{
}

std::string ofxSQLiteDelete::getLiteralQuery(bool bFillValues)
{
	std::string sql = "DELETE FROM " +table +" ";
	if(wheres.size() > 0) {
		sql += wheres.getLiteralQuery(bFillValues);
	}
	return sql;
}

int ofxSQLiteDelete::execute()
{
	std::string sql = getLiteralQuery();

	sqlite3_stmt* statement;

    int err = sqlite3_prepare_v2(sqlite,
                                 sql.c_str(),
                                 sql.length(),
                                 &statement,
                                 0);

    if (SQLITE_OK != err)
    {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	wheres.bind(statement);

	if (SQLITE_DONE != sqlite3_step(statement))
    {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	sqlite3_finalize(statement);
    
	return SQLITE_OK;
}
