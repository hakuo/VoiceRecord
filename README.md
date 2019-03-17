# Voice recording

An audio recording from gstreamer and store in FLAC.

## Dependencies

- gstreamer
- gtest
- gcov, lcov, genhtml

## Pipeline
```
gst
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