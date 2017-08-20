cmake_minimum_required(VERSION 3.8)
set(${CMAKE_PROJECT_NAME}_BOARD uno)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyUSB0)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
project(ir-thermo)

set(CMAKE_CXX_STANDARD 14)

link_directories(
        #/home/arne/Arduino/libraries/
        /home/arne/Arduino/libraries/Adafruit_MLX90614
        #/home/arne/Programs/arduino/libraries/
        #/home/arne/Programs/arduino/libraries/SPI
        #/home/arne/Programs/arduino/hardware/arduino/avr/libraries/
        /home/arne/Programs/arduino/hardware/arduino/avr/libraries/SPI
        /home/arne/Programs/arduino/hardware/arduino/avr/libraries/Wire
)

set(Wire_RECURSE true)

generate_arduino_firmware(IR-Arduino SKETCH IR-Arduino)
