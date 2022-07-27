# ofxUdpTriggeredVideoRecording

Openframeworks program to record video from a webcam.

The program receives UDP messages to control the recording.

The outline of a white circle is drawn as overlay on the video.

When recording is activated, this indicator will be filled with dark red.

## Setup

This code relies on [openframeworks](http://openframeworks.cc) and several addons.

Use the project generator to generate a build file for your IDE (MSVC, Xcode, Makefile).

The addons required are:

- ofxNetwork (part of the base distribution)
- ofxPoco
- ofxVideoRecorder
- ofxXmlSettings


## UDP messages

Originally, Matlab was used to send the UDP messages.

An example is included as a submodule (MatlabUDP).

If this folder is not included run `git submodule init && git submodule update`

The progam awaits 1 of the follwing string messages:

- 0
- 1

If a 1 is received, the indicator will be set to dark red and start the video recording.

Receiving a 0, will stop the video recording.

The UDP port is 4245 by default.


You can test this quickly bby running the OF program and then call:

```
python send_msg.py 1 # start recording
# or
python send_msg.py 0 # stop reacording
```


## Settings

After building the program a bin folder with the executable will be created.

There will be inside the bin folder.

After the first run the default settings.xml file will be created.

Inside the color, position and size of the recording indicator, the UDP port,
as well as the camera frame rate and video size can be changed without recompiling the program.

However, a restart is requiered.


## Controls

| Key  | Purpose      |
|:--------:|:--------:|
|  q   | Stop program |
