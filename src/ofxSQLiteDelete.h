#ifndef OFXSQLITEDELETEH
#define OFXSQLITEDELETEH

#include <vector>
#include <string>
#include "ofxSQLiteWhere.h"
#include "ofxSQLiteFieldValues.h"

class ofxSQLiteFieldValues;
class sqlite3;

class ofxSQLiteDelete {
	public:
		ofxSQLiteDelete(sqlite3*  pSQLite, std::string sTable);

		template<typename T>
		ofxSQLiteDelete& use(std::string sField, T sValue) {
			field_values.use(sField, sValue);
 			return *this;
		}

		std::string getLiteralQuery(bool bFillValues = false);
		int execute();

		// where clause..
		template<typename T>
		ofxSQLiteDelete& where(std::string sField, T mValue) {
			return where(sField, mValue, WHERE);
		}
		template<typename T>
		ofxSQLiteDelete& orWhere(std::string sField, T mValue) {
			return where(sField, mValue, WHERE_OR);
		}
		template<typename T>
		ofxSQLiteDelete& andWhere(std::string sField, T mValue) {
			return where(sField, mValue, WHERE_AND);
		}
		template<typename T>
		ofxSQLiteDelete& where(std::string sField, T mValue, int nType) {
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

