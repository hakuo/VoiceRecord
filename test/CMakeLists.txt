cmake_minimum_required(VERSION 3.2)
project(voicerecord_test)

include_directories(lib/googletest/include)
include_directories(${PROJECT_MAIN_SOURCE_DIR})

# only build lib for the first time
# add_subdirectory(lib)

set(SOURCES main.cpp)

set(LIBS gtest pthread
         division)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})