cmake_minimum_required(VERSION 3.14)
project(ARIListener VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the vcpkg toolchain file (adjust if necessary)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# Include directories for header files
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/utility)

# List of source files
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/utility/*.cpp
)

# Define the executable
add_executable(ARIListener ${SOURCES})

# Find and link Boost libraries
find_package(Boost REQUIRED COMPONENTS system random program_options)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(ARIListener Boost::system Boost::random Boost::program_options)
else()
    message(FATAL_ERROR "Could not find Boost libraries.")
endif()

# Find and link websocketpp
find_package(websocketpp CONFIG REQUIRED)
if (websocketpp_FOUND)
    include_directories(${websocketpp_INCLUDE_DIRS})
    target_link_libraries(ARIListener websocketpp::websocketpp)
else()
    message(FATAL_ERROR "Could not find websocketpp libraries.")
endif()

# Find and link jsoncpp
find_package(jsoncpp CONFIG REQUIRED)
if (jsoncpp_FOUND)
    include_directories(${jsoncpp_INCLUDE_DIRS})
    target_link_libraries(ARIListener jsoncpp_lib)
else()
    message(FATAL_ERROR "Could not find jsoncpp libraries.")
endif()

# Find and link PcapPlusPlus
find_package(PcapPlusPlus CONFIG REQUIRED)
if (PcapPlusPlus_FOUND)
    include_directories(${PcapPlusPlus_INCLUDE_DIRS})
    target_link_libraries(ARIListener 
        PCAP::PCAP 
        Packet::Packet 
        PcapPlusPlus::Pcap++ 
        PcapPlusPlus::Common++
    )
else()
    message(FATAL_ERROR "Could not find PcapPlusPlus libraries.")
endif()

# Find and link libtins
find_package(libtins CONFIG REQUIRED)
if (libtins_FOUND)
    include_directories(${libtins_INCLUDE_DIRS})
    target_link_libraries(ARIListener tins)
else()
    message(FATAL_ERROR "Could not find libtins libraries.")
endif()

# Find and link libcurl
find_package(CURL REQUIRED)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    target_link_libraries(ARIListener ${CURL_LIBRARIES})
else()
    message(FATAL_ERROR "Could not find libcurl libraries.")
endif()

# Ensure the linker finds the libraries
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${websocketpp_LIBRARY_DIRS})
link_directories(${jsoncpp_LIBRARY_DIRS})
link_directories(${PcapPlusPlus_LIBRARY_DIRS})
link_directories(${libtins_LIBRARY_DIRS})
link_directories(${CURL_LIBRARY_DIRS})
