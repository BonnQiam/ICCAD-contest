cmake_minimum_required(VERSION 3.5)
project (test)

find_package(ZLIB REQUIRED)
find_package(Boost 1.55.0 REQUIRED COMPONENTS graph regex)

# deal with GCC dual ABI issue
set(CMAKE_CXX_ABI 0)

if(NOT CMAKE_CXX_ABI)
    set(CMAKE_CXX_ABI 0 CACHE STRING
        "Choose the value for _GLIBCXX_USE_CXX11_ABI, options are: 0|1."
        FORCE)
endif(NOT CMAKE_CXX_ABI)

message(STATUS "CMAKE_CXX_ABI: _GLIBCXX_USE_CXX11_ABI=${CMAKE_CXX_ABI}")
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=${CMAKE_CXX_ABI})

#set(COMPILE_DEFINITIONS "-D_GLIBCXX_USE_CXX11_ABI=0")

if(Boost_INCLUDE_DIRS)
    set(INCLUDES ${Boost_INCLUDE_DIRS})
    set(LIBS ${Boost_LIBRARIES})
else(Boost_INCLUDE_DIRS)
    set(LIBS Boost::boost)
endif(Boost_INCLUDE_DIRS)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${INCLUDES}
    )
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMPILE_DEFINITIONS DEBUG_GDSREADER DEBUG_GDSWRITER)
else()
    set(COMPILE_DEFINITIONS "")
endif()

add_executable(test Main.cpp)
set_target_properties(test PROPERTIES OUTPUT_NAME "test")
target_link_libraries(test PRIVATE gdsdb gdsparser gzstream ${LIBS} ${ZLIB_LIBRARIES})