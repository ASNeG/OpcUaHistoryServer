cmake_minimum_required(VERSION 2.8.12.2)
project	(OpcUaHistoryServerTest)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# global options
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
set(OpcUaStack_INSTALL_DIR "/" CACHE STRING "OpcUaStack installation output directory")


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# libraries
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
add_definitions(-DBOOST_ALL_DYN_LINK)

find_package(Boost COMPONENTS system unit_test_framework filesystem thread date_time chrono regex REQUIRED)
find_library(OpcUaStackCore_LIBRARY NAMES OpcUaStackCore PATHS ${OpcUaStack_INSTALL_DIR}/usr/lib/OpcUaStack)
find_library(OpcUaStackClient_LIBRARY NAMES OpcUaStackClient PATHS ${OpcUaStack_INSTALL_DIR}/usr/lib/OpcUaStack)
find_library(OpcUaStackServer_LIBRARY NAMES OpcUaStackServer PATHS ${OpcUaStack_INSTALL_DIR}/usr/lib/OpcUaStack)


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# build 
# 
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
include_directories(
    ./
    ${PROJECT_BINARY_DIR}
    ../src/
	${Boost_INCLUDE_DIRS}
)

file(
    GLOB OpcUaHistoryServerTest_SRC 
    ${PROJECT_SOURCE_DIR}/*.cpp
)
add_executable(
    OpcUaHistoryServerTest
    ${OpcUaHistoryServerTest_SRC}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    OpcUaHistoryServerTest
    ${OpcUaStackCore_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${pthread}
    ${Boost_LIBRARIES}
)
