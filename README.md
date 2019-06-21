# Voice recording

An audio recording from gstreamer and store in FLAC.

## Dependencies

- gstreamer
- gtest
- gcov, lcov, genhtml

## Pipeline
```
Speaker:
gst-launch-1.0 -v alsasrc device=hw:1 ! audioconvert ! \
        audio/x-raw,format=S16LE,rate=44100,channels=1 ! \
        audioconvert ! rtpL16pay ! udpsink host=192.168.1.40  port=5000
        
Host listening:
gst-launch-1.0 -v udpsrc port=5000 ! \
    "application/x-rtp,media=(string)audio, clock-rate=(int)44100, \
    width=16, height=16, encoding-name=(string)L16, \
    encoding-params=(string)1, channels=(int)1, \
    channel-positions=(int)1, payload=(int)96" ! rtpL16depay ! \
    audioconvert ! alsasink sync=false 
```

## Unit test

### Setup build unit test
```
# in project root directory
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
```

### Generate xml/html test result

```
# in project root directory
mkdir build && cd build
./bin/voicerecord_test --gtest_output="xml:../test/utils/gtest_report/in/voicerecord_test.xml"
cd ../test/utils/gtest_report/
./generate.sh
```

### Generate html coverage report

```
lcov --directory . --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1
lcov --remove ./code_coverage.info '/usr/include/*' -o ./code_coverage.info -rc lcov_branch_coverage=1
genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report
```
