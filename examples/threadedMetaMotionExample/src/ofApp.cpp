#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(200);
    threadedmmc.setup();
    threadedmmc.start();
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetBackgroundColor(30);
    
    ofPushMatrix();
    ofNoFill();
    ofTranslate(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    
    // rotate
    float* a = threadedmmc.mmc.getAngle();
    
    float rX = a[0];
    float rY = a[1];
    float rZ = a[2];
    
    ofRotateXDeg(rX);
    ofRotateYDeg(rY);
    ofRotateZDeg(rZ);

    ofDrawBox(0, 0, 0, 200);
    ofPopMatrix();

    // information text
    ofSetColor(255);
    ofDrawBitmapString("Angle", 600, 20);

    for(int i =0; i < 3; i++){
        ofDrawBitmapString(threadedmmc.mmc.angle[i], 600, (i + 1) * 50);
    }

    auto threadFrame = threadedmmc.getThreadFrameNum();
    ofSetColor(255, 0, 0);
    ofDrawBitmapString("app frame: " + ofToString(ofGetFrameNum()), 20, 20);
    ofDrawBitmapString("thread frame: " + ofToString(threadFrame), 20, 35);
    ofDrawBitmapString("diff: " + ofToString(int64_t(ofGetFrameNum()) - threadFrame), 20, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    //reset orientation
    threadedmmc.mmc.tare();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
