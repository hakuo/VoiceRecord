cmake_minimum_required(VERSION 3.2)
project(voicerecord)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set(SOURCES main.cpp
            voip/VoIP.cpp
            media/Media.cpp)

set(LIBS pthread
         FLAC FLAC++
         ${GSTREAMER_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCES})

# build static library for unit test
if(BUILD_TESTS)
    add_subdirectory(voip)
    add_subdirectory(media)
endif()

target_link_libraries(${PROJECT_NAME} ${LIBS})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
