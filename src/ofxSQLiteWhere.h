#ifndef OFXSQLITEWHERE
#define OFXSQLITEWHERE

#include "ofxSQLiteFieldValues.h"
#include "lib/sqlite/sqlite3.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct Where {
	int field_index;
	int type;
};

class ofxSQLiteWhere {
	public:
		enum {
			 WHERE
			,WHERE_AND
			,WHERE_OR
		};

		// where clause..
		template<typename T>
		ofxSQLiteWhere& where(std::string sField, T mValue) {
			return where(sField, mValue, WHERE);
		}

		template<typename T>
		ofxSQLiteWhere& orWhere(std::string sField, T mValue) {
			return where(sField, mValue, WHERE_OR);
		}

		template<typename T>
		ofxSQLiteWhere& andWhere(std::string sField, T mValue) {
			return where(sField, mValue, WHERE_AND);
		}

		template<typename T>
		ofxSQLiteWhere& where(std::string sField, T mValue, int nType) {
			where_values.use(sField, mValue);
			struct Where where;
			where.type = nType;
			where.field_index = where_values.size() - 1;
			wheres.push_back(where);
 			return *this;
		}
		
		ofxSQLiteWhere& whereNull(std::string sField) {
			where_values.use(sField);
			struct Where where;
			where.type = WHERE;
			where.field_index = where_values.size() - 1;
			wheres.push_back(where);
 			return *this;
		}

		std::string getLiteralQuery() {
			std::string where = "";
			for (int i = 0; i < wheres.size(); ++i) {
				FieldValuePair pair = where_values.at(i);
				struct Where cond = wheres[i];
				std::string where_type = "";
				switch(cond.type) {
					case WHERE: where_type = " WHERE "; break;
					case WHERE_OR: where_type = " OR "; break;
					case WHERE_AND: where_type = " AND "; break;
				}
				if(pair.type == OFX_SQLITE_TYPE_NULL) 
					where += where_type +pair.field +" is null ";
				else
					where += where_type +pair.field +" = ?" +pair.indexString() +" ";
			}
			return where;
		}

		void bind(sqlite3_stmt* pStatement) {
			where_values.bind(pStatement);
		}

		int size() {
			return where_values.size();
		}

	private:
		ofxSQLiteFieldValues where_values;
		std::vector<Where> wheres;
};
#endif
