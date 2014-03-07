#pragma once


#include "ofMain.h"
#include "ofxSQLite.h"


class ofApp: public ofBaseApp
{
public:
    void setup();

private:
    ofxSQLite sqlite;

};
