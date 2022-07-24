#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "udpReceiverThread.hpp"
#include "ofxVideoRecorder.h"

class ofApp : public ofBaseApp{
public:
    void exit();
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);
    void audioIn(float *input, int bufferSize, int nChannels);
    UdpReceiverThread* udp_thread;
    int state;

protected:
    ofxXmlSettings* settings;
    int udp_port;
    ofColor background_color;
    int radiusRecordingIndicator;
    float xRecordingIndicator, yRecordingIndicator;

    ofVideoGrabber camera;
    int cameraID;
    int desiredCameraFrameRate;
    int cameraWidth, cameraHeight;

    ofxVideoRecorder videoRecorder;
    string video_filename;
    string video_fileext;

    void tryLoadingPreferencesOrDefaults();
    void setDefaultSettings();
    void unpackSettings();
    void setupCamera();
    void drawRecordingIndicator(float x, float y, int recording_state);
    void recordingComplete(ofxVideoRecorderOutputFileCompleteEventArgs& args);
};
