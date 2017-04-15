#!/bin/bash
su -c "cvlc --loop /server/stream.flac \
       --sout-keep \
       --sout \
       '#transcode{acodec=vorbis,ab=32,channels=1,samplerate=8000}:gather:rtp{port=1302,sdp=rtsp://0.0.0.0:1300/audio.ogg}'" \
       rtsp-server
