cmake_minimum_required(VERSION 2.8.12.2)
project	(OpcUaHistoryTest)

# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# global options
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
set(INSTALL_PREFIX_OpcUaStack "/" CACHE STRING "OpcUaStack installation output directory")


# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
#
# libraries
#
# -----------------------------------------------------------------------------
# -----------------------------------------------------------------------------
add_definitions(-DBOOST_ALL_DYN_LINK)

find_package(Boost COMPONENTS system unit_test_framework filesystem thread date_time chrono regex REQUIRED)
find_library(OpcUaStackCore_LIBRARY NAMES OpcUaStackCore PATHS ${INSTALL_PREFIX_OpcUaStack}/usr/lib/OpcUaStack)
find_library(OpcUaStackClient_LIBRARY NAMES OpcUaStackClient PATHS ${INSTALL_PREFIX_OpcUaStack}/usr/lib/OpcUaStack)
find_library(OpcUaStackServer_LIBRARY NAMES OpcUaStackServer PATHS ${INSTALL_PREFIX_OpcUaStack}/usr/lib/OpcUaStack)


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
    ${Boost_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/../src
    ${INSTALL_PREFIX_OpcUaStack}/usr/include/OpcUaStack
)

file(
    GLOB OpcUaHistoryTest_SRC 
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/*/*.cpp
    ${PROJECT_SOURCE_DIR}/../src/OpcUaHistory/HistoryAccess/*.cpp
    ${PROJECT_SOURCE_DIR}/../src/OpcUaHistory/HistoryAccessFile/*.cpp
)
add_executable(
    OpcUaHistoryTest
    ${OpcUaHistoryTest_SRC}
    ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    OpcUaHistoryTest
    ${OpcUaStackCore_LIBRARY}
    ${CMAKE_DL_LIBS}
    ${pthread}
    ${Boost_LIBRARIES}
)
