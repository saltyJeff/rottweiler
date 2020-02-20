#
# Minimum version is 3.5 (this supports Ubuntu 16.04 and later, for example)
#
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

#
# Project version numbering using semantic versioning. See:
# https://semver.org/
#
# These are used to set VERSION and SOVERSION properties of the LibSerial
# libarary. See:
# - https://cmake.org/cmake/help/latest/prop_tgt/SOVERSION.html
# - https://cmake.org/cmake/help/latest/prop_tgt/VERSION.html
#
PROJECT(LibSerial LANGUAGES C CXX VERSION 1.0.0)

# modified build by saltyjeff
include_directories(include)
add_library (serial_lib STATIC
        src/SerialPort.cpp
        src/SerialStream.cpp
        src/SerialStreamBuf.cpp
        src/libserial/SerialPort.h
        src/libserial/SerialPortConstants.h
        src/libserial/SerialStream.h
        src/libserial/SerialStreamBuf.h
        )
target_include_directories(serial_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>
        $<INSTALL_INTERFACE:include/libserial>
        )
