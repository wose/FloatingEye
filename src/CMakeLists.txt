set(floatingeye_SRC
  main.cpp
  )

set(floatingeyecore_SRC
  EmptyWidget.cpp
  Grid.cpp
  MapWidget.cpp
  SensorWidget.cpp
  State.cpp
  Vessel.cpp
  VideoStream.cpp
  )

set(floatingeyecore_HDR
  DoubleSensorWidget.h
  EmptyWidget.h
  Grid.h
  IntSensorWidget.h
  MapWidget.h
  Sensor.h
  SensorWidget.h
  State.h 
  Vessel.h
  VideoStream.h
  IWidget.h
  )

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
find_package(Irrlicht REQUIRED)
find_package(Threads)
find_package(LIBVLC REQUIRED)

include_directories(
  ${LIBVLC_EXTRA_INCLUDE_DIR}
  ${LIBVLC_EXTRA_INCLUDE_DIR}/vlc/plugins
  ${LIBVLC_INCLUDE_DIR}
  ${LIBVLC_INCLUDE_DIR}/vlc/plugins
  )

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/lsclient/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/IrrIMGUI/includes)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/IrrIMGUI/dependency)
include_directories(${IRRLICHT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_definitions(-DIMGUI_DISABLE_TEST_WINDOWS)

add_library(floatingeyecore ${floatingeyecore_SRC})
target_link_libraries(floatingeyecore
  ${OPENGL_LIBRARIES}
  ${IRRLICHT_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
  ${LIBVLC_LIBRARY}
  ${LIBVLCCORE_LIBRARY}
  IrrIMGUI
  lsclient
  )

add_executable(floatingeye ${floatingeye_SRC})
target_link_libraries(floatingeye
  floatingeyecore
  )

add_custom_command(TARGET floatingeye POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:floatingeye>
  )
