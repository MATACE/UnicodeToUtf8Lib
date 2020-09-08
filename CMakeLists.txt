#
# For x64 compile, please use command
# cmake ..
# For arm cross compile, please use command
# Build platformat arm9
# mkdir build
# cd build
# cmake ..
# make -j8
#

cmake_minimum_required (VERSION 3.5)

# Set project name.
project(LibTest)

# Set C and C++ build version.
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Build different platformat code.
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Set the build cross tool.
set(CMAKE_CXX_COMPILER /opt/arm-2014.05/bin/arm-none-linux-gnueabi-g++)
set(CMAKE_C_COMPILER   /opt/arm-2014.05/bin/arm-none-linux-gnueabi-gcc)

# Set the C or C++ cross tool build flag.
set(GNU_FLAGS "-mfpu=vfp -fPIC -g -W -O2")
set(CMAKE_CXX_FLAGS "${GNU_FLAGS}")
set(CMAKE_C_FLAGS   "${GNU_FLAGS}")

# Include the third use libs. 
include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/unicode_to_utf8/include)

link_directories(${CMAKE_CURRENT_LIST_DIR}/libs/unicode_to_utf8/lib)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src ALL_SRC)

add_executable(demotest ${ALL_SRC})
set_property(TARGET demotest PROPERTY CXX_STANDARD 11)
set_property(TARGET demotest PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(demotest dl 
    ${CMAKE_CURRENT_LIST_DIR}/libs/unicode_to_utf8/lib/libunitoutf8.a)
    # unitoutf8)



