cmake_minimum_required(VERSION 3.14)
project(ARIListener VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the vcpkg toolchain file (adjust if necessary)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# Find and link Boost libraries
find_package(Boost REQUIRED COMPONENTS system random)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find Boost libraries.")
endif()

# Find and link websocketpp
find_package(websocketpp CONFIG REQUIRED)
if (websocketpp_FOUND)
    include_directories(${websocketpp_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find websocketpp libraries.")
endif()

# Find and link jsoncpp
find_package(jsoncpp CONFIG REQUIRED)
if (jsoncpp_FOUND)
    include_directories(${jsoncpp_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find jsoncpp libraries.")
endif()

# Find and link PcapPlusPlus
find_package(PcapPlusPlus CONFIG REQUIRED)
if (PcapPlusPlus_FOUND)
    include_directories(${PcapPlusPlus_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Could not find PcapPlusPlus libraries.")
endif()

# Include directory for header files
include_directories(include)

# List of source files
set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/ari_event_listener.cpp
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/rtp_capture.cpp
)

# Define the executable
add_executable(ARIListener ${SOURCES})

# Link the necessary libraries
target_link_libraries(ARIListener
    Boost::system
    Boost::random
    websocketpp::websocketpp
    jsoncpp_lib
    PCAP::PCAP
        Packet::Packet
        PcapPlusPlus::Pcap++
        PcapPlusPlus::Common++
)

# Ensure the linker finds the libraries
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${websocketpp_LIBRARY_DIRS})
link_directories(${jsoncpp_LIBRARY_DIRS})
link_directories(${PcapPlusPlus_LIBRARY_DIRS})
