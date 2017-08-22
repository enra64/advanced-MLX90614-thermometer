cmake_minimum_required(VERSION 3.8)
set(${CMAKE_PROJECT_NAME}_BOARD nano)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)
set(ARDUINO_CPU atmega328)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
project(ir-thermo)

set(CMAKE_CXX_STANDARD 14)

set(
        ARDUINO_USER_LIB_PATH /home/arne/Arduino/libraries
        ARDUINO_SYSTEM_LIB_PATH /home/arne/Programs/arduino/hardware/arduino/avr/libraries
)

#generate_arduino_library(U8g2 BOARD nano SRCS ${ARDUINO_USER_LIB_PATH}/U8g2/src/U8g2lib.h
#        HDRS ${ARDUINO_USER_LIB_PATH}/U8g2/src/U8g2lib.h NO_AUTOLIBS MANUAL)

link_directories(
        ${ARDUINO_USER_LIB_PATH}/Adafruit_MLX90614
        ${ARDUINO_SYSTEM_LIB_PATH}/SPI
        ${ARDUINO_SYSTEM_LIB_PATH}/Wire
        Bounce2
        U8g2
)

set(
        Wire_RECURSE true
        U8g2_RECURSE true
        SPI_RECURSE true
)

generate_arduino_firmware(
        IR-Arduino
        BOARD nano
        SKETCH IR-Arduino)
