# matlabOSCtoOFXpseyecam

Openframeworks program to record video from a webcam.

The program receives UDP messages to control the recording.

The outline of a white circle is drawn as overlay on the video.

When recording is activated, this indicator will be filled with dark red.


## UDP messages

Originally, Matlab was used to send the UDP messages.

An example is included as a submodule (MatlabUDP).

If this folder is not included run `git submodule init && git submodule update`

The progam listens on port 4245 and awaits 1 of the follwing string messages:

- 0
- 1
- -1

If a 1 is received, the indicator will be set to dark red and start the video recording.

Receiving a 0, will stop the video recording.

The -1 message has currently no function.


## Controls

| Key  | Purpose      |
|:--------:|:--------:|
|  q   | Stop program |
