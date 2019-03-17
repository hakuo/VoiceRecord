project(voip)

set(SOURCES VoIP.cpp)

set(LIBS pthread media ${GSTREAMER_LIBRARIES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})