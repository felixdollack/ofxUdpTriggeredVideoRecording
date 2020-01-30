#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"

//--------------------------------------------------------------
void ofApp::setup(){
    this->udp_thread = new UdpReceiverThread(4245);
    this->state = this->udp_thread->readState();
    this->udp_thread->start();
    this->setupCamera();
}

//--------------------------------------------------------------
void ofApp::update(){
    this->state = this->udp_thread->readState();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(ofColor::blanchedAlmond);
    this->camera.draw(0, 0);
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

void ofApp::setupCamera() {
    // Set the custom ofxPS3EyeGrabber.
    this->camera.setGrabber(std::make_shared<ofxPS3EyeGrabber>());

    // The native pixel format for the ofxPS3EyeGrabber is a Bayer pattern
    // (it will look black and white).
    this->camera.setPixelFormat(OF_PIXELS_NATIVE);
    this->camera.setDesiredFrameRate(75);
    this->camera.setup(640, 480);

    // These are examples of ofxPS3EyeGrabber-specific paramaters.
    // These must be set after the grabber is set up.
    this->camera.getGrabber<ofxPS3EyeGrabber>()->setAutogain(true);
    this->camera.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(true);
}
