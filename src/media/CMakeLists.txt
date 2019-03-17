project(media)

set(SOURCES Media.cpp)

#set(LIBS pthread ${GSTREAMER_LIBRARIES})

add_library(${PROJECT_NAME} STATIC ${SOURCES})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})