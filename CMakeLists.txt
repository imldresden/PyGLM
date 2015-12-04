cmake_minimum_required(VERSION 2.8.4)
project(pyglm)

set(SOURCE_FILES pyglm_wrapper.cpp)
set(CMAKE_SHARED_MODULE_PREFIX)
add_library(pyglm MODULE ${SOURCE_FILES})

execute_process(COMMAND python -c "import os;import libavg;print os.path.dirname(libavg.__file__)"
        OUTPUT_VARIABLE AVG_LIB_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories("$ENV{AVG_PATH}/libavg/src")
if (NOT APPLE)
    target_link_libraries(pyglm ${AVG_LIB_PATH}/avg.so)
endif()

find_package(PythonLibs REQUIRED)
include_directories("${PYTHON_INCLUDE_DIRS}")
target_link_libraries(pyglm ${PYTHON_LIBRARIES})

if (APPLE)
    execute_process(COMMAND xml2-config --cflags OUTPUT_VARIABLE LIBXML2_CFLAGS)
    add_definitions(${LIBXML2_CFLAGS})
    set(CMAKE_MODULE_LINKER_FLAGS "-undefined dynamic_lookup ${CMAKE_MODULE_LINKER_FLAGS} -lxml2")
else()
    find_package(LibXml2 REQUIRED)
    include_directories("${LIBXML2_INCLUDE_DIR}")
    target_link_libraries(pyglm ${LIBXML2_LIBRARIES})
endif()

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "${PYTHON_INCLUDE_DIRS}")
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost COMPONENTS python system)
TARGET_LINK_LIBRARIES(pyglm ${Boost_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

install(TARGETS pyglm LIBRARY DESTINATION ${AVG_LIB_PATH}/plugin)
