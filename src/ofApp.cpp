#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    this->udp_thread = new UdpReceiverThread(4245);
    this->state = this->udp_thread->readState();
    this->udp_thread->start();
}

//--------------------------------------------------------------
void ofApp::update(){
    this->state = this->udp_thread->readState();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::blanchedAlmond);
    this->drawRecordingIndicator(100, 100, this->state);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::drawRecordingIndicator(float x, float y, int recording_state) {
    if (recording_state == 1) {
        ofFill();
        ofSetColor(ofColor::darkRed);
        ofDrawCircle(x, y, 15);
    }
    //if (this->state == 0) draw empty circle (which is the edge anyway, so draw always)
    ofNoFill();
    ofSetColor(ofColor::white);
    ofDrawCircle(x, y, 15);
}
