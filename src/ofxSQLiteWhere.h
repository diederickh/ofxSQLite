#ifndef OFXSQLITEWHERE
#define OFXSQLITEWHERE

#include "ofxSQLiteFieldValues.h"
#include "lib/sqlite/sqlite3.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std; // tmp


enum WhereTypes {
		 WHERE
		,WHERE_AND
		,WHERE_OR
		,WHERE_LIKE
		,WHERE_OR_LIKE
		,WHERE_AND_LIKE
};

struct Where {
	int field_index;
	int type;
	
	std::string getAndOr(bool isFirstWhereClause = false) {
		std::string result;
		if(!isFirstWhereClause) {
			if(type == WHERE_AND) {
				result += " AND ";
			}
			else if(type == WHERE_OR) {
				result += " OR ";
			}
			else if(type == WHERE_OR_LIKE) {
				result += " OR  ";
			}
			else if(type == WHERE_AND_LIKE) {
				result += " AND  ";
			}
		}
		else {
			return " WHERE "; 
		}
		return result;
	}
};

class ofxSQLiteWhere {
	public:
		
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
			// It's also possible to use the where like:
			// andWhere('fieldname > ?',value), by default we use "=" as
			// comparator.
			std::stringstream ss(sField);
			std::string part;
			std::string prev_part = "";
			int sql_operator = 0;
			while(ss) {
				ss >> part;
				if(part == "<") {
					sql_operator = OP_LESS_THAN; 
				}
				else if(part == ">") {
					sql_operator = OP_GREATER_THAN;
				}
				else if(part == "<=") {
					sql_operator = OP_LESS_EQUAL_THAN;
				}
				else if(part == ">=") {
					sql_operator = OP_GREATER_EQUAL_THAN;
				}
				
				if(sql_operator != 0) {
					//div = part;
					//has_questionmark = true;
					sField = prev_part;
					break;
				}
				prev_part = part;
			}
			
			// when no other operator found we use the default one...
			if(sql_operator == 0) {
				sql_operator = OP_EQUAL;
			}
			if(nType == WHERE_LIKE || nType == WHERE_OR_LIKE || nType == WHERE_AND_LIKE) {
				sql_operator = OP_LIKE;
			}
			
			int field_value_index = where_values.use(sField, mValue);
			where_values.at(field_value_index).setOperatorType(sql_operator);
			
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
				
		std::string getLiteralQuery(bool bFillValues = false) {
			std::string result = "";
			std::vector<Where>::iterator it = wheres.begin();
			int counter = 0;
			while(it != wheres.end()) {
				Where& where = *it;
				FieldValuePair value_pair = where_values.at(counter);
				
				if(value_pair.type == OFX_SQLITE_TYPE_NULL) {
					result += where.getAndOr(counter == 0) +value_pair.field +" is null ";
				}
				else {
					result += where.getAndOr(counter == 0);
					result += value_pair.getFieldAndValueForQuery();
				}
				++counter;
				++it;
			}
			return result;
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
