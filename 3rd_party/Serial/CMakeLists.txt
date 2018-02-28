cmake_minimum_required(VERSION 2.8.3)
project(serial)

include_directories(include)

set(serial_SRCS src/serial.cc include/serial/serial.h include/serial/v8stdint.h)
if(UNIX)
    list(APPEND serial_SRCS src/impl/unix.cc)
else()
    list(APPEND serial_SRCS src/impl/win.cc)
endif()

## Add serial library
add_library(serial STATIC ${serial_SRCS})
if(UNIX AND NOT APPLE)
    target_link_libraries(serial rt)
endif()