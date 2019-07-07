# Gstreamer 

## Record audio and stream through udpsink:

```
# Microphone side:
# sample rate = 16000, channels = 1
gst-launch-1.0 -v alsasrc device=hw:1 ! audioconvert ! audio/x-raw,format=S16LE,rate=16000,channels=1 ! audioconvert ! rtpL16pay ! udpsink host=192.168.12.50  port=5000

# Host side:
gst-launch-1.0 -v udpsrc port=5000 ! "application/x-rtp,media=(string)audio, clock-rate=(int)16000, width=16, height=16, encoding-name=(string)L16, width=16, height=16, encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, channel-positions=(int)1, payload=(int)96" ! rtpL16depay ! audioconvert ! alsasink sync=false
```

## Play audio through udpsink

```
# Host side
gst-launch-1.0 -v filesrc location=./test_1.mp3  ! decodebin ! audioconvert ! rtpL16pay ! udpsink host=192.168.12.192 port=5000

# Speaker side
gst-launch-1.0  udpsrc port=5000 ! "application/x-rtp, media=(string)audio, clock-rate=(int)16000, encoding-name=(string)L16, encoding-params=(string)1, channels=(int)1, payload=(int)96" ! rtpL16depay ! audioconvert ! alsasink sync=false

# Note convert audio to 16kHz, 1 channels
ffmpeg -i input.mp3 -ar 16000 -ac 1 output.mp3
```
