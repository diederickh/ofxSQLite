#pragma once


#include <string>
#include "ofxSQLite.h"


class ofxSQLiteSimpler {
public:
	ofxSQLiteSimpler(ofxSQLite& rDB, const std::string& sTable);

	template<typename T>
	ofxSQLiteSelect find(const std::string& sWhereField,
                         const T& mWhereValue,
                         const std::string& sSelectFields = "*")
    {
		return db.select(sSelectFields).from(table).where(sWhereField,
                                                          mWhereValue).execute();
	}

	// example: db["pakbox_users"].findOne("pu_id", 68, "pu_name").getString(0);
	template<typename T>
	ofxSQLiteSelect findOne(const std::string& sWhereField,
                            const T& mWhereValue,
                            const std::string& sSelectFields = "*")
    {
		return db.select(sSelectFields)
        .from(table)
        .where(sWhereField, mWhereValue)
        .limit(1)
        .execute()
        .begin();
	}

	void print()
    {
		std::cout << db.select("*").from(table).execute().getResultAsAsciiTable();
	}

private:
	ofxSQLite& db;
    std::string table;

};
