#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(200);
    threadedmmc.setup();
    threadedmmc.start();
    
    mmc.setup();

}

//--------------------------------------------------------------
void ofApp::update()
{
    mmc.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofSetBackgroundColor(30);

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
