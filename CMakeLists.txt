cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(Odrive_USB)

find_path(LIBUSB_INCLUDE_DIR
  NAMES libusb.h
  PATH_SUFFIXES "3rd_party" "include" "libusb" "libusb-1.0")

find_library(LIBUSB_LIBRARY
  NAMES usb
  PATH_SUFFIXES "lib" "lib32" "lib64")
  
  
add_subdirectory(3rd_party/Serial)
include_directories(3rd_party/Serial/include)


include_directories(3rd_party/libusb-1.0.21/include)
include_directories(3rd_party/nlohmann)


add_executable(Odrive_USB src/main.cpp src/ODrive.cpp src/ODrive.h src/endpoint.cpp src/endpoint.h)
target_link_libraries(Odrive_USB serial ${LIBUSB_LIBRARY})
