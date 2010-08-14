#ifndef OFXSQLITEINSERTH
#define OFXSQLITEINSERTH

#include "ofxSQLiteAbstract.h"
#include <vector>
#include "ofxSQLiteFieldValues.h"
#include <string>


class sqlite3;
class ofxSQLiteInsert {
	public:
		ofxSQLiteInsert(sqlite3* pSQLite, std::string sTable);
		std::string getLiteralQuery();
		ofxSQLiteFieldValues getFields();

		template<typename T>
		ofxSQLiteInsert& use(std::string sField, T sValue) {
			field_values.use(sField, sValue);
 			return *this;
		}
		int execute();

	private:
		std::string table;
		sqlite3* sqlite;
		int pair_count;
		ofxSQLiteFieldValues field_values;
};
#endif
