#include "ofxSQLite.h"
#include "ofxSQLiteInsert.h"
#include "ofxSQLiteFieldValues.h"
#include "sqlite/sqlite3.h"


ofxSQLiteInsert::ofxSQLiteInsert(sqlite3* pSQLite,
                                 const std::string& sTable):
    sqlite(pSQLite),
    table(sTable),
    pair_count(0)
{
}

ofxSQLiteFieldValues ofxSQLiteInsert::getFields() const {
	return field_values;
}

std::string ofxSQLiteInsert::getLiteralQuery() {
    std::stringstream sql;
    sql << "INSERT INTO ";
    sql << table <<
    sql << " (";

    std::stringstream values;
	values << ") VALUES (";

    field_values.begin();

	while(field_values.hasNext()) {
		FieldValuePair field = field_values.current();
        
		field_values.next();

        std::string delimiter = (field_values.hasNext() ? "," : "");

        sql << field.field;
        sql << delimiter;

		values << "?";
        values << field.indexString();
        values << delimiter;
	}

    sql << values << ")";

	return sql.str();
}

int ofxSQLiteInsert::execute()
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

	// bind values.
	field_values.bind(statement);

	// execute the query.
	if (SQLITE_DONE != sqlite3_step(statement))
    {
		sqlite3_finalize(statement);
		return sqlite3_errcode(sqlite);
	}

	// cleanup.
	sqlite3_finalize(statement);
	return SQLITE_OK;
}
