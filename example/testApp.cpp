#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
testApp::testApp(){
	sqlite = new ofxSQLite("test.db");
	sqlite->simpleQuery(""\
		"CREATE TABLE IF NOT EXISTS scores (" \
			" id INTEGER PRIMARY KEY AUTOINCREMENT" \
			" ,time TEXT" \
			", score INTEGER" \
		");"
	);

	sqlite->simpleQuery(""\
		"CREATE TABLE IF NOT EXISTS stats ("\
			"id INTEGER PRIMARY KEY AUTOINCREMENT" \
			", time TEXT" \
		");"
	);

	sqlite->simpleQuery(""\
		"CREATE TABLE IF NOT EXISTS game_runs( " \
			" id INTEGER PRIMARY KEY AUTOINCREMENT" \
			",start_time TEXT" \
			", end_time TEXT" \
		");"
	);

	if (SQLITE_OK != sqlite->simpleQuery(""\
		"CREATE TABLE IF NOT EXISTS game_run_data( " \
			" gid INTEGER PRIMARY KEY AUTOINCREMENT" \
			",runid INTEGER" \
			",gdata TEXT" \
		");"
	)) {
		cout << "ERROR CREATE TABLE\n";
	}

	// insert
	sqlite->insert("game_runs")
		.use("start_time", "today")
		.use("end_time","tomorrow")
	.execute();

	// lastInsertID
	int last_run_id = sqlite->lastInsertID();
	sqlite->insert("game_run_data")
		.use("runid",last_run_id)
		.use("gdata", "MyData")
	.execute();
	cout << "insert into game_run_data error:" << sqlite->getError() << endl;;


	// insert
	sqlite->insert("scores")
		.use("score", 5999)
		.use(
			"time"
			,ofToString(ofGetDay())
			 +"-" +ofToString(ofGetMonth())
			 +"-" +ofToString(ofGetYear())
			 +" " +ofToString(ofGetHours())
			 +":" +ofToString(ofGetMinutes())
			 +":" +ofToString(ofGetSeconds())
	).execute();

	// get last inserted row id
	cout << "inserted row id: " << sqlite->lastInsertID() << endl;


	ofxSQLiteSelect sel = sqlite->select("id, time").from("scores");
	sel.execute().begin();

	while(sel.hasNext()) {
		int id = sel.getInt();
		std::string name = sel.getString();
		cout << id << ", " << name << endl;
		sel.next();
	}

	// select
	sel = sqlite->select("id, start_time")
		.from("game_runs")
		.join("game_run_data", "runid = id", "runid, gdata")
		.where("runid", 3)
		.orWhere("runid",13)
		.orWhere("runid", last_run_id)
		//.limit(5)
		.order("runid", " DESC ")
		.execute().begin();

	while(sel.hasNext()) {
		int runid = sel.getInt();
		string gdata = sel.getString();
		cout << "runid: " << runid << ", gdata: " << gdata << endl;
		sel.next();
	}

	// update
	sqlite->update("game_runs")
		.use("end_time", "past")
		.where("id", last_run_id)
		.execute();


	// delete
	sqlite->remove("game_runs")
		.where("id",last_run_id)
		.execute();
		
	// auto increment field and auto-timestamp field. on each insert
	// the value for date_created is added automatically.
	// -------------------------------------------------------------------------
	if (SQLITE_OK != sqlite->simpleQuery(""\
		"CREATE TABLE IF NOT EXISTS  photos( " \
			" id INTEGER PRIMARY KEY AUTOINCREMENT" \
			",old_name VARCHAR(255)" \
			",new_name VARCHAR(255)" \
			",dir_name VARCHAR(255)" \
			",file_path VARCHAR(255)" \
			",synchronized BOOLEAN" \
			",date_synchronized DATETIME" \
			",date_created DATETIME DEFAULT CURRENT_TIMESTAMP" \
		");"
	)) {
		cout << "ERROR CREATE TABLE\n";
	}
	
	// just pasted this example here from a project I did.. (did no test it, 
	// but shows you some things you can do with sqlite tables
	// ------------------------------------------------------------------------
	int r = sqlite->simpleQuery("CREATE TABLE IF NOT EXISTS tweets (" \
						" id INTEGER PRIMARY KEY AUTOINCREMENT " \
						",avatar VARCHAR(255) " \
						",user_id VARCHAR(100) " \
						",screen_name VARCHAR(50)" \
						",tweet_id VARCHAR(50) " \
						",date_created DATETIME DEFAULT CURRENT_TIMESTAMP "\
						",date_exported DATETIME "\
						",contains_face BOOLEAN " \
						",is_used BOOLEAN " \
						",is_fetched BOOLEAN " \
						",is_exported BOOLEAN " \
					");"
	);
	
	// inserting mass amount of entries: use transations
	// -------------------------------------------------------------------------
	if (SQLITE_OK != sqlite->simpleQuery("BEGIN TRANSACTION;")) {
		cout << "ERROR: cannot begin transaction" << std::endl;
	}
	
	for(int i = 0; i < num_entries; ++i) {
		int result = sqlite->insert("photos")
			.use("old_name", "old_name")
			.use("new_name","new_name")
			.use("dir_name", "IMAGE2008.08.06/")
			.use("file_path", "dirname/image0001.jpg")
			.use("synchronized",0)
		.execute();
		if(result != SQLITE_OK) {
			cout << "error: " << result << endl;
			cout << "message:" << sqlite->getError() << endl;
			break;
		}
		else {
			ok++;
		}
		
	}
	cout << "Inserted: " << ok << std::endl;
	if (SQLITE_OK != sqlite->simpleQuery("COMMIT;")) {
		cout << "ERROR: cannot commit" << std::endl;
	}

	// Get amount of rows.
	//-------------------------------------------------------------------------
	ofxSQLiteSelect sel = sqlite->select("count(id) as total").from("photos");
	sel.execute().begin();
	int count = sel.getInt();
	cout << "total entries:" << count << std::endl;

	// Example of using a timestamp (not tested in this code; purely as example)
	// -------------------------------------------------------------------------
	/*
	// table with a couple of date fields
	db = new ofxSQLite(ofToDataPath("tweets.db"));
	int r = db->simpleQuery("CREATE TABLE IF NOT EXISTS tweets (" \
						" id INTEGER PRIMARY KEY AUTOINCREMENT " \
						",avatar VARCHAR(255) " \
						",user_id VARCHAR(100) " \
						",screen_name VARCHAR(50)" \
						",tweet_id VARCHAR(50) " \
						",date_created DATETIME DEFAULT CURRENT_TIMESTAMP "\
						",date_exported DATETIME "\
						",contains_face BOOLEAN " \
						",is_used BOOLEAN " \
						",is_fetched BOOLEAN " \
						",is_exported BOOLEAN " \
					");"
	);
	
	// Updating the time field with a current timestamp:
	// you can use "now()" to get a timestamp object
	int r = db->update("tweets").use("is_exported",true).use("date_exported",db->now()).execute();
	if(r != SQLITE_OK) {
		ofLog(OF_LOG_ERROR, "error: cannot set export to true");
	}
	*/

}

//--------------------------------------------------------------
void testApp::setup(){
}

//--------------------------------------------------------------
void testApp::update(){
}


//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}

