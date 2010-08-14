#include "ofxSQLiteSelect.h"
#include <iostream>
#include <sstream>

ofxSQLiteSelect::ofxSQLiteSelect(sqlite3* pSQLite):sqlite(pSQLite),limit_offset(0),limit_count(0) {
}

ofxSQLiteSelect& ofxSQLiteSelect::select(std::string sFields) {
	fields = sFields;
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::join(std::string sTable ,std::string sOnField, std::string sFields) {
	struct Join join = {
		sTable
		,sOnField
		,sFields
	};
	joins.push_back(join);
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::order(std::string sField) {
	return order(sField, "ASC");
}

ofxSQLiteSelect& ofxSQLiteSelect::order(std::string sField, std::string sOrder) {
	struct Order order;
	order.field = sField;
	order.order = sOrder;
	orders.push_back(order);
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::from(std::string sFrom) {
	from_table = sFrom;
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::limit(int nCount) {
	return limit(nCount,0);
}

ofxSQLiteSelect& ofxSQLiteSelect::limit(int nCount, int nOffset) {
	limit_count = nCount;
	limit_offset = nOffset;
	return *this;
}

std::string ofxSQLiteSelect::getLiteralQuery() {
	// inner joins.
	std::string inner_joins = "";
	for (int i = 0; i < joins.size(); ++i) {
		fields += ", " +joins[i].fields;
		inner_joins = " INNER JOIN " +joins[i].table +" ON " +joins[i].condition;
	}

	// wheres
	std::string where = wheres.getLiteralQuery();

	// limit + offset
	std::string limit = "";
	if (limit_count != 0 && limit_offset == 0) {
		std::stringstream ss_count;
		ss_count << limit_count;
		limit = " LIMIT " +ss_count.str() +" ";
	}
	else if (limit_offset != 0 && limit_count != 0) {
		std::stringstream ss_count, ss_offset;
		ss_count << limit_count;
		ss_offset << limit_offset;
		limit = " LIMIT " +ss_offset.str() +", " +ss_count.str() +" ";
	}

	// order by
	std::string order_by = "";
	if (orders.size() > 0) {
		order_by += " ORDER BY ";
		for(int i = 0; i < orders.size(); ++i) {
			order_by += " " +orders.at(i).field +" " +orders.at(i).order +((i < orders.size()-1) ? ", " : "");
		}
	}

	// combine all sql parts
	std::string sql = 	" SELECT " +fields
						+" FROM " +from_table
						+inner_joins
						+where
						+order_by
						+limit;

	//std::cout << sql << std::endl;
	return sql;
}

ofxSQLiteSelect& ofxSQLiteSelect::execute() {
	std::string sql = getLiteralQuery();
	if (SQLITE_OK != sqlite3_prepare_v2(sqlite, sql.c_str(),-1, &statement, 0)) {
		sqlite3_finalize(statement);
		std::cout << sqlite3_errmsg(sqlite);
		return *this;
	}
	wheres.bind(statement);
	return *this;
}

ofxSQLiteSelect& ofxSQLiteSelect::begin() {
	col_index = 0;
	last_result = sqlite3_step(statement);
	return *this;
}

int ofxSQLiteSelect::next() {
	last_result = sqlite3_step(statement);
	col_index = 0;
	return last_result;
}

bool ofxSQLiteSelect::hasNext() {
	if (last_result != SQLITE_DONE) {
		return true;
	}
	else {
		sqlite3_finalize(statement);
		return false;
	}
}

bool ofxSQLiteSelect::hasRow() {
	return last_result == SQLITE_ROW;
}

std::string ofxSQLiteSelect::getString(int nIndex) {
	std::string result = "";
	if(last_result != SQLITE_ROW) {
		return result;
	}
	int use_index = nIndex;
	if(use_index == -1)
		use_index = col_index++;
		
	std::stringstream ss;
	ss << sqlite3_column_text(statement, use_index);
	return ss.str();
}

int ofxSQLiteSelect::getInt(int nIndex) {
	if(last_result != SQLITE_ROW) {
		return 0;
	}
	int use_index = nIndex;
	if(use_index == -1)
		use_index = col_index++;
	return sqlite3_column_int(statement, use_index);
}
