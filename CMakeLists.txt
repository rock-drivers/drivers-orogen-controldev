PROJECT(controldev)
cmake_minimum_required(VERSION 2.6)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/.orogen/config")
INCLUDE(controldevBase)

INSTALL(FILES ControlDevTypes.hpp DESTINATION include)

# FIND_PACKAGE(KDL)
# FIND_PACKAGE(OCL)

