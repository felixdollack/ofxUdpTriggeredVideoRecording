//
//  udpReceiverThread.hpp
//  matlabOSCtoOFXpseyecam
//
//  Created by Felix Dollack on 30.01.20.
//

#ifndef udpReceiverThread_hpp
#define udpReceiverThread_hpp

#include "ofMain.h"
#include "ofxNetwork.h"

class UdpReceiverThread : public ofThread {
public:
    UdpReceiverThread(int osc_port);
    void start();
    void stop();
    int readState();

protected:
    const int _buffer_size = 32;
    ofxUDPManager _udp_manager;
    int _udp_receive_port;
    void threadedFunction();
    void updateData();
    char* _udp_buffer;
    int received_state;
    int state;
};
#endif /* udpReceiverThread_hpp */
