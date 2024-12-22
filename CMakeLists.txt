cmake_minimum_required(VERSION 3.10)

project(NetworkRightsChecker)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Configure and Include EBCpp
set(EBCPP_USE_GUI Off)          # On/Off - Using the windows gui functions (only for testing)
set(EBCPP_USE_SSL Off)           # On/Off - Using openssl for ssl sockets and https
set(EBCPP_USE_SSL_STATIC Off)    # On/Off - Activate static linking of openssl into your application (see openssl license)
set(EBCPP_USE_SQLITE Off)        # On/Off - Using sqlite in your application (static linked)
set(EBCPP_STATIC On)            # On/Off - Static linking of libc, libc++ and winpthread

include(dep/EBCpp/ebcpp.cmake)

add_subdirectory(src)

