#include "ofApp.h"

void ofApp::exit() {
    ofRemoveListener(this->videoRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
    this->videoRecorder.close();
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetEscapeQuitsApp(false);
    this->tryLoadingPreferencesOrDefaults();
    this->state = 0;
    //create the socket and bind to port
    // https://openframeworks.cc/documentation/ofxNetwork/ofxUDPManager
    udpConnection.Create();
    udpConnection.Bind(4245);
    udpConnection.SetNonBlocking(true);

    this->setupCamera();

    // use this is you have ffmpeg installed in your data folder
    //this->videoRecorder.setFfmpegLocation(ofFilePath::getAbsolutePath("ffmpeg"));
    this->video_filename = "test";
    this->video_fileext = ".mp4";
    // override the default codecs if you like
    // run 'ffmpeg -codecs' to find out what your implementation supports (or -formats on some older versions)
    this->videoRecorder.setVideoCodec("mpeg4");
    this->videoRecorder.setVideoBitrate("800k");
    this->videoRecorder.setAudioCodec("mp3");
    this->videoRecorder.setAudioBitrate("192k");
    ofAddListener(this->videoRecorder.outputFileCompleteEvent, this, &ofApp::recordingComplete);
}

//--------------------------------------------------------------
void ofApp::update(){
    // check for messages from Matlab (UDP)
    char udpMessage[256];
    udpConnection.Receive(udpMessage,256);
    string message = udpMessage;
    if (message.length() == 1){
        this->state = ofToInt(message);
    }

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
//        case 2: // untested
//            if(this->videoRecorder.isInitialized()) {
//                this->videoRecorder.setPaused(true);
//            }
        default:
            break;
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
    if (key == 'q') {
        ofExit();
    }
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
void ofApp::audioIn(float *input, int bufferSize, int nChannels){
    if (state == 1) {
        this->videoRecorder.addAudioSamples(input, bufferSize, nChannels);
    }
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
    vector<ofVideoDevice> list = this->camera.listDevices();
    if (list.size() > 0) {
        cameraID = 0;
    }
    for (int kk=0; kk<list.size(); kk++) {
        if (list[kk].deviceName == "HD Webcam C615") {
            cameraID = list[kk].id;
            break;
        }
    }
    //grabber.setGrabber();
    this->camera.setDeviceID(cameraID);

    // The native pixel format for the ofxPS3EyeGrabber is a Bayer pattern
    // (it will look black and white).
    //this->camera.setPixelFormat(OF_PIXELS_NATIVE);
    this->camera.setDesiredFrameRate(this->desiredCameraFrameRate);
    this->camera.setup(this->cameraWidth, this->cameraHeight);

    //this->camera.getGrabber()->setAutogain(true);
    //this->camera.getGrabber()->setAutoWhiteBalance(true);
}

void ofApp::tryLoadingPreferencesOrDefaults() {
    this->settings = new ofxXmlSettings();
    bool success = this->settings->load("settings.xml");
    if (!success) {
        // set standard values and save settings
        this->setDefaultSettings();
        this->settings->save("settings.xml");
    }
    this->unpackSettings();
}

void ofApp::setDefaultSettings() {
    // write layout settings
    this->background_color = ofColor(200, 200, 200);
    this->settings->setValue("setting:color:background:red",   this->background_color.r);
    this->settings->setValue("setting:color:background:green", this->background_color.g);
    this->settings->setValue("setting:color:background:blue",  this->background_color.b);
    this->radiusRecordingIndicator = 15;
    this->settings->setValue("setting:layout:recIndicator:radius",  this->radiusRecordingIndicator);
    this->xRecordingIndicator = 100;
    this->settings->setValue("setting:layout:recIndicator:x",  this->xRecordingIndicator);
    this->yRecordingIndicator = 100;
    this->settings->setValue("setting:layout:recIndicator:y",  this->yRecordingIndicator);

    // write matlab network settings
    this->udp_port = 4245;
    this->settings->setValue("setting:network:udpPort", this->udp_port);

    // write camera settings
    this->desiredCameraFrameRate = 30;//75;
    this->settings->setValue("setting:camera:fps",    this->desiredCameraFrameRate);
    this->cameraWidth  = 640;
    this->settings->setValue("setting:camera:width",  this->cameraWidth);
    this->cameraHeight = 480;
    this->settings->setValue("setting:camera:height", this->cameraHeight);
}

void ofApp::unpackSettings() {
    int r,g,b;
    // unpack settings
    // read layout settings
    r = this->settings->getValue("setting:color:background:red",   0);
    g = this->settings->getValue("setting:color:background:green", 0);
    b = this->settings->getValue("setting:color:background:blue",  0);
    this->background_color = ofColor(r, g, b);
    this->radiusRecordingIndicator = this->settings->getValue("setting:layout:recIndicator:radius", 0);
    this->xRecordingIndicator = this->settings->getValue("setting:layout:recIndicator:x", 0);
    this->yRecordingIndicator = this->settings->getValue("setting:layout:recIndicator:y", 0);

    // read matlab network settings
    this->udp_port = this->settings->getValue("setting:network:udpPort", 0);

    // read camera settings
    this->desiredCameraFrameRate = this->settings->getValue("setting:camera:fps", 25);
    this->cameraWidth  = this->settings->getValue("setting:camera:width",  640);
    this->cameraHeight = this->settings->getValue("setting:camera:height", 480);
}
