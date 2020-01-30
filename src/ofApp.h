#pragma once

#include "ofMain.h"
#include "udpReceiverThread.hpp"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void windowResized(int w, int h);
    void gotMessage(ofMessage msg);

    UdpReceiverThread* udp_thread;
    int state;

protected:
    void drawRecordingIndicator(float x, float y, int recording_state);
};
