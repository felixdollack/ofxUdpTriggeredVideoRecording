//
//  udpReceiverThread.cpp
//  matlabOSCtoOFXpseyecam
//
//  Created by Felix Dollack on 30.01.20.
//

#include "udpReceiverThread.hpp"

UdpReceiverThread::UdpReceiverThread(int osc_port) {
    this->_udp_receive_port = osc_port;
    this->_udp_manager.Create();
    this->_udp_manager.SetReceiveBufferSize(this->_buffer_size);
    this->_udp_manager.Bind(this->_udp_receive_port);
    this->_udp_buffer = new char[this->_buffer_size];
    
    this->received_state = -1;
    this->state = 0;
}

void UdpReceiverThread::start() {
    startThread();
}
void UdpReceiverThread::stop() {
    stopThread();
    this->_udp_manager.Close();
}

void UdpReceiverThread::threadedFunction() {
    while (this->isThreadRunning())
    {
        int bytesReceived = this->_udp_manager.Receive(this->_udp_buffer, this->_buffer_size);
        if (bytesReceived > 0) {
            this->_udp_buffer[bytesReceived] = '\0';
            this->received_state = ofToInt(this->_udp_buffer);
        }
        switch (this->received_state) {
            case 0:
                this->state = 0; // stop recording
                break;
            case 1:
                this->state = 1; // (start) recording
                break;
            case 2:
                this->state = -1; // shutdown
                break;
            default:
                break; // undefined
        }
    }
}

int UdpReceiverThread::readState() {
    return this->state;
}
