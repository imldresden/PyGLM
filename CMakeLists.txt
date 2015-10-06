cmake_minimum_required(VERSION 2.8.4)
project(pyglm)

set(SOURCE_FILES pyglm_wrapper.cpp)
set(CMAKE_SHARED_MODULE_PREFIX)
add_library(pyglm MODULE ${SOURCE_FILES})

include_directories("$ENV{AVG_PATH}/libavg/src")

find_package(PythonLibs REQUIRED)
include_directories("${PYTHON_INCLUDE_DIRS}")
target_link_libraries(pyglm ${PYTHON_LIBRARIES})

find_package(LibXml2 REQUIRED)
include_directories("${LIBXML2_INCLUDE_DIR}")
target_link_libraries(pyglm ${LIBXML2_LIBRARIES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

execute_process(COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
        OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
install(TARGETS pyglm LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
