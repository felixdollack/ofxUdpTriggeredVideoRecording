#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"

void ofApp::exit() {
    ofRemoveListener(this->videoRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    this->videoRecorder.close();
}

//--------------------------------------------------------------
void ofApp::setup(){
    this->tryLoadingPreferencesOrDefaults();

    this->udp_thread = new UdpReceiverThread(this->udp_port);
    this->state = this->udp_thread->readState();
    this->udp_thread->start();
    this->setupCamera();

    // use this is you have ffmpeg installed in your data folder
    //this->videoRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg"));
    this->video_filename = "test";
    this->video_fileext = ".mp4";
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    this->videoRecorder.setVideoCodec("mpeg4");
    this->videoRecorder.setVideoBitrate("800k");
//    this->videoRecorder.setAudioCodec("mp3");
//    this->videoRecorder.setAudioBitrate("192k");
    ofAddListener(this->videoRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
}

//--------------------------------------------------------------
void ofApp::update(){
    // check for messages from Matlab (UDP)
    this->state = this->udp_thread->readState();

    // adjust recording to state changes
    switch (state){
        case 1: // record
            if (!this->videoRecorder.isInitialized()) {
                this->videoRecorder.setup(this->video_filename+ofGetTimestampString() + this->video_fileext, this->camera.getWidth(), this->camera.getHeight(), 30);
//                this->videoRecorder.setup(fileName+ofGetTimestampString()+fileExt, vidGrabber.getWidth(), vidGrabber.getHeight(), 30); // no audio
//                this->videoRecorder.setup(fileName+ofGetTimestampString()+fileExt, 0,0,0, sampleRate, channels); // no video
//                this->videoRecorder.setupCustomOutput(vidGrabber.getWidth(), vidGrabber.getHeight(), 30, sampleRate, channels, "-vcodec mpeg4 -b 1600k -acodec mp2 -ab 128k -f mpegts udp://localhost:1234"); // for custom ffmpeg output string (streaming, etc)

                // Start recording
                this->videoRecorder.start();
            } else if (this->videoRecorder.isInitialized()) {
                this->videoRecorder.setPaused(false);
            }
            break;
        case 0: // stop recording
            this->videoRecorder.close();
            break;
        case 2:
            if(this->videoRecorder.isInitialized()) {
                this->videoRecorder.setPaused(true);
            }
    }

    // update camera
    this->camera.update();

    // save current frame to video file
    if(this->camera.isFrameNew() && (this->state == 1)){
        bool success = this->videoRecorder.addFrame(this->camera.getPixels());
        if (!success) {
            ofLogWarning("This frame was not added!");
        }
    }
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

//--------------------------------------------------------------
void ofApp::recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args){
    cout << "The recoded video file is now complete." << endl;
}

//--------------------------------------------------------------
//void ofApp::audioIn(float *input, int bufferSize, int nChannels){
//    if (state == 1) {
//        this->videoRecorder.addAudioSamples(input, bufferSize, nChannels);
//    }
//}

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
