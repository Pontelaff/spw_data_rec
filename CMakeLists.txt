#########################################################
# CMake file for the spw_data_rec project
# Author: Jonas Gesch (jonas.gesch@dlr.de)
# Date: 23.03.2022
#########################################################

cmake_minimum_required(VERSION 3.14.3)

# Project name and current version
project(spw_data_rec VERSION 0.4.1)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Control where the static and shared libraries and executable are built
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

# Select source files to build
add_executable(${PROJECT_NAME} src/main.c src/arg_parser.c src/data_logger.c src/LA_interface.c src/config_logger.c src/packet_archiver.c)

# Add include directories for header files
target_include_directories(${PROJECT_NAME} PUBLIC
                        "${PROJECT_SOURCE_DIR}/inc"
                        "/usr/local/STAR-Dundee/STAR-System/inc/star"
                        "/usr/local/STAR-Dundee/spw_la_mk3/inc"
                        "/usr/include/librdkafka"
                        "/usr/include/uuid"
                        "/usr/include/json-c"
                        )

# Add external libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
                        star-api
                        star_conf_api_brick_mk2
                        spw_la_api
                        librdkafka
                        uuid
                        json-c
                        )

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
