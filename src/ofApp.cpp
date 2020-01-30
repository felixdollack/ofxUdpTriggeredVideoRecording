#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"

//--------------------------------------------------------------
void ofApp::setup(){
    this->tryLoadingPreferencesOrDefaults();

    this->udp_thread = new UdpReceiverThread(this->udp_port);
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
    ofBackground(this->background_color);
    this->camera.draw(0, 0);
    this->drawRecordingIndicator(this->xRecordingIndicator, this->yRecordingIndicator, this->state);
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
        ofDrawCircle(x, y, this->radiusRecordingIndicator);
    }
    //if (this->state == 0) draw empty circle (which is the edge anyway, so draw always)
    ofNoFill();
    ofSetColor(ofColor::white);
    ofDrawCircle(x, y, this->radiusRecordingIndicator);
}

void ofApp::setupCamera() {
    // Set the custom ofxPS3EyeGrabber.
    this->camera.setGrabber(std::make_shared<ofxPS3EyeGrabber>());

    // The native pixel format for the ofxPS3EyeGrabber is a Bayer pattern
    // (it will look black and white).
    this->camera.setPixelFormat(OF_PIXELS_NATIVE);
    this->camera.setDesiredFrameRate(this->desiredCameraFrameRate);
    this->camera.setup(this->cameraWidth, this->cameraHeight);

    // These are examples of ofxPS3EyeGrabber-specific paramaters.
    // These must be set after the grabber is set up.
    this->camera.getGrabber<ofxPS3EyeGrabber>()->setAutogain(true);
    this->camera.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(true);
}

void ofApp::tryLoadingPreferencesOrDefaults() {
    this->settings = new ofxXmlSettings();
    bool success = this->settings->loadFile("settings.xml");
    if (!success) {
        // set standard values and save settings
        this->setDefaultSettings();
        this->settings->saveFile("settings.xml");
    }
    this->unpackSettings();
}

void ofApp::setDefaultSettings() {
    // write layout settings
    this->background_color = ofColor(200, 200, 200);
    this->settings->setValue("inhibitionExperiment:color:background:red",   this->background_color.r);
    this->settings->setValue("inhibitionExperiment:color:background:green", this->background_color.g);
    this->settings->setValue("inhibitionExperiment:color:background:blue",  this->background_color.b);
    this->radiusRecordingIndicator = 15;
    this->settings->setValue("inhibitionExperiment:layout:recIndicator:radius",  this->radiusRecordingIndicator);
    this->xRecordingIndicator = 100;
    this->settings->setValue("inhibitionExperiment:layout:recIndicator:x",  this->xRecordingIndicator);
    this->yRecordingIndicator = 100;
    this->settings->setValue("inhibitionExperiment:layout:recIndicator:y",  this->yRecordingIndicator);

    // write matlab network settings
    this->udp_port = 4245;
    this->settings->setValue("inhibitionExperiment:network:udpPort", this->udp_port);

    // write camera settings
    this->desiredCameraFrameRate = 75;
    this->settings->setValue("inhibitionExperiment:camera:fps",    this->desiredCameraFrameRate);
    this->cameraWidth  = 640;
    this->settings->setValue("inhibitionExperiment:camera:width",  this->cameraWidth);
    this->cameraHeight = 480;
    this->settings->setValue("inhibitionExperiment:camera:height", this->cameraHeight);
}

void ofApp::unpackSettings() {
    int r,g,b;
    // unpack settings
    // read layout settings
    r = this->settings->getValue("inhibitionExperiment:color:background:red",   0);
    g = this->settings->getValue("inhibitionExperiment:color:background:green", 0);
    b = this->settings->getValue("inhibitionExperiment:color:background:blue",  0);
    this->background_color = ofColor(r, g, b);
    this->radiusRecordingIndicator = this->settings->getValue("inhibitionExperiment:layout:recIndicator:radius", 0);
    this->xRecordingIndicator = this->settings->getValue("inhibitionExperiment:layout:recIndicator:x", 0);
    this->yRecordingIndicator = this->settings->getValue("inhibitionExperiment:layout:recIndicator:y", 0);

    // read matlab network settings
    this->udp_port = this->settings->getValue("inhibitionExperiment:network:udpPort", 0);

    // read camera settings
    this->desiredCameraFrameRate = this->settings->getValue("inhibitionExperiment:camera:fps", 25);
    this->cameraWidth  = this->settings->getValue("inhibitionExperiment:camera:width",  640);
    this->cameraHeight = this->settings->getValue("inhibitionExperiment:camera:height", 480);
}
