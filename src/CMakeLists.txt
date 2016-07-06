set(floatingeye_SRC
  main.cpp
  )

set(floatingeye_HDR
  )

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
find_package(Irrlicht REQUIRED)
find_package(Threads)

include_directories(${PROJECT_SOURCE_DIR}/3rdparty/lsclient/include)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/IrrIMGUI/includes)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty/IrrIMGUI/dependency)
include_directories(${IRRLICHT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

add_definitions(-DIMGUI_DISABLE_TEST_WINDOWS)

add_executable(floatingeye ${floatingeye_SRC})
target_link_libraries(floatingeye ${OPENGL_LIBRARIES} ${IRRLICHT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} IrrIMGUI lsclient)

add_custom_command(TARGET floatingeye POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:floatingeye>
  )
