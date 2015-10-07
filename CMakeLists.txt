cmake_minimum_required(VERSION 2.8.4)
project(pyglm)

set(SOURCE_FILES pyglm_wrapper.cpp)
set(CMAKE_SHARED_MODULE_PREFIX)
add_library(pyglm MODULE ${SOURCE_FILES})

execute_process(COMMAND python -c "import os;import libavg;print os.path.dirname(libavg.__file__)"
        OUTPUT_VARIABLE AVG_LIB_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories("$ENV{AVG_PATH}/libavg/src")
target_link_libraries(pyglm ${AVG_LIB_PATH}/avg.so)

find_package(PythonLibs REQUIRED)
include_directories("${PYTHON_INCLUDE_DIRS}")
target_link_libraries(pyglm ${PYTHON_LIBRARIES})

find_package(LibXml2 REQUIRED)
include_directories("${LIBXML2_INCLUDE_DIR}")
target_link_libraries(pyglm ${LIBXML2_LIBRARIES})

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "${PYTHON_INCLUDE_DIRS}")
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost COMPONENTS python)
TARGET_LINK_LIBRARIES(pyglm ${Boost_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

execute_process(COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
        OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

install(TARGETS pyglm LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
