cmake_minimum_required(VERSION 3.8)
set(${CMAKE_PROJECT_NAME}_BOARD nano)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)
set(ARDUINO_CPU atmega328)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
project(ir-thermo)

set(CMAKE_CXX_STANDARD 14)



link_directories(
        #/home/arne/Arduino/libraries/
        /home/arne/Arduino/libraries/Adafruit_MLX90614
        /home/arne/Arduino/libraries/Bounce2
        #/home/arne/Programs/arduino/libraries/
        #/home/arne/Programs/arduino/libraries/SPI
        #/home/arne/Programs/arduino/hardware/arduino/avr/libraries/
        /home/arne/Programs/arduino/hardware/arduino/avr/libraries/SPI
        /home/arne/Programs/arduino/hardware/arduino/avr/libraries/Wire
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
