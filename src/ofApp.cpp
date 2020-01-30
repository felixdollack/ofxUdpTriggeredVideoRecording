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
