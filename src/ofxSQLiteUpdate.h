#ifndef OFXSQLITEUPDATEH
#define OFXSQLITEUPDATEH

#include <vector>
#include <string>
#include "ofxSQLiteWhere.h"
#include "ofxSQLiteFieldValues.h"

class ofxSQLiteFieldValues;
class sqlite3;

class ofxSQLiteUpdate {
	public:
		ofxSQLiteUpdate(sqlite3*  pSQLite, std::string sTable);

		template<typename T>
		ofxSQLiteUpdate& use(std::string sField, T sValue) {
			field_values.use(sField, sValue);
 			return *this;
		}

		std::string getLiteralQuery();
		int execute();

		// where clause..
		template<typename T>
		ofxSQLiteUpdate& where(std::string sField, T mValue) {
			return where(sField, mValue, WHERE);
		}
		template<typename T>
		ofxSQLiteUpdate& orWhere(std::string sField, T mValue) {
			return where(sField, mValue, WHERE_OR);
		}
		template<typename T>
		ofxSQLiteUpdate& andWhere(std::string sField, T mValue) {
			return where(sField, mValue, WHERE_AND);
		}
		template<typename T>
		ofxSQLiteUpdate& where(std::string sField, T mValue, int nType) {
			wheres.where(sField, mValue, nType);
			return *this;
		}

	private:
		std::string table;
		ofxSQLiteWhere wheres;
		ofxSQLiteFieldValues field_values;
		sqlite3* sqlite;
};
#endif
