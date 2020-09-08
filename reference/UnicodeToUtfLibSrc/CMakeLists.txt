# For x64 compile, please use command
# mkdir build
# cd build
# cmake ..
# make -j8

# For arm cross compile, please use command
# First set your pc arm cross compile tool

# Build platformat arm9 cmd
# mkdir build
# cd build
# cmake -DPLATFORM=arm -DDEVICE=arm9 ..
# make -j8

# Build platformat a8 cmd
# mkdir build
# cd build
# cmake -DPLATFORM=arm -DDEVICE=a8 ..
# make -j8

cmake_minimum_required (VERSION 3.5)

# Set project name.
project(unitoutf8)

option(BUILD_SHARED_LIBS "Build shared library" ON)

# Set C and C++ build version.
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# set the library version
if(BUILD_SHARED_LIBS)
    set(VERSION_MAJOR 1)
    set(VERSION_MINOR 1)
    set(VERSION_PATCH 0)

    configure_file (
        "${CMAKE_CURRENT_LIST_DIR}/version/version.h.in"
        "${CMAKE_CURRENT_LIST_DIR}/version/version.h"
        )
    include_directories(${CMAKE_CURRENT_LIST_DIR}/version)
endif()

# Set the lib output path
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/build/libs/lib)
file(GLOB_RECURSE ALL_HEARD_FILE ${CMAKE_CURRENT_LIST_DIR}/include/*.h)
file(COPY ${ALL_HEARD_FILE} DESTINATION ${CMAKE_CURRENT_LIST_DIR}/build/libs/include)

if(PLATFORM MATCHES "arm")
    # Build different platformat code.
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR arm)

    # Set the build cross tool.
    set(CMAKE_CXX_COMPILER /opt/arm-2014.05/bin/arm-none-linux-gnueabi-g++)
    set(CMAKE_C_COMPILER   /opt/arm-2014.05/bin/arm-none-linux-gnueabi-gcc)
    
    set(ARM9_FLAGS "-w -O3")
    set(A8_FLAGS "-march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp")

    if(DEVICE MATCHES "arm9")
        # Set the C or C++ cross tool build flag.
        set(GNU_FLAGS "-mfpu=vfp -fPIC -g -W -O3")
        set(CMAKE_CXX_FLAGS "${GNU_FLAGS}")
        set(CMAKE_C_FLAGS   "${GNU_FLAGS}")
    elseif(DEVICE MATCHES "a8")
        # Set the C or C++ cross tool build flag.
        set(GNU_FLAGS "-march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp")
        set(CMAKE_CXX_FLAGS "${GNU_FLAGS}")
        set(CMAKE_C_FLAGS   "${GNU_FLAGS}")
    endif()

else()
    set(GNU_FLAGS "-DCOM_X64 -g -W")
    set(CMAKE_CXX_FLAGS "${GNU_FLAGS}")
    set(CMAKE_C_FLAGS   "${GNU_FLAGS}")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src ALL_SRC)

if(BUILD_SHARED_LIBS)
    add_library(unitoutf8 SHARED ${ALL_SRC})
else()
    add_library(unitoutf8 STATIC ${ALL_SRC})
endif()

set_property(TARGET unitoutf8 PROPERTY CXX_STANDARD 11)
set_property(TARGET unitoutf8 PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(unitoutf8)

# add_executable(demotest ${ALL_SRC})
# set_property(TARGET demotest PROPERTY CXX_STANDARD 11)
# set_property(TARGET demotest PROPERTY CXX_STANDARD_REQUIRED ON)
# target_link_libraries(demotest dl)